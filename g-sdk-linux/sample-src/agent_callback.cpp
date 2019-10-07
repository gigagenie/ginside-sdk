/*
Copyright (c) 2019 KT corp.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "ginside.h"
#include "ginsidedef.h"
#include "agent_callback.h"
#include "cJSON.h"
#include "base64.h"

extern bool B_KWS;
extern bool B_RECORD;

pthread_t sendVoiceThread;


// for timer
pthread_t timerThread;
int g_set_timer_set = 0;
int g_set_timer = 0;
std::string g_reqAct;
std::string g_actionTrx;

// for TEST //
extern bool TEST;
extern int TEST_CNT;

void forTestMethod() {
	TEST = false;
	TEST_CNT = 0;
}
// for TEST //

void onEvent(int eventMask, std::string opt) {
	switch (eventMask) {
        case INSIDE_EVENT::VOICE_STOP:
        	// TODO : 음성인식이 성공하고, 결과를 내려줄 때, 이 이벤트가 전달된다.
        	// 필요 시 유저에게 어떤 단어가 인식됬는지 알려준다. (recognizedText)
            // 만약 KWS 가 활성화 되어있다면, 호출어를 인식할 수 있게 KWS 를 시작해준다.
            // 음성 녹음을 중단하고, 필요 시 UI 혹은 LED 등으로 녹음 중단을 알린다.
        	printf("onEvent VOICE_STOP CALLED\n");

        	// FLAG 처리
        	// KWS 를 활성화시킨다.
        	B_KWS = true;
        	// 음성 녹음을 중단한다.
        	B_RECORD = false;
        	forTestMethod();
            break;
        case INSIDE_EVENT::VOICE_START:
            // TODO : 음성 시작 요청이 성공하여 음성을 보내야 할 때 해당 이벤트가 온다.
            // 만약 KWS 가 활성화 되어있다면, 호출어 인식을 중단한다.
            // 음성 녹음을 시작하고, agent_sendVoice()로 입력된 데이터 stream을 16kHz Signed 16bit Linear PCM 데이터로 전달
            // 필요 시 UI 혹은 LED 등으로 녹음 시작을 알린다..
        	printf("onEvent VOICE_START CALLED\n");

        	// FLAG 처리
        	// KWS 를 중단한다.
        	B_KWS = false;
        	// 음성 녹음을 활성화한다.
        	B_RECORD = true;
            break;
        case INSIDE_EVENT::SERVER_ERROR:
            // TODO : 음성인식이 특정 오류로 인해 실패했을 때 해당 이벤트가 온다.
            // 에러 종류를 확인하고 필요 시 사용자에게 알린다.
            // VOICE_STOP 과 똑같이 동작하되, 기존 재생중인 음성이 있었다면, 필요 시 다시 재생한다.
        	printf("onEvent SERVER_ERROR CALLED\n");

        	// FLAG 처리
			// KWS 를 활성화시킨다.
			B_KWS = true;
			// 음성 녹음을 중단한다.
			B_RECORD = false;
        	forTestMethod();
        	// 기존에 재생하던 채널이 있었다면, resume 시켜준다.
            break;
        default:
        	// KWS 를 활성화시킨다.
			B_KWS = true;
			// 음성 녹음을 중단한다.
			B_RECORD = false;
        	forTestMethod();
            //printf("onEvent ---> eventMask=%d, opt=%s\n", eventMask, opt.c_str());
            break;
	}
}

void processReqSTTM(std::string cmdPayload) {
	cJSON *cmdp_jsonObj = cJSON_Parse(cmdPayload.c_str());
    if (cmdp_jsonObj != NULL) {
		cJSON *cmdp_payload = cJSON_GetObjectItem(cmdp_jsonObj, "payload");
        if (cmdp_payload != NULL) {
            // TODO : 서버에서 전달 받은 타이머 정보를 내부에 저장한다.
            // 타이머 시간이 도래하면 SDK 에게 알린다.
            // 문서 상 Snd_TMEV 를 참조한다.
        	cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
        	cJSON *reqAct_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "reqAct");
			cJSON *actionTrx_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "actionTrx");
			cJSON *setTime_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "setTime");
			if (reqAct_obj != NULL && actionTrx_obj != NULL && setTime_obj != NULL) {
				std::string reqAct(reqAct_obj->valuestring);
				std::string actionTrx(actionTrx_obj->valuestring);
				std::string setTime(setTime_obj->valuestring);

				// TODO : 위 값을 이용하여 타이머를 세팅한다.
				// 해당 값들은 timerThread 에서 처리된다.
				printf("reqAct=[%s], actionTrx=[%s], setTime=[%s]\n", reqAct.c_str(), actionTrx.c_str(), setTime.c_str());
				g_reqAct = reqAct;
				g_actionTrx = actionTrx;
				g_set_timer = atoi(setTime.c_str());
				g_set_timer_set = 1;
			}
		}
		cJSON_Delete(cmdp_jsonObj);
	}
}

void sendTmEvent(const char *pReqAct, const char *pActionTrx)
{
	char	szLocalTime[20];

	// TODO : 현재 시간을 구해 szLocalTime 에 넣는다.
	// 형식은 다음과 같다 : YYYYMMDDHH24MISS

	cJSON *payload_root = cJSON_CreateObject();
	cJSON *cmdtype = cJSON_CreateString(CMD_SND_TMEV);
	cJSON *cmdOpt_reqAct = cJSON_CreateString(pReqAct);
	cJSON *cmdOpt_actionTrx = cJSON_CreateString(pActionTrx);
	cJSON *cmdOpt_localTime = cJSON_CreateString(szLocalTime);

	cJSON_AddItemToObject(payload_root, "cmdType", cmdtype);
	cJSON_AddItemToObject(payload_root, "reqAct", cmdOpt_reqAct);
	cJSON_AddItemToObject(payload_root, "actionTrx", cmdOpt_actionTrx);
	cJSON_AddItemToObject(payload_root, "localTime", cmdOpt_localTime);

	char *msgPayloadStr = cJSON_Print(payload_root);
	printf("[%s]", msgPayloadStr);
	agent_sendCommand(msgPayloadStr);

	cJSON_Delete(payload_root);
	free(msgPayloadStr);
}

void onCommand(std::string cmd) {
	std::string	msgType, actionType, msgPayload, contentType;
	const char* voiceData;
	int voiceDataSize = 0;
	cJSON *cmdp_jsonObj = cJSON_Parse(cmd.c_str());
	if (cmdp_jsonObj != NULL) {
		cJSON *cmdp_actionType = cJSON_GetObjectItem(cmdp_jsonObj, "actionType");
		cJSON *cmdp_commandType = cJSON_GetObjectItem(cmdp_jsonObj, "commandType");	// commandType or contentType
		cJSON *cmdp_contentType = cJSON_GetObjectItem(cmdp_jsonObj, "contentType");
		cJSON *cmdp_payload = cJSON_GetObjectItem(cmdp_jsonObj, "payload");

		actionType = cmdp_actionType->valuestring;

		msgType = "";
		if (cmdp_commandType != NULL) {
			msgType = cmdp_commandType->valuestring;
		}
		contentType = "";
		if (cmdp_contentType != NULL) {
			contentType = cmdp_contentType->valuestring;
		}

		printf("onCommand -> actionType=[%s], msgType=[%s], contentType=[%s]\n", actionType.c_str(), msgType.c_str(), contentType.c_str());

		if (strcmp(actionType.c_str(), "media_stream") == 0) {
			if (strcmp(contentType.c_str(), "wav") == 0) {
			    // media_stream 일 때 처리
                cJSON *cmdp_mediastream = cJSON_GetObjectItem(cmdp_jsonObj, "mediastream");
                std::string media = cmdp_mediastream->valuestring;
                std::string decoded = base64_decode(media);

                voiceDataSize = (int)decoded.size();
                voiceData = decoded.c_str();

                // TODO : voiceData 를 이용하여 음성을 재생하고, 시작, 끝을 알려야한다.
                // agent_updateMediaStatus 메소드를 이용하여 SDK 에게 음성의 시작과 끝을 알려줘야한다.

                // play -> voiceData
                // 시작 시 -> agent_updateMediaStatus(채널, "started", 0);
                // 종료 시 -> agent_updateMediaStatus(채널, "complete", 음성의 길이);
			} else if (strcmp(contentType.c_str(), "pcm_stream") == 0) {
                // 음성 스트림이 4096바이트 단위로 잘라져서 온다.
                // end 값이 0일 때 시작, 1 진행중, 2 일 때 마지막 데이터이다.
                // 단말에서는 스트림을 실시간으로 재생하여야 한다.
                // 음성을 재생하고, 재생 시 이벤트를 SDK 로 전달해야 한다. 개발규격서의 agent_updateMediaStatus 부분을 확인한다.

                cJSON *cmdp_mediastream = cJSON_GetObjectItem(cmdp_jsonObj, "mediastream");
                // end 값
                cJSON *cmdp_end = cJSON_GetObjectItem(cmdp_jsonObj, "end");

                std::string media = cmdp_mediastream->valuestring;
                std::string decoded = base64_decode(media);
                voiceDataSize = (int)decoded.size();
                voiceData = decoded.c_str();

                if(cmdp_end->valueint == 0) {
                    // end 값이 0 일 때는 음성이 시작이다.
                }
                if(cmdp_end->valueint == 2) {
                    // end 값이 2 일 때는 음성의 마지막 데이터이다.
                }

                // TODO : voiceData 를 이용하여 음성을 재생하고, 시작, 끝을 알려야한다.
				// pcm_stream 일 때는, 음성이 일정 크기로 나눠서 들어오게되고, cmdp_end 값으로 시작과 끝을 알 수 있다.

				// play -> voiceData
				// 시작 시 -> agent_updateMediaStatus(채널, "started", 0);
				// 종료 시 -> agent_updateMediaStatus(채널, "complete", 음성의 길이);
                cJSON_Delete(cmdp_jsonObj);
			}
		} else if (strcmp(actionType.c_str(), "start_voice") == 0) {
            // 재생중인 무언가가 있다면, 일시중지시킨다.
            // 만약 KWS 가 활성화 되어있다면, 호출어 인식을 중단한다.
            // 음성 녹음을 시작하고, agent_sendVoice()로 입력된 데이터 stream을 16kHz Signed 16bit Linear PCM 데이터로 전달
            // 필요 시 UI 혹은 LED 등으로 녹음 시작을 알린다.

			printf("onCommand VOICE_START CALLED\n");

			// TODO : KWS 를 중지하고, 음성녹음을 시작한다.
			// FLAG 처리
			// KWS 를 중단한다.
			B_KWS = false;
			// 음성 녹음을 활성화한다.
			B_RECORD = true;
		} else if (strcmp(actionType.c_str(), "timer_set") == 0) {
            // 아래 메소드 참조
			processReqSTTM(cmd);
		} else if (strcmp(actionType.c_str(), "media_url") == 0) {
			if (cmdp_payload != NULL && strcmp(msgType.c_str(), "Req_PLMD") == 0) {
                // 재생해야 할 채널, url 이 전달된다.
                // 다른 채널에 재생중인 미디어가 있다면, actOnOther 값에 따라 제어한다.
                cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
                if (cmdp_cmdOpt != NULL) {
                    // channel : 재생할 채널
                    cJSON *cmdp_channel = cJSON_GetObjectItem(cmdp_cmdOpt, "channel");
                    // actOnOther : 기존에 재생중이던 미디어를 어떻게 제어할지에 대한 값
                    cJSON *cmdp_actOnOther = cJSON_GetObjectItem(cmdp_cmdOpt, "actOnOther");
                    // 재생해야 할 url
                    cJSON *cmdp_url = cJSON_GetObjectItem(cmdp_cmdOpt, "url");

                    // TODO : 위의 데이터를 가지고 재생을 시도한다.
                    // 시작 시 -> agent_updateMediaStatus(채널, "started", 0);
                }
			} else if (cmdp_payload != NULL && strcmp(msgType.c_str(), "Req_UPMD") == 0) {
                // 제어해야 할 채널, 동작 등의 값이 전달된다.
                // 예를들어 channel=1, act="stop" 이라면, 1번 채널에 재생중인 미디어를 중지시킨다.
                cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
                if (cmdp_cmdOpt != NULL) {
                    // 제어할 채널
                    cJSON *cmdp_channel = cJSON_GetObjectItem(cmdp_cmdOpt, "channel");
                    // 어떻게 제어할지
                    cJSON *cmdp_act = cJSON_GetObjectItem(cmdp_cmdOpt, "act");
                    // 플레이시간
                    cJSON *cmdp_playtime = cJSON_GetObjectItem(cmdp_cmdOpt, "playtime");
                    // 값이 존재하면 해당 dssStatus 세팅
                    cJSON *cmdp_setDssStatus = cJSON_GetObjectItem(cmdp_cmdOpt, "setDssStatus");
                    // 값이 존재하면 해당 dssStatus 해제
                    cJSON *cmdp_cleartime = cJSON_GetObjectItem(cmdp_cmdOpt, "clearDssStatus");

                    // TODO : 위의 값을 가지고 처리한다.
                    // channel = 1, act = pause 값이 들어왔다면.
                    // 1번 채널의 음성을 pause 시키고 아래 메소드를 호출한다.
                    // agent_updateMediaStatus(1, "paused", 현재시);
                }
			}
		} else if (strcmp(actionType.c_str(), "dialog_response") == 0) {
            // execType, execOpt 값에 따라 처리한다.
            // 자세한 사항은 개발규격서를 참조한다.
		} else if (strcmp(actionType.c_str(), "hardware_control") == 0) {
			if (cmdp_payload != NULL && strcmp(msgType.c_str(), "Req_HWCL") == 0) {
				cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
				if (cmdp_cmdOpt != NULL) {
                    // 제어해야 할 대상과, 값이 전달된다.
                    // 블루투스 제어와 볼륨 컨트롤이 존재한다.
					cJSON *cmdp_target = cJSON_GetObjectItem(cmdp_cmdOpt, "target");
					cJSON *cmdp_hwCmd = cJSON_GetObjectItem(cmdp_cmdOpt, "hwCmd");
					cJSON *cmdp_hwCmdOpt = cJSON_GetObjectItem(cmdp_cmdOpt, "hwCmdOpt");
					if (strcmp(cmdp_target->valuestring, "volume") == 0 && strcmp(cmdp_hwCmd->valuestring, "setVolume") == 0 && cmdp_hwCmdOpt != NULL) {
						// 볼륨을 조절한다.
                        cJSON *cmdp_control = cJSON_GetObjectItem(cmdp_hwCmdOpt, "control");
                        cJSON *cmdp_value = cJSON_GetObjectItem(cmdp_hwCmdOpt, "value");

                        // TODO : control, value 값에 따라 제어한다.
                        // if (strcmp(cmdp_control->valuestring, "UP") == 0)
                        //		이 경우 단말의 볼륨을 UP 시킨다.
                        // else
                        // 		이 경우 단말의 볼륨을 DOWN 시킨다.
					} else {
                        // TODO : 블루투스 제어를 한다.
                        // 자세한 내용은 개발규격서를 참조한다.
					}
				}
			}
		} else if (strcmp(actionType.c_str(), "webview_url") == 0) {
			if (strcmp(msgType.c_str(), "Req_OAuth") == 0 && cmdp_payload != NULL) {
                // 실행해야 할 web url 이 전달된다.
                cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
                if (cmdp_cmdOpt != NULL) {
                    cJSON *cmdp_oauth_url = cJSON_GetObjectItem(cmdp_cmdOpt, "oauth_url");

                    // TODO: 해당 값이 온다면, 지니뮤직 로그인이 필요한 상태이다.
                    // 브라우저 혹은 페어링 된 기가지니 앱 등을 이용하여 지니뮤직에 로그인 하도록 유도한다.
                }
			}
		}
	}
	cJSON_Delete(cmdp_jsonObj);
}

void* sendVoice(void* param) {
	int ret;
	int size = 256;
	short testData[size];

	while(true) {
		if(B_RECORD) {
			// TODO: ALSA 라이브러리 등을 이용하여 음성데이터를 입력받는다. testData 음성데이터가 있다고 가정한다.
			// 입력받은 음성데이터를 agent_sendVoice 로 넘겨준다.
			agent_sendVoice((short *) testData, size);
			usleep(1000*10);
		} else {
			// 음성녹음 활성화 중이 아닐 때
			usleep(1000*10);
		}
	}
}
void* timer(void* param) {
	int	i, sec;

	while(true) {
		if (g_set_timer_set > 0) {
			for (i = 0; i < g_set_timer; i++) {
				sleep(1000);	// 1 sec sleep
			}
			sendTmEvent(g_reqAct.c_str(), g_actionTrx.c_str());
			g_set_timer_set = 0;
		}
	}
}

void AgentCallback::init() {
	// 음성녹음을 위한 스레드를 생성
	pthread_create(&sendVoiceThread, NULL, &sendVoice, NULL);

	// 타이머 이벤트를 위한 스레드를 생성
	pthread_create(&sendVoiceThread, NULL, &timer, NULL);

	// TODO : 그 외 기타 필요한 것들을 여기서 세팅한다.
}
