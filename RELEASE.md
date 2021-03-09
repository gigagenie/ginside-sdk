# GiGA Genie Inside SDK release notes

이 문서는 기가지니 인사이드 SDK의 릴리즈 노트로 신규 기능 및 변경 사항을 포함합니다.
## Device SDK for iOS v1.3.0 (2021-03-09)
### initial release

* initial public release
* iOS 9.0 or later supported
* 기가지니 호출어 라이브러리 kt-EasyVoi.1.0.748 적용

## Device SDK for Android v1.2.1 (2021-01-18)

* (ADD) agent_startVoiceToText API 추가

## Device SDK for Linux v1.2.2 (2020-11-06)
* (FIX) agent_register 호출 시 HTTP/1.1 400 Bad Request Case 수정
* (FIX) agent_setConfig API parameter type 현행화

## Mobile SDK for iOS v2.0.3 (2020-09-21)
* (FIX) 버전명 "beta" 삭제
* (ADD) agent_serviceLoginStatus API, agent_serviceLogout API 추가
* (ADD) SDK 버전 정보 서버로 전송 : g-plugin-ios/2.0.3 (SDK_VERSION)

## Device SDK for Linux-WebSocket v1.2.0 (2020-09-07)
* (ADD) agent_setConfig API 추가
* (ADD) agent_onEvent : SET_CONFIG_SUCCESS, SET_CONFIG_FAIL 추가
* (ADD) agent_setCustomVersion API 추가

## Device SDK for Linux/Android/Windows v1.2.0 (2020-08-25)
#### Common
* (ADD) agent_setConfig API 추가
* (ADD) agent_onEvent : SET_CONFIG_SUCCESS, SET_CONFIG_FAIL 추가
* (ADD) agent_setCustomVersion API 추가
#### Android Device SDK
* (FIX) dssStatus가 없는 경우 해당 key에 대해 빈값[]으로 서버에 전송하도록 수정

## Device SDK for Linux-WebSocket v1.1.0 (2020-06-18)
* (NEW) initial public release
  - SEE g-sdk-linux-ws/

## Device SDK v1.1.0 (2020-06-10)
#### Common
* Version up from v1.0.x to v1.1.x
  - 주요 변경사항은 [Upgrading_from_v1.0_to_v1.1.md](docs/Upgrading_from_v1.0_to_v1.1.md) 참고
#### Android Device SDK
* (UPDATE) agent_onCommand: Parameter 변경
* (UPDATE) agent_onCommand: actionType name 변경
* (UPDATE) agent_onEvent의 START_VIOCE, STOP_VOICE를 agent_onCommand로 대체
* (UPDATE) agent_getTTS API return 방식 변경: 기존 agent_onCommand에서 API 호출 시 바로 return 받도록 수정
* (ADD) agent_serviceLoginStatus API, agent_serviceLogout API 추가
* (ADD) SDK 버전 정보 서버로 전송 : g-sdk-android/1.1.0 (SDK_VERSION)
* (FIX) debugmode : true 인 경우에만 log 정보 전달
#### Linux Device SDK
* (UPDATE) agent_onCommand: Parameter 변경
* (UPDATE) agent_onCommand: actionType name 변경
* (UPDATE) agent_onEvent의 START_VIOCE, STOP_VOICE를 agent_onCommand로 대체
* (UPDATE) agent_getTTS API return 방식 변경: 기존 agent_onCommand에서 API 호출 시 바로 return 받도록 수정
* (ADD) agent_serviceLoginStatus API, agent_serviceLogout API 추가
* (ADD) x-client-version 추가 : g-sdk-linux/1.1.0 (SDK_VERSION)
* (FIX) Res_STCK 수신 후 음성인식 Timer 해제
#### Windows Device SDK
* (NEW) 32bit 운영체제 지원
* (UPDATE) agent_onCommand: Parameter 변경
* (UPDATE) agent_onCommand: actionType name 변경
* (UPDATE) agent_onEvent의 START_VIOCE, STOP_VOICE를 agent_onCommand로 대체
* (UPDATE) agent_getTTS API return 방식 변경: 기존 agent_onCommand에서 API 호출 시 바로 return 받도록 수정
* (ADD) agent_serviceLoginStatus API, agent_serviceLogout API 추가
* (ADD) x-client-version 추가 : g-sdk-windows/1.1.0 (SDK_VERSION)
* (FIX) Res_STCK 수신 후 음성인식 Timer 해제

