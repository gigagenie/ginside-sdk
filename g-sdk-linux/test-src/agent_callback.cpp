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
#include "sys/time.h"
#include <string>
#include "agent_callback.h"
#include "cJSON.h"
#include "ginside.h"
#include "ginsidedef.h"
#include "base64.h"

#ifdef  LINUX
#define  SLEEP_MS(x)	usleep((x)*1000)
#define  strcmp			strcasecmp
#else
#define  SLEEP_MS(x)	Sleep(x)
#endif


int g_tts_stream_play_set = 0;
int g_tts_stream_playtime = 0;
int g_tts_stream_org_playtime = 0;
int g_tts_play_set = 0;
int g_tts_playtime = 0;
int g_set_timer_set = 0;
int g_set_timer = 0;
std::string g_reqAct;
std::string g_actionTrx;

void getfilename(char *pfilename)
{
#ifdef LINUX
	struct timeval		tv;
	struct timezone		tz;
	struct tm			*tptr, t;

	gettimeofday(&tv, &tz);
	localtime_r(&tv.tv_sec, &t);
	tptr = &t;

	sprintf(pfilename, "%02d%02d_%02d%02d%02d",
		tptr->tm_mon + 1,
		tptr->tm_mday,
		tptr->tm_hour,
		tptr->tm_min,
		tptr->tm_sec);
#else
	SYSTEMTIME	st;

	GetLocalTime(&st);
	sprintf(pfilename, "%02d%02d_%02d%02d%02d",
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
#endif
}

void get_localtime(char *pTimestr)
{
#ifdef LINUX
	struct timeval		tv;
	struct timezone		tz;
	struct tm			*tptr, t;

	gettimeofday(&tv, &tz);
	localtime_r(&tv.tv_sec, &t);
	tptr = &t;

	sprintf(pTimestr, "%4d%02d%02d%02d%02d%02d",
		1900 + tptr->tm_year,
		tptr->tm_mon + 1,
		tptr->tm_mday,
		tptr->tm_hour,
		tptr->tm_min,
		tptr->tm_sec);
#else
	SYSTEMTIME	st;

	GetLocalTime(&st);
	sprintf(pTimestr, "%4d%02d%02d%02d%02d%02d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
#endif
}

void sendTmEvent(const char *pReqAct, const char *pActionTrx)
{
	char	szLocalTime[20];

	get_localtime(szLocalTime);
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

THREAD_RET_TYPE THREAD_CALLING_CONVENTION TimerThread(void *arg)
{
	int	i, sec;

	while (true)
	{
		SLEEP_MS(1000);	// 1 sec sleep
		if (g_tts_play_set > 0)
		{
			sec = g_tts_playtime / 1000;
			for (i = 0; i < sec; i++)
			{
				SLEEP_MS(1000);	// 1 sec sleep
			}
			agent_updateMediaStatus(0, "complete", g_tts_playtime);
			g_tts_play_set = 0;	// not exact....
		}
		if (g_tts_stream_play_set > 0)
		{
			sec = g_tts_stream_playtime / 1000;
			for (i = 0; i < sec; i++)
			{
				SLEEP_MS(1000);	// 1 sec sleep
			}
			agent_updateMediaStatus(0, "complete", g_tts_stream_org_playtime);
			g_tts_stream_play_set = 0;	// not exact....
		}
		if (g_set_timer_set > 0)
		{
			for (i = 0; i < g_set_timer; i++)
			{
				SLEEP_MS(1000);	// 1 sec sleep
			}
			sendTmEvent(g_reqAct.c_str(), g_actionTrx.c_str());
			g_set_timer_set = 0;
		}
	}

	THREAD_RETURN;
}

char	g_szFilename[255];
char	g_szDialogFilename1[255], g_szDialogFilename2[255];
int		g_start_mic = 0;
int		g_start_dialog = 0, g_dialog_count;


THREAD_RET_TYPE THREAD_CALLING_CONVENTION MicThread(void *arg)
{
	FILE	*fp;
	size_t	nread;
	short	buf[256];

	while (true)
	{
		// Wait for an event.
		while (g_start_mic == 0)
		{
			SLEEP_MS(10);	// 10 msec sleep
		}

		printf("Now calling agent_sendVoice() and reading a file, %s\n", g_szFilename);
		if (g_start_dialog == 0)
		{
			fp = fopen(g_szFilename, "rb");
			if (fp != NULL)
			{
				while (!feof(fp))
				{
					nread = fread(buf, sizeof(short), 256, fp);
					agent_sendVoice(buf, (int)nread);
					SLEEP_MS(15);	// 15 msec sleep
					if (g_start_mic == 0)
					{
						printf("Mic Stop Request has been received!. so stop to record a Mic!\n");
						break;
					}
				}
				fclose(fp);
				printf("Finished to record a MIC, %s\n", g_szFilename);
			}
			else
			{
				printf("MIC Error: CANNOT read a file, %s\n", g_szFilename);
			}
			g_start_mic = 0;
		}
		else
		{
			if (g_dialog_count == 0)
			{
				fp = fopen(g_szDialogFilename1, "rb");
				if (fp != NULL)
				{
					while (!feof(fp))
					{
						nread = fread(buf, sizeof(short), 256, fp);
						agent_sendVoice(buf, (int)nread);
						SLEEP_MS(15);	// 15 msec sleep
						if (g_start_mic == 0)
						{
							printf("Mic Stop Request has been received!. so stop to record a Mic!\n");
							break;
						}
					}
					fclose(fp);
					printf("Finished to record a MIC, %s\n", g_szDialogFilename1);
				}
				else
				{
					printf("MIC Error: CANNOT read a file, %s\n", g_szDialogFilename1);
				}
				g_dialog_count++;
			}
			else
			{
				fp = fopen(g_szDialogFilename2, "rb");
				if (fp != NULL)
				{
					while (!feof(fp))
					{
						nread = fread(buf, sizeof(short), 256, fp);
						agent_sendVoice(buf, (int)nread);
						SLEEP_MS(15);	// 15 msec sleep
						if (g_start_mic == 0)
						{
							printf("Mic Stop Request has been received!. so stop to record a Mic!\n");
							break;
						}
					}
					fclose(fp);
					printf("Finished to record a MIC, %s\n", g_szDialogFilename2);
				}
				else
				{
					printf("MIC Error: CANNOT read a file, %s\n", g_szDialogFilename2);
				}
				g_start_dialog = 0;
				g_dialog_count = 0;
			}
			g_start_mic = 0;
		}
	}

	THREAD_RETURN;
}

#ifdef LINUX
unsigned int timediff(struct timeval startTime, struct timeval endTime)
{
	return (endTime.tv_sec - startTime.tv_sec) * 1000 + (int)((endTime.tv_usec - startTime.tv_usec) / 1000.);
}
#endif


void onEvent(int eventMask, std::string opt) {
	switch (eventMask) {
//		case INSIDE_EVENT::VOICE_STOP:
//			printf("onEvent ---> VOICE_STOP (%s) received. Stop to record a mic!\n", opt.c_str());
//			if (g_start_mic == 1) {
//				printf("Stop request to record a mic!\n");
//				g_start_mic = 0;
//			} else {
//				printf("Recording a mic is already stopped!\n");
//			}
//			break;
//		case INSIDE_EVENT::VOICE_START:
//			printf("onEvent ---> VOICE_START -- Start to record a mic\n");
//			g_start_mic = 1;
//			break;
		case INSIDE_EVENT::SERVER_ERROR:
			printf("onEvent ---> SERVER_ERROR (%s) received.\n", opt.c_str());
			break;
		case INSIDE_EVENT::GRPC_INIT_SUCCESS:
			printf("onEvent ---> GRPC_INIT_SUCCESS received.\n");
			break;
		case INSIDE_EVENT::GRPC_INIT_FAIL:
			printf("onEvent ---> GRPC_INIT_FAIL received.\n");
			break;
		case INSIDE_EVENT::GRPC_DISCONNECTED:
			printf("onEvent ---> GRPC_DISCONNECTED received. opt : %s\n", opt.c_str());
			break;
		default:
			printf("onEvent ---> eventMask=%d, opt=%s\n", eventMask, opt.c_str());
			break;
	}
}

void processReqSTTM(std::string cmdPayload)
{
	printf("%s\n", __FUNCTION__);

	cJSON *cmdp_jsonObj = cJSON_Parse(cmdPayload.c_str());
	if (cmdp_jsonObj == NULL)
	{
		printf("cjson parsing Error");
	}
	else
	{
		cJSON *cmdp_payload = cJSON_GetObjectItem(cmdp_jsonObj, "payload");
		if (cmdp_payload != NULL)
		{
			printf("found payload from the Req_STTM\n");
			cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
			if (cmdp_cmdOpt != NULL)
			{
				cJSON *reqAct_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "reqAct");
				cJSON *actionTrx_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "actionTrx");
				cJSON *setTime_obj = cJSON_GetObjectItem(cmdp_cmdOpt, "setTime");
				if (reqAct_obj != NULL && actionTrx_obj != NULL && setTime_obj != NULL)
				{
					std::string reqAct(reqAct_obj->valuestring);
					std::string actionTrx(actionTrx_obj->valuestring);
					std::string setTime(setTime_obj->valuestring);
					printf("reqAct=[%s], actionTrx=[%s], setTime=[%s]\n", reqAct.c_str(), actionTrx.c_str(), setTime.c_str());
					g_reqAct = reqAct;
					g_actionTrx = actionTrx;
					g_set_timer = atoi(setTime.c_str());
					g_set_timer_set = 1;
				}
			}
			else
			{
				printf("No cmdOpt in the input\n");
			}
		}
		else
		{
			printf("No payload in the input\n");
		}
		cJSON_Delete(cmdp_jsonObj);
	}
}

void onCommand(std::string actionType, std::string payload)
{
	int voiceDataSize = 0;
	const char* voiceData;
	static char filename[255], filename_ext[255];
	static FILE	*fp = NULL;
#ifndef LINUX
	char inbuf[40960], outbuf[40960];
#endif

#ifdef LINUX
	static struct timeval	startTime, endTime;
#endif

	printf("onCommand -> actionType=[%s]\n", actionType.c_str());
	if (strcmp(actionType.c_str(), "media_data") == 0)
	{
	    std::string media = payload;
	    std::string decoded = base64_decode(media);
	    printf("media size=[%d]\n", decoded.size());
	    voiceDataSize = (int)decoded.size();
	    voiceData = decoded.c_str();

	    getfilename(filename);
	    sprintf(filename_ext, "%s.wav", filename);

	    printf("Writing data to a file, %s with a size, %d\n", filename_ext, voiceDataSize);

	    fp = fopen(filename_ext, "w+b");
	    if (fp != NULL)
	    {
	        fwrite(voiceData, 1, voiceDataSize, fp);
	        fclose(fp);
	        fp = NULL;
	    }
	    memset(filename, 0x0, sizeof(filename));
	    memset(filename_ext, 0x0, sizeof(filename_ext));

	    // send request about UPD_MEST: started, complete
	    g_tts_play_set = 1;
	    g_tts_playtime = voiceDataSize / 32;	// ignore the headersize - 16 * 1000 * 2 <-- why  32?
	    agent_updateMediaStatus(0, "started", 0);
	    return;
	}
	else if (strcmp(actionType.c_str(), "start_voice") == 0)
	{
	    printf("received action type=%s, so calling agent_startVoice().\n", "start_voice");
	    g_start_mic = 1;
	    //agent_startVoice();
	}
	else if(strcmp(actionType.c_str(), "stop_voice") == 0)
	{
        if (g_start_mic == 1) {
            printf("Stop request to record a mic!\n");
            g_start_mic = 0;
        } else {
            printf("Recording a mic is already stopped!\n");
        }
	}
	else if (strcmp(actionType.c_str(), "set_timer") == 0)
	{
	    processReqSTTM(payload);
	}
	else if (strcmp(actionType.c_str(), "play_media") == 0)
	{
	    cJSON *cmdp_payload = cJSON_Parse(payload.c_str());
	    if (cmdp_payload == NULL)
	    {
	        printf("play_media parsing Error\n");
	    }
	    else
	    {
	        cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
	        if (cmdp_cmdOpt != NULL)
	        {
	            cJSON *cmdp_channel = cJSON_GetObjectItem(cmdp_cmdOpt, "channel");
	            cJSON *cmdp_actOnOther = cJSON_GetObjectItem(cmdp_cmdOpt, "actOnOther");
	            cJSON *cmdp_url = cJSON_GetObjectItem(cmdp_cmdOpt, "url");
	            if(cmdp_url != NULL) {
	                printf("media_url, Req_PLMD, payload: {channel=%d,actOnOther=%s,url=%s}\n",
	                        cmdp_channel->valueint, cmdp_actOnOther->valuestring, cmdp_url->valuestring);
	            } else {
	                printf("media_url, Req_PLMD, payload: {channel=%d,actOnOther=%s,url=%s}\n",
	                        cmdp_channel->valueint, cmdp_actOnOther->valuestring, "");
	            }    
	        }
	    }
	    cJSON_Delete(cmdp_payload);
	}		
	else if (strcmp(actionType.c_str(), "exec_dialogkit") == 0)
	{
	    printf("exec_dialogkit, dialogResponse = %s\n", payload.c_str());
	}
	else if (strcmp(actionType.c_str(), "control_hardware") == 0)
	{
	    cJSON *cmdp_payload = cJSON_Parse(payload.c_str());
	    if (cmdp_payload == NULL)
	    {
	        printf("control_hardware parsing Error\n");
	    }
	    else
	    {
	        cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
	        if (cmdp_cmdOpt != NULL)
	        {
	            cJSON *cmdp_target = cJSON_GetObjectItem(cmdp_cmdOpt, "target");
	            cJSON *cmdp_hwCmd = cJSON_GetObjectItem(cmdp_cmdOpt, "hwCmd");
	            cJSON *cmdp_hwCmdOpt = cJSON_GetObjectItem(cmdp_cmdOpt, "hwCmdOpt");
	            if (strcmp(cmdp_target->valuestring, "volume") == 0 && strcmp(cmdp_hwCmd->valuestring, "setVolume") == 0 && cmdp_hwCmdOpt != NULL)
	            {
	                cJSON *cmdp_control = cJSON_GetObjectItem(cmdp_hwCmdOpt, "control");
	                cJSON *cmdp_value = cJSON_GetObjectItem(cmdp_hwCmdOpt, "value");

	                printf("hardware_control, Req_HWCL, volume, setVolme:(%s, %s)\n", cmdp_control->valuestring, cmdp_value->valuestring);

	                if (strcmp(cmdp_control->valuestring, "UP") == 0)
	                {
	                    printf("volume up!\n");
	                }
	                else
	                {
	                    printf("volume down!\n");
	                }
	            }
	            else
	            {
	                printf("target=%s, cmd=%s\n", cmdp_target->valuestring, cmdp_hwCmd->valuestring);
	            }
	        }
	    }
	    cJSON_Delete(cmdp_payload);
	}
	else if (strcmp(actionType.c_str(), "webview_url") == 0)
	{
	    cJSON *cmdp_payload = cJSON_Parse(payload.c_str());
	    if (cmdp_payload == NULL)
	    {
	        printf("webview_url parsing Error\n");
	    }
	    else
	    {
	        cJSON *cmdp_cmdOpt = cJSON_GetObjectItem(cmdp_payload, "cmdOpt");
	        if (cmdp_cmdOpt != NULL)
	        {
	            cJSON *cmdp_oauth_url = cJSON_GetObjectItem(cmdp_cmdOpt, "oauth_url");
	            if(cmdp_oauth_url != NULL) {
	                printf("webview_url, Req_OAuth, payload: {oauth_url=%s}\n", cmdp_oauth_url->valuestring);
	            }
	        }
	    }
	    cJSON_Delete(cmdp_payload);
	}
	else
	{
	    printf("test application onCommand() Callback: wrong actionType=[%s]\n", actionType.c_str());
	    if (actionType == "Res_TXCM")
	    {
	        printf("Res_TXCM, msgPayload=[%s]\n", payload.c_str());
	    }
	}
}
