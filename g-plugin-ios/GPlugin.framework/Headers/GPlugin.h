//
//  GPlugin.h
//  GPlugin
//
//  Created by EungShik Kim on 28/11/2017.
//  Copyright © 2017 KT. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

//! Project version number for GPlugin.
FOUNDATION_EXPORT double GPluginVersionNumber;

//! Project version string for GPlugin.
FOUNDATION_EXPORT const unsigned char GPluginVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <GPlugin/PublicHeader.h>


// MARK: - Error Code (Int)
#define NONE                    1000
#define REC_FAILED              1001
#define CFK_TIMEOUT             1002
#define CFK_FAILED_CONN         1003
#define CFK_OTHERS              1004
#define CFK_LOGS                1005
#define START_REC_FAILED        1006
#define CFK_NULL                1007
#define FAIL_UNSUPPORTED_API_V1 1008

/*!
 *  @brief     GPlugin(singleton) Notification Key
 */
NS_ASSUME_NONNULL_BEGIN
static NSString * GPLUGIN_NOTIFICATION_RESULT = @"GPluginNotificationResult";
static NSString * GPLUGIN_NOTIFICATION_TYPE_KEY = @"_key";   // onError, onResult, onEvent
static NSString * GPLUGIN_NOTIFICATION_TYPE_ERROR = @"onError";
static NSString * GPLUGIN_NOTIFICATION_TYPE_RESULT = @"onResult";
static NSString * GPLUGIN_NOTIFICATION_TYPE_EVENT = @"onEvent";
static NSString * GPLUGIN_NOTIFICATION_TYPE_VOICE_MODE = @"onVoiceMode";    // type
static NSString * GPLUGIN_NOTIFICATION_TYPE_RMS_CHANGED = @"onRmsChanged";  // isVoiceBegun, rmsdB
static NSString * GPLUGIN_NOTIFICATION_TYPE_AUDIO_SESSION = @"onInitAudioSession";
static NSString * GPLUGIN_NOTIFICATION_TYPE_AUDIO_SESSION_TTS = @"onInitAudioSessionTTS";
static NSString * GPLUGIN_NOTIFICATION_TYPE_ONCOMMAND = @"onCommand"; //2.0
NS_ASSUME_NONNULL_END


@class GPlugin;


////////////////////////////////////////////////////////////////////////////////
// GPlugin 델리게이트 메소드 (리스너 메소드)
@protocol GPluginListener <NSObject>
@optional
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onError:(NSError * _Nonnull)error;
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onResults:(NSDictionary * _Nonnull)results;
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onEvent:(NSDictionary * _Nonnull)event;
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onRmsChanged:(BOOL)isVoiceBegun rmsdB:(float)rmsdB;
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onVoiceMode:(int)type __deprecated;  // type = 0: 호출어 인식 상태, type = 2: 명령어 인식 상태
- (BOOL)gPlugin:(GPlugin * _Nonnull)gPlug onInitAudioSession:(AVAudioSession * _Nonnull)audioSession;
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onInitAudioSessionTTS:(AVAudioSession * _Nonnull)audioSession; //for tts
- (void)gPlugin:(GPlugin * _Nonnull)gPlug onCommand:(NSDictionary * _Nonnull)result;
@end
////////////////////////////////////////////////////////////////////////////////

@interface GPlugin: NSObject

- (instancetype _Nonnull)init UNAVAILABLE_ATTRIBUTE;     // Using +[createGPlugin:] instead
+ (instancetype _Nonnull)new UNAVAILABLE_ATTRIBUTE;      // Using +[createGPlugin:] instead

// MARK: - Shared instance of G-Plugin
/*!
 *  @brief      GPlugin Singleton Instance
 *  @details    If using singleton, could be notified if using addObserver()
 */
+ (GPlugin * _Nonnull)shared;

////////////////////////////////////////////////////////////////////////////////
// GPlugin2.0 생성
/*!
 *  @param      delegate - Delegator, You can also using setGPluginListner, it works same.
 *  @param      usingNotification - if YES using Notification(Singleton) as along with delegate.
 *  @param      clientId - clientId
 *  @param      clientKey - clientKey
 *  @param      clientSecret - clientSecret
 *  @param      userId - userKey
 */
+ (void)init:(id _Nullable)delegate
preferSingleton:(BOOL)usingNotification
    clientId:(NSString * _Nonnull)clientId
   clientKey:(NSString * _Nonnull)clientKey
clientSecret:(NSString * _Nonnull)clientSecret
      userId:(NSString * _Nonnull)userId
    complete:(void(^_Nullable)(id _Nullable gplugObject))complete;

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin 생성
/*!
 *  @param      delegate - Delegator, You can also using setGPluginListner, it works same.
 *  @param      usingNotification - if YES using Notification(Singleton) as along with delegate.
 *  @return     GPlugin
 */