## Mobile SDK for iOS v2.0.2beta (2020-04-22)
* (UPDATE) Snd_SVEV에서 type이 "standby"로 전달 되는 경우 onEvent의 GO_TO_STANDBY 이벤트 전달
* (FIX) bundle id 체크 로직 수정

## Mobile SDK for Android v2.0.7 (2020-04-17)
* (UPDATE) KWS-S 호출어 라이브러리 beta2 적용
* (UPDATE) 인텐트 답변기능__systemMesg 추가
* (UPDATE) Snd_SVEV에서 type이 "standby"로 전달 되는 경우 onEvent의 GO_TO_STANDBY 이벤트 전달

## Device SDK for Linux/Android/Windows v1.0.3 (2020-04-17)
* (NEW) ARM Cortex-A53 Embedded Linux 지원
* (UPDATE) 공통 : Snd_SVEV에서 type이 "standby"로 전달 되는 경우 onEvent의 GO_TO_STANDBY 이벤트 전달
* (FIX) Android SDK : default 서버 수정(inside.gigagenie.ai)
* (FIX) Android SDK : InfoDetail imageurl 오타 수정

## Device SDK for Linux/Android/Windows v1.0.2 (2020-02-28)
* (UPDATE) 공통 : kws model name 구조변경(0~3.cnsf) 및 default 호출어(지니야) 반영
* (FIX) 공통 : agent_onCommand() 날씨 metaInfo 데이터 현행화, 변경된 stream ID 구분 처리
* (FIX) 공통 : agent_onEvent : Snd_ERR errCode가 200이 아닌 경우에만 Server Error 이벤트 전달
* (FIX) Android SDK : 연속대화시 START_VOICE onCommand 전달하지 않는 오류 수정
* (FIX) Android SDK : media_url 전달 시 playNotiTime Type 변경
* (FIX) Android SDK : Res_STCK 수신후 서버로 Req_STCK 계속 전송하는 오류 수정

## Mobile SDK for iOS v2.0.1beta (2020-01-21)
* (FIX) init() : package name 버그 수정

## Device SDK for Raspberry Pi released (2019-12-28)
* (NEW) Raspbian-armv7l(Raspberry Pi) 지원
  - G-SDK v1.0.1 for Raspbian Linux
  - SEE g-sdk-linux/lib/raspbian-armv7l/

## Mobile SDK for iOS v2.0.0beta (2019-12-05)
* initial public release
* iOS 9.0 or later supported

## Mobile SDK for Android v2.0.6 (2019-12-05)
* (ADD) MEDIA_STREAM_META 추가(TTS의 text 내용을 'mesg'로 확인 가능)
* (ADD) cancelVoiceCmd() API 추가(음성 취소시 서버로 이벤트 전송)
* (ADD) 이벤트 코드 추가 및 변경(onEvent callback)
  - VOICE_STARTED, VOICE_STOPPED, SERVER_ERROR
  - KWS_STARTED, KWS_DETECTED, KWS_ERROR

## Device SDK for Linux/Android/Windows v1.0.1 (2019-11-19)
* (FIX) 서버 요청 시 gRPC 세션 연결 상태 체크 후 필요시 서버 재연결 로직 추가
* (ADD) gRPC 연결 상태 이상/에러 발생 시 gRPC disconnect 및 SERVER_ERROR(GRPC_DISCONNECTED) 이벤트 전달
* (ADD) 서버 응답 timeout 이벤트 추가
  - 음성인식 요청(agent_startVoice) 후 10초 내에 서버 응답 없을 경우, SERVER_ERROR(601) 이벤트 전달
  - VOICE_START 이벤트 수신 후 sendVoice() 음성 데이터 전송 시작 후 10초 내 VOICE_STOP 이벤트가 오지 않을 경우, SERVER_ERROR(602) 이벤트 전달
  - VOICE_STOP 이벤트 수신 후 10초 내 서버 Command(TTS, Media 등)가 없을 경우, SERVER_ERROR(603) 이벤트 전달

