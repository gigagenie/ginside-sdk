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
// ginsidewin-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef  LINUX
#include  "windows.h"
#else
#include  "stdio.h"
#include  "string.h"
#include  "unistd.h"
#endif

#include  "test_api.h"
#include  "agent_callback.h"
#include  "base64.h"
#include  "cJSON.h"

void usage()
{
	puts("Main menu:::");
	printf("h) help               d) agent_debugmode\n");
	printf("1) agent_register     2) agent_init         3) agent_unregister\n");
	printf("4) agent_reset        5) agent_sendText     6) agent_sendCommand\n");
	printf("7) agent_startVoice   8) agent_stopVoice    9) agent_getTTS\n");
	printf("0) agent_serviceLogin a) dialog test        b) kws_init\n");
	printf("c) kws_detect         e) kws_setKeyword     f) agent_setServerInfo\n");
	printf("g) agent_getVersion   i) agent_setLocation  j) agent_setModelPath\n");
	printf("k) agent_serviceLoginStatus                 l) agent_serviceLogout\n"); // v1.1.0
	printf("x, q) Exit this program!\n");
	puts("");
}

int main()
{
	int		loop_exit;
	char	ch[255];
	FILE	*fp;
	char	szUuid[255];
	THREAD_HANDLE	hTimer, hMic;
	REGISTER_CODE	rc;
	std::string CLIENT_ID;
	std::string CLIENT_KEY;
	std::string CLIENT_SECRET;

	int		count, length;
	char	szTemp[255];
	fp = fopen("./conf/key.txt", "rt");
	if (fp != NULL)
	{
		count = 0;
		while(!feof(fp))
		{
			memset(szTemp, 0x0, sizeof(szTemp));
			fgets(szTemp, sizeof(szTemp), fp);
			length = strlen(szTemp);
			if (szTemp[length - 1] == '\n') szTemp[length - 1] = 0x0;
			switch (count)
			{
			case 0:
				CLIENT_ID = szTemp;
				break;
			case 1:
				CLIENT_KEY = szTemp;
				break;
			case 2:
				CLIENT_SECRET = szTemp;
				break;
			}
			count++;
		}
		fclose(fp);
	}
	else
	{
		printf("Key file (key.txt) does not exist. so exit!\n");
		return 0;
	}
	printf("%s\n", CLIENT_ID.c_str());
	printf("%s\n", CLIENT_KEY.c_str());
	printf("%s\n", CLIENT_SECRET.c_str());

	hTimer = SpawnThread(TimerThread, NULL);
	hMic = SpawnThread(MicThread, NULL);

	rc.rc = 0;
	rc.uuid = "";
	rc.rcmsg = "";

	std::string gettts_result;
	int voiceDataSize = 0;
	const char* voiceData;
	cJSON *cmdp_jsonObj;

	usage();
	loop_exit = 0;
	while (!loop_exit)
	{
		printf("\nChoose a menu >>>>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0])
		{
		case '?':
		case 'h':
		case 'H':
			usage();
			break;
		case 'd':
		case 'D':
			test_debugmode();
			break;
		case '1':
			rc = test_register(CLIENT_ID, CLIENT_KEY, CLIENT_SECRET);
			if (rc.rc == 200)
			{
				printf("Successfully registered and uuid=[%s]\n", rc.uuid.c_str());
				fp = fopen("uuid.txt", "wt");
				if (fp != NULL)
				{
					fwrite(rc.uuid.c_str(), 1, rc.uuid.length(), fp);
					fclose(fp);
				}
			}
			else
			{
				rc.uuid = "";
				rc.rcmsg = "";
			}
			break;
		case '2':
			memset(szUuid, 0x0, sizeof(szUuid));
			fp = fopen("uuid.txt", "rt");
			if (fp != NULL)
			{
				fread(szUuid, 1, sizeof(szUuid), fp);
				fclose(fp);
			}

			if (rc.uuid.length() > 0)
			{
				test_init(CLIENT_ID, CLIENT_KEY, CLIENT_SECRET, (char*) rc.uuid.c_str());
			}
			else
			{
				if (szUuid[0] != 0x0)
				{
					test_init(CLIENT_ID, CLIENT_KEY, CLIENT_SECRET, szUuid);
				}
				else
				{
					printf("Call agent_register() first!\n");
				}
			}
			break;
		case '3':
			test_unregister();
			break;
		case '4':
			test_reset();
			break;
		case '5':
			test_sendText();
			break;
		case '6':
			test_sendCommand();
			break;
		case '7':
			test_startVoice();
			break;
		case '8':
			test_stopVoice();
			break;
		case '9':
		    gettts_result = test_getTTS();
		    cmdp_jsonObj = cJSON_Parse(gettts_result.c_str());
		    if (cmdp_jsonObj != NULL) {
		        cJSON *rc = cJSON_GetObjectItem(cmdp_jsonObj, "rc");
		        cJSON *rcmsg = cJSON_GetObjectItem(cmdp_jsonObj, "rcmsg");
		        if(rc != NULL && rcmsg != NULL) {
		            if(rc->valueint == 200) {
		                std::string media(rcmsg->valuestring);
		                std::string decoded = base64_decode(media);             
		                printf("Successful getTTS and media size=[%d]\n", decoded.size());
                        
		                voiceDataSize = (int)decoded.size();
		                voiceData = decoded.c_str();
		                printf("Writing data to a file with a size, %d\n", voiceDataSize);

		                fp = fopen("getttswav", "w+b");
		                if (fp != NULL) {
		                    fwrite(voiceData, 1, voiceDataSize, fp);
		                    fclose(fp);
		                    fp = NULL;
		                }
		            }
		        }
		        cJSON_Delete(cmdp_jsonObj);
		    }
			break;
		case '0':
			test_serviceLogin();
			break;
		case 'a':
		case 'A':
			test_dialog();
			break;
		case 'b':
		case 'B':
			test_kws_init();
			break;
		case 'c':
		case 'C':
			test_kws_detect();
			break;
		case 'e':
		case 'E':
			test_kws_setKeyword();
			break;
		case 'f':
		case 'F':
			test_setServerInfo();
			break;
		case 'g':
		case 'G':
			test_getVersion();
			break;
		case 'i':
		case 'I':
			test_setLocation();
			break;
		case 'j':
		case 'J':
			test_setModelPath();
			break;
		case 'k':
		case 'K':
		    test_serviceLoginStatus();
		    break;
		case 'l':
		case 'L':
		    test_serviceLogout();
		    break;
		case 'x':
		case 'X':
		case 'q':
		case 'Q':
			loop_exit = 1;
			break;
		default:
			printf("!!! Invalid option !!!\n");
			break;
		}
	}


#ifdef  LINUX
	pthread_cancel(hTimer);
	pthread_cancel(hMic);
#endif

	printf("Exit!\n");
}
