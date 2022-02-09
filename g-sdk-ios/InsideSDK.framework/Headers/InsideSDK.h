//
//  InsideSDK.h
//  InsideSDK
//
//  Created by piaotj on 2020/11/05.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#import <Foundation/Foundation.h>
#import <Speech/Speech.h>

//! Project version number for InsideSDK.
FOUNDATION_EXPORT double InsideSDKVersionNumber;

//! Project version string for InsideSDK.
FOUNDATION_EXPORT const unsigned char InsideSDKVersionString[];

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

#define GRPC_INIT_SUCCESS       0   // gRPC 연결 성공
#define GRPC_INIT_FAIL          1   // gRPC 연결 실패
#define GRPC_DISCONNECTED       2   // gRPC 연결 끊김
#define GO_TO_STANDBY           3   // 요청 정상 처리, 요청에 대한 응답 TTS 없음
#define SET_CONFIG_SUCCESS      4   // agent_setConfig 요청 정상 처리
#define SET_CONFIG_FAIL         5   // agent_setConfig 요청 실패
// 2021-10-12 Add S2S_REQUEST
#define S2S_REQUEST             6   // 음성인식 결과에 대해 S2S 요청
#define SERVER_ERROR            205 // 서버 에러

#define TIMEOUT_START_VOICE     601
#define TIMEOUT_STOP_VOICE      602
#define TIMEOUT_RECEIVE_DATA    603

@protocol InsideSDKDelegate <NSObject>

- (void) agent_onCommand:(NSString * _Nonnull) actionType:(NSDictionary * _Nonnull) payload
//- (void) agent_onCommand:(NSString * _Nonnull) actionType:(NSString * _Nonnull) payload
    NS_SWIFT_NAME(agent_onCommand(_:_:));
- (void) agent_onEvent:(int) evt:(NSDictionary * _Nullable) opt
    NS_SWIFT_NAME(agent_onEvent(_:_:));

@end

//@class InsideSDK;

@interface InsideSDK: NSObject

@property (nonatomic, weak) id<InsideSDKDelegate> delegate;

/// insideSDK agent_register()
/// @param clientId clientId
/// @param clientKey clientKey
/// @param clientSecret clientSecret
/// @param userId userId
- (NSDictionary * _Nonnull)agent_register:(NSString * _Nonnull)clientId
                                         :(NSString * _Nonnull)clientKey
                                         :(NSString * _Nonnull)clientSecret
                                         :(NSString * _Nullable)userId;

/// insideSDK agent_unregister
- (NSDictionary * _Nonnull)agent_unregister;

/// insideSDK agent_init
/// @param clientId clientId
/// @param clientKey clientKey
/// @param clientSecret clientSecret
/// @param uuid uuid
- (NSDictionary * _Nonnull)agent_init:(NSString * _Nonnull)clientId
                               :(NSString * _Nonnull)clientKey
                               :(NSString * _Nonnull)clientSecret
                               :(NSString * _Nonnull)uuid;

/// insideSDK agent_reset
- (void)agent_reset;

- (void)agent_updateMediaStatus:(int)ch
                               :(NSString * _Nonnull)state
                               :(int)playtime;

- (void)agent_setLocation:(NSString * _Nonnull)Longitute
                               :(NSString * _Nonnull)Latitue
                               :(NSString * _Nonnull)Address;

- (void)agent_sendText:(NSString * _Nonnull)msg;

- (void)agent_startVoice;
- (void)agent_startVoiceToText;
- (void)agent_sendVoice:(NSData * _Nonnull)data;
//                               :(NSInteger * _Nonnull)size;
- (void)agent_stopVoice;
- (NSDictionary * _Nullable)agent_getTTS:(NSString * _Nonnull)msg;
// 2021-10-19 Remove < reason DE Server Issue
//- (NSDictionary * _Nullable)agent_getTTS:(NSString * _Nonnull)msg
//                                        :(int)sampleRates;
// 2021-10-12 Add Language, Speed
- (NSDictionary * _Nullable)agent_getTTS_options:(NSString * _Nonnull)msg
                                        :(int)sampleRates
                                        :(NSString * _Nullable)language
                                        :(int) speed;
- (void)agent_sendCommand:(NSString * _Nonnull)cmd;
- (NSDictionary * _Nullable)agent_serviceLogin:(NSString * _Nonnull)service_type
                               :(NSString * _Nullable)return_url;
- (NSDictionary * _Nullable)agent_serviceLoginStatus:(NSString * _Nonnull)service_type;
- (NSDictionary * _Nullable)agent_serviceLogout:(NSString * _Nonnull)service_type;
- (void)agent_setServerInfo:(NSString * _Nonnull)serverIP
                               :(NSString * _Nonnull)grpcPort
                               :(NSString * _Nonnull)restPort;
- (NSString * _Nonnull)agent_getVersion;
- (void)agent_debugmode;
- (void)agent_setCustomVersion:(NSString * _Nonnull)appName
                               :(NSString * _Nonnull)version;
- (void)agent_setConfig:(NSString * _Nonnull)msg;

// KWS
- (int)kws_init:(int)kwsId;
- (int)kws_detect:(short * _Nonnull)data
                               :(int)size;
- (void)kws_reset;
- (int)kws_error;
- (NSString * _Nonnull)kws_getVersion;
- (int)kws_getKeyword;
- (int)kws_setKeyword:(int)kwsId;

- (NSString *)copyToDocument;

@end

