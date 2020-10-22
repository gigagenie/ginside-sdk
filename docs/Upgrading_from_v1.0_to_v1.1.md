### G-INSIDE Device SDK - Upgrading from v1.0.x to v1.1.x(or later)

>  **NOTICE** : Device SDK v1.0.x를 이용하여 만들어진 Application에서 SDK v1.0.x 대신 SDK v1.1.x 이상 버전을 이용하기 위해서는 아래 변경사항에 대해서 반드시 수정해야 합니다.



#### Summary

1. agent_onCommand()의 input parameter 변경
2. agent_onCommand()의 actionType value 변경
3. 음성인식 시작(Mic ON 후 sendVoice)
4. 음성인식 종료(Mic OFF, 결과 Text 확인)
5. TTS 재생 (wav 파일로 재생할 경우)
6. Media URL 재생
7. 재생 중인 Media 제어
8. 단말 특화 서비스(Dialog Kit)
9. agent_getTTS() 사용 방법 변경
10. 서비스 로그인 관련 신규 API 추가



#### 1. agent_onCommand()의 input parameter 변경

**v1.0.x** | **v1.1.x**
------------ | -------------
agent_onCommand(string **cmd**) | agent_onCommand(string **actionType**, string **payload**) 

> v1.0.x에서 **cmd** JSON에 포함되어 있던 **actionType**과 **payload**를 별도 parameter로 분리

* **actionType** : 명령 유형 string
* **payload** :  각 actionType의 부가 정보를 담은 JSON string
* 예시
  > v.1.0.x
  ```
  cmd = {
  	"actionType" : "media_stream",
  	"payload" : { "cmdOpt" : { ... }}
  ```
  > **v1.1.x**
  ```
  actionType = "play_media"
  payload = { "cmdOpt" : { ... }}
  ```



#### 2. agent_onCommand()의 actionType value 변경

> agent_onCommand()에서 사용하는 actionType은 각각 아래와 같이 변경되었다

| **v1.0.x**   | **v1.1.x** |
| ------------ | ---------- |
| media_stream | **play_media** |
| media_url | **play_media** |
| media_stream(contentType: wav) | **media_data** |
| dialog_response | **exec_dialogkit** |
| media_url(commandType: Req_UPMD) | **control_media** |
| hardware_control | **control_hardware** |
| timer_set | **set_timer** |
| webview_url | webview_url |



#### 3. 음성인식 시작(Mic ON 후 sendVoice)

**v1.0.x** | **v1.1.x**
------------ | -------------
**agent_onEvent()** callback의 **evt : VOICE_START** 이벤트로 처리 | **VOICE_START** 이벤트 삭제<br>**agent_onCommand()** 의 **actionType : start_voice**로 변경 

* 예시(pseudo-code)
  > **v1.1.x**
  ```
  agent_onCommand(actionType, payload) {
  	if actionType == "start_voice":
  		Mic-ON & agent_sendVoice()
  }
  ```
  



#### 4. 음성인식 종료(Mic OFF, 결과 Text 확인)

**v1.0.x** | **v1.1.x**
------------ | -------------
**agent_onEvent()** callback의 **evt : VOICE_STOP** 이벤트로 처리 | **VOICE_STOP** 이벤트 삭제<br>**agent_onCommand()** 의 **actionType : stop_voice**로 변경 

* 예시(pseudo-code)
  > **v1.1.x**
  ```
  agent_onCommand(actionType, payload) {
  	if actionType == "stop_voice":
  		Mic-OFF
  }
  ```



#### 5. TTS 재생 (wav 파일로 재생할 경우)

**v1.0.x** | **v1.1.x**
------------ | -------------
Step1: agent_onCommand()에서 cmd의 actionType = **media_stream**으로 TTS Text 확인<br><br>Step2: agent_onCommand()에서 cmd의 actionType = **media_stream**, contentType = **wav**인 데이터 전달받아 재생 | Step1: agent_onCommand()에서 actionType = **play_media** 로 TTS Text 확인<br><br>Step2: agent_onCommand()에서 actionType = **media_data** 로 전달되는 TTS wav 데이터 전달받아 재생 



#### 6. Media URL 재생

**v1.0.x** | **v1.1.x**
------------ | -------------
agent_onCommand()에서 cmd의 actionType = **media_url**로 전달되는 미디어 url 확인 & 재생 | agent_onCommand()에서 actionType = **play_media**로 전달되는 미디어 url 확인 & 재생 



#### 7. 재생 중인 Media 제어

| **v1.0.x**                                                   | **v1.1.x**                                                   |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| agent_onCommand()에서 actionType = **media_url**, commandType = **Req_UPMD** 로 전달되는 cmdOpt에서 미디어 제어 정보 확인 | agent_onCommand()에서 actionType = **control_media**로 미디어 제어 정보 확인하여 재생 중인 미디어 제어 |



#### 8. 단말 특화 서비스(Dialog Kit)

**v1.0.x** | **v1.1.x**
------------ | -------------
agent_onCommand(string cmd)에서 cmd 내 actionType = **dialog_response** 일 때 JSON의 **dialogResponse** 정보를 사용하여 단말 특화 서비스 구현 | agent_onCommand(string actionType, string payload)에서 actionType = **exec_dialogkit** 일 때 payload JSON의 **dssMsg** 정보를 사용하여 단말 특화 서비스 구현 

* 예시
  > v.1.0.x
  ```
  cmd = {
  	"actionType" : "dialog_response",
  	"commandType" : "Req_EXEC",
  	"dialogResponse" : {
  		"Intent" : "lightTurnOff",
  		"appInfo" : { "NE-LIGHT" : "전등"},
  		"__systemMesg" : "전등을 껐습니다"
  	}
  }
  ```
  > **v1.1.x**
  ```
  actionType = "exec_dialogkit"
  payload = {}
  	"cmdOpt" : {
  		"execType" : "dialogHandler",
  		"execOpt" : {
  			"action" : "kt.action.dialog.custom",
  			"dssMsg" : {
  				"Intent" : "lightTurnOff",
  				"appInfo" : { "NE-LIGHT" : "전등"},
  				"__systemMesg" : "전등을 껐습니다"		
  			}
  		}
  	}
  }
  ```



#### 9. agent_getTTS() 사용 방법 변경

**v1.0.x** | **v1.1.x**
------------ | -------------
**void** agent_getTTS(string msg) | **string** agent_getTTS(string msg) 

> **agent_getTTS()** 의 return이 void에서 **string** 타입으로 변경
>
> v1.0.x에서는 **agent_getTTS()** 후 **agent_onCommand()** callback의 actionType = media_stream을 통해 wav 데이터 전달
>
> v1.1.x에서는 **agent_getTTS()** 의 return 값(string)으로 wav 데이터의 base64 encoded string 전달

* 예시(pseudo-code)

  > v.1.0.x
  ```
  agent_getTTS("안녕");
  
  void agent_onCommand(cmdJSON) {
  	if cmdJSON.actionType == "media_stream" {
  		// get TTS WAV data
  	}
  }
  ```
  > **v1.1.x**
  ```
  wav_data = agent_getTTS("안녕");
  ```



#### 10. 서비스 로그인 관련 신규 API 추가

v1.1.x 신규 API | 설명 
------------ | -------------
agent_serviceLoginStatus | 서비스 로그인 확인 (OAuth access_token 유효성 확인) 
agent_serviceLogout | 서비스 로그아웃 (OAuth 인증정보 삭제)