+ (GPlugin * _Nonnull)createGPlugin:(id _Nullable)delegate preferSingleton:(BOOL)usingNotification;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin 리스너, 번들 설정
/*!
 *  @brief      GPlugin delegater(listener) setup
 *  @details    set delegate
 *  @param      listener - Delegator, You can also set in createGPlugin, it works same.
 */
- (void)setGPluginListener:(id <GPluginListener> _Nonnull)listener;

/*!
 *  @brief      GPlugin setup application bundle
 *  @details    setup application bundle if needed
 *  @param      bundleDictionary - Dictionary of application bundle information
 */
- (void)setAppBundle:(NSDictionary * _Nonnull)bundleDictionary;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin 시작 / 종료
- (void)startGPlugin;
- (void)stopGPlugin;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin 기타 메소드
- (void)requestApp:(NSString * _Nonnull)appId;
- (void)setLogVisible:(BOOL)flag; //로그 보기 설정
- (NSString * _Nonnull)getGPluginVersion; // GPlugin 버전 정보 확인
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin 대화 서버 설정
- (void)sendEvent:(NSDictionary * _Nonnull)event; //대화서버에 이벤트 전달
- (void)setLoginId:(NSString * _Nonnull)loginID; //대화서버에 전달할 로그인ID 설정
- (void)updateServiceJoin:(int)appId isJoined:(BOOL)isJoined; //대화서버에 전달할 서비스 가입여부 설정
- (void)setServiceAppId:(NSString * _Nonnull)appId; //대화서버에 전달할 서비스 대상 단말 식별자(appId) 설정
- (void)updateLocInfo:(NSString * _Nonnull)addr withDestAddress:(NSString * _Nullable)destAddr withCAddress:(NSString * _Nullable)cAddr; //대화서버에 전달할 위치정보(주소) 설정
- (void)setDssDelayTime:(int)millisecond; //대화서버 호출 전 딜레이 타임 설정
- (void)enableDSS:(BOOL)flag; //대화서버 사용 여부 설정
- (BOOL)checkDSSAvailable; //대화서버 사용 여부 확인
- (void)setDssServerType:(BOOL)type; //개발서버, 상용서버 여부 설정
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin  KWS(Key Word Spot) 관련
- (int)getKwsId; //현재 설정된 호출어 확인
- (void)setKwsId:(int)kwsId; //호출어 설정
- (void)enableKws:(BOOL)isOn; //호출어 사용 여부 설정
- (BOOL)checkKwsAvailable; //호출어 사용 여부 확인
- (void)startKws; //호출어 인식 시작
- (void)stopKws;  //호출어 인식 종료
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin  STT(Speech-To-Text) 관련
- (BOOL)isRecognitionAvailable; //음성인식 가능 여부
- (void)setVoiceApiKey:(NSString * _Nonnull)apiKey;
- (void)startVoiceCmd;
- (void)startVoiceCmd:(CGFloat)timeout;  // default: 1.2 seconds
- (void)stopVoiceCmd;
- (BOOL)isWakeUp;
- (void)setInitAudioSession:(BOOL)isOverriding audioSession:(void(^_Nonnull)(void))completion;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin  TTS(Text-To-Speech) 관련
- (void)startTTS __attribute__((deprecated("GPlugin 2.0부터 지원안합니다.")));
- (BOOL)isTTSPlaying;
- (void)stopTTS;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// GPlugin  통화 관련
- (void)enableCall:(BOOL)isOn;
- (void)makeCall:(NSString * _Nonnull)callNumber;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// GPlugin2.0
- (void)voiceCommand;   //GRPC SERVICEM 시작 VOCM
- (void)textCommand:(NSString * _Nonnull)msg reqType:(NSString * _Nonnull)type; //GRPC SERVICEM 시작 TXCM
- (void)setMediaStatus:(int)channel status:(NSString * _Nonnull)status playtime:(int)playtime; //grpc music controll
- (NSString * _Nullable)serviceLogin:(NSString * _Nonnull)service_type url:(NSString * _Nonnull)custom_url; //Service Login
- (void)getTTS:(NSString * _Nonnull)msg withComplete:(void(^_Nullable)(id _Nullable dataObject))complete;   //text를 음성데이터로 전달
- (void)setServerInfo:(NSString * _Nonnull)serverIP grpcPort:(NSString * _Nonnull)grpcPort restPort:(NSString * _Nonnull)restPort; //연동 서버 주소 변경
- (void)updateLocInfoV2:(NSString * _Nonnull)longitude latitude:(NSString * _Nonnull)latitude address:(NSString * _Nonnull)address; //단말 위치 설정

// 테스트 용도의 임시 함수
- (void)setGPlugInVersion:(NSString * _Nonnull)version;
- (void)setVoiceCmdForKws:(BOOL)flag; // v2에서 startVoiceCmd를 사용하기 위한 함수.
//#if 0
- (void)setWebSocket:(BOOL)flag;    //websocket을 사용여부 true : 사용, false : 미사용
//#endif
////////////////////////////////////////////////////////////////////////////////


@end