## Mobile SDK for Android v2.0.5 (2019-10-31)
* (UPDATE) KWS-G 호출어 라이브러리 v1.0.607 적용
* (FIX) setSeverInfo() 오류 수정

## Device SDK for Linux/Android/Windows v1.0.0 (2019-10-7)
* 기가지니 default 호출어 변경(기가지니->지니야)
* 서비스/컨텐츠에 대한 meta 정보 제공(Developer Guide 38페이지 "9.12.8 metaInfo 상세설명" 참고)
  - metaInfo 지원 컨텐츠 종류: TTS 텍스트, 지니뮤직, 팟캐스트, 라디오, 날씨 등
  - TTS 메시지를 보내는 attribute명 변경(title -> mesg)

## Mobile SDK for Android v2.0.4 (2019-09-09)
* (UPDATE) KWS 라이브러리 20190902T144406 버전 적용 (AndroidX 지원)
* (FIX) 초기화(init) 내부 로직 수정

## Device SDK for Windows v0.9.0 beta (2019-08-21)

### initial beta release
* 기가지니 호출어 라이브러리 KwsRnet v6.0 beta2 적용
* agent_xxx() APIs, kws_xxx() APIs
* Windows 64-bit 지원(Windows 7 이상)

## Device SDK for Linux v0.9.1 beta (2019-08-21)
* (NEW) Embedded Linux on G-INSIDE Reference H/W(Yocto aarch64) 지원
* (NEW) CentOS Linux x86_64 지원
* (NEW) kws_setModelPath() API 추가
* (NEW) Sample client template open source 추가(g-sdk-linux/sample-src/)
* (UPDATE) test-src : 연결서버 정보와 client-key 정보를 파일에서 읽도록 수정(sever_info.txt, key.txt)

## Mobile SDK for Android v2.0.3 (2019-08-21)
* (UPDATE) 호출어 detect 후 음성인식 자동 시작 flow 삭제(KWS detect 후, startVoice 호출해야 음성인식 시작함)
* (ADD) Instant Run은 off로 가이드 추가

## Device SDK for Android v0.9.1 beta (2019-08-01)
* (NEW) 위치 정보 등록 API agent_setLocation() 추가
* (NEW) 테스트서버 연동을 위한 agent_setServerInfo() 추가
* (FIX) client-type 오류 수정
* (FIX) 초기화(init) 내부 로직 수정
* (UPDATE) agent_onCommand()의 dialog_response actionType 추가

## Mobile SDK for Android v2.0.2 (2019-08-01)
* (NEW) 테스트서버 연동을 위한 setServerInfo() 추가
* (UPDATE) KWS 라이브러리 20190730T200947 버전 적용
* (FIX) 초기화(init) 내부 로직 수정

## Device SDK for Linux v0.9.0 beta (2019-07-18)

### initial beta release
* 기가지니 호출어 라이브러리 KwsRnet v6.0 beta3 적용
* agent_xxx() APIs, kws_xxx() APIs
* Ubuntu Linux x86_64 지원

### Added
* agent_setServerInfo() : 테스트 서버 연동을 위한 설정 API

## Device SDK for Android v0.9.0 beta (2019-07-10)

### initial beta release
* 기가지니 호출어 라이브러리 KwsRnet v6.0 beta4 적용
* agent_xxx() 17개 APIs : 기가지니 Cloud AI 플랫폼 연동
* kws_xxx() 7개 APIs : 기가지니 호출어(지니야, 기가지니, 친구야, 자기야) 제공

## Mobile SDK for Android v2.0.0 (2019-06-28)

### New features

* 기가지니 Cloud AI Platform 상용 서버 연동

### Changes

* KWS 라이브러리 20190626T210212 버전 적용(마이크 충돌 시 E_RECOGN_TIMEOUT 대신 E_INTERNAL 전달)
* KWS(기가지니 호출 라이브러리) 2019-06-14일자 버전 적용(특정 상황에서 호출어 False Alarm 현상 개선)

### Fixes

* 음성인식/대화/음성합성 gRPC API 호출 후 connection close 되도록 수정

## Mobile SDK(G-Plugin 2.0) for Android/iOS alpha version (2019-01-31)

* 기가지니 Cloud AI Platform 개발 서버 연동 버전 배포 
