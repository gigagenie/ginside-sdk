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

#ifndef  LINUX
#include  "windows.h"
#else
#include  "stdio.h"
#include  "string.h"
#include  "strings.h"
#include  "unistd.h"
#include  "sys/time.h"
#include  "time.h"
#endif

#include  "stdlib.h"
#include  "cJSON.h"
#include  "test_api.h"

#include  "agent_callback.h"



extern char	g_szFilename[255];
extern char	g_szDialogFilename1[255], g_szDialogFilename2[255];
extern int	g_start_dialog, g_dialog_count;

bool isInit = false;
bool isKwsInit = false;

std::string g_clientId = "";
std::string g_clientKey = "";
std::string g_clientSecret = "";

THREAD_HANDLE SpawnThread(THREAD_RET_TYPE(THREAD_CALLING_CONVENTION *func)(void *), void *arg)
{
	THREAD_HANDLE	handle;

#ifdef LINUX
	if (pthread_create(&handle, NULL, func, arg) != 0)
	{
		perror("pthread_create returned an error!");
	}
#else
	unsigned	tid;
	if ((handle = (HANDLE)_beginthreadex(NULL, 0, func, arg, 0, &tid)) == INVALID_HANDLE_VALUE)
	{
		printf("_beginthreadex returned an error!\n");
	}
#endif

	return handle;
}

void sendHwEvent(std::string target, std::string event, std::string opt)
{
	cJSON *payload_root = cJSON_CreateObject();

	cJSON *cmdtype = cJSON_CreateString(CMD_SND_HWEV);
	cJSON *cmdOpt_target = cJSON_CreateString((const char*)target.c_str());
	cJSON *cmdOpt_event = cJSON_CreateString((const char*)event.c_str());
	cJSON *cmdOpt_opt = cJSON_CreateString((const char*)opt.c_str());

	cJSON_AddItemToObject(payload_root, "cmdType", cmdtype);
	cJSON_AddItemToObject(payload_root, "target", cmdOpt_target);
	cJSON_AddItemToObject(payload_root, "event", cmdOpt_event);
	cJSON_AddItemToObject(payload_root, "eventOpt", cmdOpt_opt);
	if (target == "volume")
	{
		cJSON *cmdOpt_volume = cJSON_CreateNumber(atoi(opt.c_str()));
		cJSON_AddItemToObject(cmdOpt_opt, "value", cmdOpt_volume);
	}
	char *msgPayloadStr = cJSON_Print(payload_root);
	printf("[%s]", msgPayloadStr);
	agent_sendCommand(msgPayloadStr);

	cJSON_Delete(payload_root);
	free(msgPayloadStr);
}

void sendDSSP(char *pTtsTransport)
{
	cJSON *payload_root = cJSON_CreateObject();

	cJSON *cmdtype = cJSON_CreateString(CMD_REQ_DSSP);
	cJSON *cmdOpt_ttsTransport = cJSON_CreateString(pTtsTransport);

	cJSON_AddItemToObject(payload_root, "cmdType", cmdtype);
	cJSON_AddItemToObject(payload_root, "ttsTransport", cmdOpt_ttsTransport);

	char *msgPayloadStr = cJSON_Print(payload_root);
	printf("[%s]", msgPayloadStr);
	agent_sendCommand(msgPayloadStr);

	cJSON_Delete(payload_root);
	free(msgPayloadStr);
}

// 		case 'D':
void test_debugmode()
{
	char	ch[255];
	int		loop_exit = 0;

	puts("\nagent_debugmode menu:::");
	printf("	a) debugmode(off)       b) debugmode(on)       x,q) Exit\n");
	puts("");
	while (!loop_exit)
	{
		printf("choose a submenu >>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0])
		{
		case 'a':
		case 'A':
			printf("debugmode is off!\n");
			agent_debugmode(false);
			loop_exit = 1;
			break;
		case 'b':
		case 'B':
			printf("debugmode is on!\n");
			agent_debugmode(true);
			loop_exit = 1;
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
}

//		case '1':
REGISTER_CODE test_register(std::string ID, std::string KEY, std::string SECRET)
{
	REGISTER_CODE rc;

	printf("Calling agent_register().\n");
	rc = agent_register(ID, KEY, SECRET, "");
	if (rc.rc != 200)
	{
		printf("Error: agent_register got %d, %s\n", rc.rc, rc.rcmsg.c_str());
	}
	else
	{
		printf("agent_register received an uuid [%s] with a msg, %s\n", rc.uuid.c_str(), rc.rcmsg.c_str());
	}
	return rc;
}

//		case '2':
void test_init(std::string ID, std::string KEY, std::string SECRET, char *puuid)
{
	REGISTER_CODE rc;

	printf("Calling agent_init(), isInit=%d.\n",isInit);
	if(isInit && g_clientId == ID && g_clientKey == KEY && g_clientSecret == SECRET) { // v1.1.1
	    printf("already agent_init() succeed.\n");
	    return;    
	}
	
	rc = agent_init(ID, KEY, SECRET, puuid);
	if (rc.rc == 200)
	{
		printf("Calling agent_setCommandEventCallback(), onCommand=%p, onEvent=%p.\n", onCommand, onEvent);
		agent_setCommandEventCallback(onCommand, onEvent);

		isInit = true;
	}
	else
	{
		if (rc.rc == 404)
		{
			printf("agent_init() got 404. Please call agent_register() again!\n");
		}
	}
}

//		case '3':
void test_unregister() {
	if(!isInit) {
		printf("agent_unregister failed. call agent_init first!\n");
		return;
	}
	
	REGISTER_CODE rc = agent_unregister();
	if(rc.rc == 200) {
		printf("testProgram agent_unregister success!!\n");
		remove("uuid.txt");
		if(isKwsInit) kws_reset();
		isInit = isKwsInit = false;
	} else {
		printf("testProgram agent_unregister fail : %d\n", rc.rc);
	}

//	std::string ret = agent_unregister();
//	printf("agent_unregister() returned %s.\n", ret.c_str());
//
//	if(isKwsInit) kws_reset();
//	isInit = isKwsInit = false;
}

//		case '4':
void test_reset() {
	if(!isInit) {
		printf("agent_reset failed. call agent_init first!\n");
		return;
	}

	printf("Calling agent_reset().\n");
	agent_reset();

	if(isKwsInit) kws_reset();
	isInit = isKwsInit = false;
}

//		case '5':
void test_sendText() {
	if(!isInit) {
		printf("agent_sendText failed. call agent_init first!\n");
		return;
	}

	char outbuf[4096];
	size_t length;

	memset(outbuf, 0x0, sizeof(outbuf));

	printf("Type the text to send >>> ");
	fgets(outbuf, sizeof(outbuf), stdin);
	length = strlen(outbuf);
	if (length <= 0) {
		printf("nothing to send, so returned to a main menu\n");
	} else {
		if (outbuf[length - 1] == '\n') outbuf[length - 1] = 0x0;
	}

	printf("calling agent_sendText() with a text, [%s]\n", outbuf);
	agent_sendText(outbuf);
}

void usage_sendCommand() {
	puts("\nagent_sendCommand menu:::");
	printf("	h) help\n");
	printf("	p) play       a) pause       v) previous     n) next\n");
	printf("	u) volume-up  d) volume-down t) TTS Transport(DSSP)\n");
	printf("	b) BT\n");
	printf("	x,q) exit\n");
	puts("");
}

//		case '6':
void test_sendCommand() {
	if(!isInit) {
		printf("agent_sendCommand failed. call agent_init first!\n");
		return;
	}

	char	ch[255];
	int		loop_exit = 0, inner_loop_exit = 0;
	static int	volume_level = 5;
	char volume_str[5];

	memset(volume_str, 0x0, sizeof(volume_str));
	usage_sendCommand();
	while (!loop_exit) {
		printf("choose a submenu >>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0]) {
		case '?':
		case 'h':
		case 'H':
			usage_sendCommand();
			break;
		case 'p':
		case 'P':	// play
			printf("play!\n");
			sendHwEvent("button", "Btn_PU", "");
			loop_exit = 1;
			break;
		case 'a':
		case 'A':	// pause
			printf("pause!\n");
			sendHwEvent("button", "Btn_PU", "");
			loop_exit = 1;
			break;
		case 'v':
		case 'V':	// previous
			printf("previous!\n");
			sendHwEvent("button", "Btn_PV", "");
			loop_exit = 1;
			break;
		case 'n':
		case 'N':	// next
			printf("next!\n");
			sendHwEvent("button", "Btn_NX", "");
			loop_exit = 1;
			break;
		case 'u':
		case 'U':	// volume up
			printf("Volumen up (%d)!\n", volume_level);
			sprintf(volume_str, "%d", volume_level);
			volume_level++;
			sendHwEvent("volume", "setVolume", volume_str);
			loop_exit = 1;
			break;
		case 'd':
		case 'D':	// volume down
			printf("Volumen down (%d)!\n", volume_level);
			sprintf(volume_str, "%d", volume_level);
			volume_level--;
			if (volume_level < 0) volume_level = 0;
			sendHwEvent("volume", "setVolume", volume_str);
			loop_exit = 1;
			break;
		case 't':
		case 'T':	// TTS Transport(DSSP)
			while(!inner_loop_exit)
			{
				printf("agent_sendCommand:TTS Transport (DSSP) submenu >>>\n");
				printf("	              v) voice       s) stream\n");
				printf("choose v for voice or s for stream >>> ");
				fgets(ch, sizeof(ch), stdin);
				switch(ch[0])
				{
				case 'v':
				case 'V':
					sendDSSP((char*)"voice");
					inner_loop_exit = 1;
					break;
				case 's':
				case 'S':
					sendDSSP((char*)"stream");
					inner_loop_exit = 1;
					break;
				default:
					printf("incorrect input!\n\n");
					break;
				}
			}
			loop_exit = 1;
			break;
		case 'b':
		case 'B':	// BT
			while (!inner_loop_exit)
			{
				printf("agent_sendCommand:BT submenu >>>\n");
				printf("	              p) BT play     a) BT pause   d) BT teardown\n");
				printf("choose an option >>> ");
				fgets(ch, sizeof(ch), stdin);
				switch (ch[0])
				{
				case 'p':
				case 'P':
					sendHwEvent("bluetooth", "BTEvent", "801");
					inner_loop_exit = 1;
					break;
				case 'a':
				case 'A':
					sendHwEvent("bluetooth", "BTEvent", "802");
					inner_loop_exit = 1;
					break;
				case 'd':
				case 'D':
					sendHwEvent("bluetooth", "BTEvent", "800");
					inner_loop_exit = 1;
					break;
				default:
					printf("incorrect input!\n\n");
					break;
				}
			}
			loop_exit = 1;
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
}

//		case '7':
void test_startVoice() {
	if(!isInit) {
		printf("agent_startVoice failed. call agent_init first!\n");
		return;
	}

	char	filename[255];
	size_t	length;

	memset(filename, 0x0, sizeof(filename));

	printf("Enter the pcm file name to send >>> ");
	fgets(filename, sizeof(filename), stdin);
	length = strlen(filename);
	if (length <= 0)
	{
		printf("nothing to send, so returned to a main menu\n");
		return;
	}
	else
	{
		if (filename[length - 1] == '\n') filename[length - 1] = 0x0;
	}

	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fclose(fp);
		strcpy(g_szFilename, filename);
		printf("Calling agent_startVoice().\n");
		agent_startVoice();
	}
	else
	{
		printf("A file, %s does not exist!\n", filename);
	}
}

//		case '8':
void test_stopVoice() {
	if(!isInit) {
		printf("agent_stopVoice failed. call agent_init first!\n");
		return;
	}

	printf("Calling agent_stopVoice().\n");
	agent_stopVoice();
}

//		case '9':
std::string test_getTTS() {
	if(!isInit) {
		printf("agent_getTTS failed. call agent_init first!\n");
        
		cJSON *root = cJSON_CreateObject();
		cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(500));
		cJSON_AddItemToObject(root, "rcmsg", cJSON_CreateString("Internal SDK error"));
        
		char *msgPayloadStr = cJSON_Print(root);    
		std::string sendmsg(msgPayloadStr);
		free(msgPayloadStr);
		cJSON_Delete(root);

		return sendmsg;
	}

	char inbuf[4096];
	size_t length;

	memset(inbuf, 0x0, sizeof(inbuf));
	printf("Type the text to send to a TTS >>> ");
	fgets(inbuf, sizeof(inbuf), stdin);
	length = strlen(inbuf);
	if (length <= 0) {
		printf("nothing to send, so returned to a main menu\n");
	} else {
		if (inbuf[length - 1] == '\n') inbuf[length - 1] = 0x0;
	}

	printf("calling agent_getTTS() with a text, [%s]\n", inbuf);
	std::string sendmsg = agent_getTTS(inbuf);
	printf("agent_getTTS got %s\n", sendmsg.c_str());
	return sendmsg;	
}

//		case '0':
void test_serviceLogin() {
	if(!isInit) {
		printf("agent_serviceLogin failed. call agent_init first!\n");
		return;
	}

	printf("calling agent_serviceLogin('geniemusic', '')\n");
	std::string strRet = agent_serviceLogin("geniemusic", "");
	printf("returned %s\n", strRet.c_str());
	if (strRet.size() > 0)
	{
		cJSON *cmdp_jsonObj = cJSON_Parse(strRet.c_str());
		if (cmdp_jsonObj == NULL)
		{
			printf("ERROR: agent_serviceLogin returns %s\n", strRet.c_str());
		}
		else
		{
			cJSON *cmdp_rc = cJSON_GetObjectItem(cmdp_jsonObj, "rc");
			cJSON *cmdp_rcmsg = cJSON_GetObjectItem(cmdp_jsonObj, "rcmsg");
			if (cmdp_rc->valueint == 200)
			{
				printf("agent_serviceLogin returned, [%s]\n\n", strRet.c_str());

				cJSON *cmdp_oauth_url = cJSON_GetObjectItem(cmdp_jsonObj, "oauth_url");
				printf("oauth_url %s\n", cmdp_oauth_url->valuestring);
			}
			else
			{
				printf("ERROR: agent_serviceLogin returns an error, return code=%d, msg=%s\n", cmdp_rc->valueint, cmdp_rcmsg->valuestring);
			}
			cJSON_Delete(cmdp_jsonObj);
		}
	}
}

//		case 'a':
void test_dialog() {
	if(!isInit) {
		printf("dialog_test failed. call agent_init first!\n");
		return;
	}

	char	filename[255];
	size_t	length;
	FILE	*fp;

	memset(filename, 0x0, sizeof(filename));

	printf("Enter the first pcm file name to send >>> ");
	fgets(filename, sizeof(filename), stdin);
	length = strlen(filename);
	if (length <= 0)
	{
		printf("nothing to send, so returned to a main menu\n");
		return;
	}
	else
	{
		if (filename[length - 1] == '\n') filename[length - 1] = 0x0;
	}

	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fclose(fp);
		strcpy(g_szDialogFilename1, filename);
	}
	else
	{
		printf("A file, %s for a dialog does not exist!\n", filename);
		return;
	}

	memset(filename, 0x0, sizeof(filename));

	printf("Enter the second pcm file name to send >>> ");
	fgets(filename, sizeof(filename), stdin);
	length = strlen(filename);
	if (length <= 0)
	{
		printf("nothing to send, so returned to a main menu\n");
		return;
	}
	else
	{
		if (filename[length - 1] == '\n') filename[length - 1] = 0x0;
	}

	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fclose(fp);
		strcpy(g_szDialogFilename2, filename);
		printf("Calling agent_startVoice().\n");
		g_start_dialog = 1;
		g_dialog_count = 0;
		agent_startVoice();
	}
	else
	{
		printf("A file, %s for a dialog does not exist!\n", filename);
		return;
	}
}

//		case 'b':
void test_kws_init() {
	if(!isInit) {
		printf("kws_init failed. call agent_init first!\n");
		return;
	}

	printf("calling kws_init()\n");
	int ret = kws_init();
	if(ret == 0) {
		printf("kws_init() returned %d, success.\n", ret);
		isKwsInit = true;
	} else {
		printf("kws_init() returned %d, fail.\n", ret);
		isKwsInit = false;
	}
}

#define CHUNK_SIZE 160

//		case 'c':
void test_kws_detect() {
	if(!isKwsInit) {
		printf("kws_detect failed. call kws_init first!\n");
		return;
	}

	int		count, ret, prev_ret, length;
	char	filename[255];
	FILE	*fp;
	short	pcm_buf[CHUNK_SIZE];

	printf("Enter the pcm file name for KWS >>> ");
	fgets(filename, sizeof(filename), stdin);
	length = strlen(filename);
	if (length <= 0) {
		printf("nothing to send, so returned to a main menu\n");
	} else {
		if (filename[length - 1] == '\n') filename[length - 1] = 0x0;
	}
	fp = fopen(filename, "rb");
	if (fp == NULL)	{
		printf("The file, %s DOES NOT exist!\n", filename);
		return;
	}

	printf("KWS version: %s\n", kws_getVersion());
	printf("The current kws_getKeyword() is %d\n", kws_getKeyword());

	printf("calling kws_detect()\n");
	count = 0;
	prev_ret = -5;
	while (!feof(fp)) {
		size_t read_len = fread(pcm_buf, sizeof(short), CHUNK_SIZE, fp);
		ret = kws_detect(pcm_buf, read_len);
		if (ret == KWS_DET_DETECTED) {
			printf("Now Detected!\n");
			count++;
		} else {
			if (prev_ret != ret) {
				printf("kws_detect returns %d\n",ret);
				prev_ret = ret;
			}
		}
	}
	fclose(fp);
	printf("Now KWS Ends! kws_count=%d\n\n", count);
}

void usage_kws_setKeyword() {
	puts("\nkws_setKeyword menu:::");
	printf("	0) GGN       1) GNY       2) CGY     3) JGY\n");
	printf("	x,q) exit\n");
	puts("");
}

//		case 'e':
void test_kws_setKeyword() {
	if(!isKwsInit) {
		printf("kws_setKeyword failed. call kws_init first!\n");
		return;
	}

	char	ch[255];
	int		loop_exit = 0;

	usage_kws_setKeyword();
	while (!loop_exit)
	{
		printf("choose a submenu >>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0])
		{
		case '0':
			printf("Giga Genie!\n");
			kws_setKeyword(0);
			loop_exit = 1;
			break;
		case '1':
			printf("Genie Ya!\n");
			kws_setKeyword(1);
			loop_exit = 1;
			break;
		case '2':
			printf("Chin Gu Ya!\n");
			kws_setKeyword(2);
			loop_exit = 1;
			break;
		case '3':
			printf("Ja Gi Ya!\n");
			kws_setKeyword(3);
			loop_exit = 1;
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
}
//		case 'f':
void test_setServerInfo() {
	char	ch[255];
	char	server[255], grpcPort[100], restPort[100];
	int		length, loop_exit = 0;
	FILE	*fp;

	puts("\nagent_setServerInfo:::");
	printf("	0) use Dev Server       1) user input     x) exit!\n");
	while (!loop_exit)
	{
		printf("choose a submenu >>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0])
		{
		case '0':
			fp = fopen("./conf/server_info.txt", "rt");
			if(fp != NULL)
			{
				fgets(server, sizeof(server), fp);
				length = strlen(server);
				if (server[length - 1] == '\n') server[length - 1] = 0x0;

				fgets(grpcPort, sizeof(grpcPort), fp);
				length = strlen(grpcPort);
				if (grpcPort[length - 1] == '\n') grpcPort[length - 1] = 0x0;

				fgets(restPort, sizeof(restPort), fp);
				length = strlen(restPort);
				if (restPort[length - 1] == '\n') restPort[length - 1] = 0x0;
				fclose(fp);

				printf("--->  server:%s, grpcPort=%s, restPort=%s\n", server, grpcPort, restPort);
				agent_setServerInfo(server, grpcPort, restPort);
				loop_exit = 1;
			}
			else
			{
				printf("Don't have server info. Use the menu, user input.\n");
			}
			break;
		case '1':
			printf("Type server >>> ");
			fgets(server, sizeof(server), stdin);
			length = strlen(server);
			if (server[length - 1] == '\n') server[length - 1] = 0x0;

			printf("Type GRPC port, ex) 50109 >>> ");
			fgets(grpcPort, sizeof(grpcPort), stdin);
			length = strlen(grpcPort);
			if (grpcPort[length - 1] == '\n') grpcPort[length - 1] = 0x0;

			printf("Type REST port, ex) 30109 >>> ");
			fgets(restPort, sizeof(restPort), stdin);
			length = strlen(restPort);
			if (restPort[length - 1] == '\n') restPort[length - 1] = 0x0;

			agent_setServerInfo(server, grpcPort, restPort);
			loop_exit = 1;
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
}
//		case 'g':
void test_getVersion() {
	printf("agent_getVersion() returns %s\n", agent_getVersion());
}

//		case 'i':
void test_setLocation(){
	if(!isInit) {
		printf("agent_setLocation failed. call agent_init first!\n");
		return;
	}

	char	ch[255];
	char	Longitude[255], Latitude[255], Address[255];
	int		length, loop_exit = 0;

	puts("\nagent_setLocation:::");
	printf("	0) use a sample       1) user input     x) exit!\n");
	while (!loop_exit)
	{
		printf("choose a submenu >>> ");
		fgets(ch, sizeof(ch), stdin);
		switch (ch[0])
		{
		case '0':
			printf("Use a sample, Longitude(35.232640), Latitude(123.456789), Address(Seoul, Korea)\n");
			agent_setLocation("35.232640", "123.456789", "Seoul, Korea");
			loop_exit = 1;
			break;
		case '1':
			printf("Type Longitude, ex) 35.232640 >>> ");
			fgets(Longitude, sizeof(Longitude), stdin);
			length = strlen(Longitude);
			if (Longitude[0] == '\n') strcpy(Longitude, "35.232640");
			else if (Longitude[length - 1] == '\n') Longitude[length - 1] = 0x0;

			printf("Type Latitude, ex) 123.456789 >>> ");
			fgets(Latitude, sizeof(Latitude), stdin);
			length = strlen(Latitude);
			if (Latitude[0] == '\n') strcpy(Latitude, "123.456789");
			else if (Latitude[length - 1] == '\n') Latitude[length - 1] = 0x0;

			printf("Type Address, ex) Seoul Korea in Korean >>> ");
			fgets(Address, sizeof(Address), stdin);
			length = strlen(Address);
			if (Address[length - 1] == '\n') Address[length - 1] = 0x0;

			agent_setLocation(Longitude, Latitude, Address);
			loop_exit = 1;
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
}

//		case 'j':
void test_setModelPath() {
	char	ch[255];
	int		length, loop_exit = 0;

	puts("\nkws_setModelPath:::\n");
	while (!loop_exit)
	{
		printf("Type KWS Model Path(directory) >>> ");
		fgets(ch, sizeof(ch), stdin);
		length = strlen(ch);
		if (ch[length - 1] == '\n') ch[length - 1] = 0x0;
		printf("kws_setModelPath returned %d\n", kws_setModelPath(ch));
		loop_exit = 1;
	}
}

//      case 'k':
void test_serviceLoginStatus() {
    if(!isInit) {
        printf("agent_serviceLoginStatus failed. call agent_init first!\n");
        return;
    }

    printf("calling agent_serviceLoginStatus('geniemusic')\n");
    std::string strRet = agent_serviceLoginStatus("geniemusic");
    printf("returned %s\n", strRet.c_str());
    if (strRet.size() > 0)
    {
        cJSON *cmdp_jsonObj = cJSON_Parse(strRet.c_str());
        if (cmdp_jsonObj == NULL)
        {
            printf("ERROR: agent_serviceLoginStatus returns %s\n", strRet.c_str());
        }
        else
        {
            cJSON *cmdp_rc = cJSON_GetObjectItem(cmdp_jsonObj, "rc");
            cJSON *cmdp_rcmsg = cJSON_GetObjectItem(cmdp_jsonObj, "rcmsg");
            if (cmdp_rc->valueint == 200)
            {
                printf("agent_serviceLoginStatus returned, [%s]\n\n", strRet.c_str());

                cJSON *cmdp_oauth_status = cJSON_GetObjectItem(cmdp_jsonObj, "oauth_status");
                cJSON *cmdp_oauth_token = cJSON_GetObjectItem(cmdp_jsonObj, "oauth_token");
                cJSON *cmdp_user_name = cJSON_GetObjectItem(cmdp_jsonObj, "user_name");
                if(cmdp_oauth_status != NULL) {
                    printf("oauth_status : %s\n", cmdp_oauth_status->valuestring);
                }
                if(cmdp_oauth_token != NULL) {
                    printf("oauth_token : %s\n", cmdp_oauth_token->valuestring);
                }
                if(cmdp_user_name != NULL) {
                    printf("user_name : %s\n", cmdp_user_name->valuestring);
                }
            }
            else
            {
                printf("ERROR: agent_serviceLoginStatus returns an error, return code=%d, msg=%s\n", cmdp_rc->valueint, cmdp_rcmsg->valuestring);
            }
            cJSON_Delete(cmdp_jsonObj);
        }
    }
}

//      case 'k':
void test_serviceLogout() {
    if(!isInit) {
        printf("agent_serviceLogout failed. call agent_init first!\n");
        return;
    }

    printf("calling agent_serviceLogout('geniemusic')\n");
    std::string strRet = agent_serviceLogout("geniemusic");
    printf("returned %s\n", strRet.c_str());
    if (strRet.size() > 0)
    {
        cJSON *cmdp_jsonObj = cJSON_Parse(strRet.c_str());
        if (cmdp_jsonObj == NULL)
        {
            printf("ERROR: agent_serviceLogout returns %s\n", strRet.c_str());
        }
        else
        {
            cJSON *cmdp_rc = cJSON_GetObjectItem(cmdp_jsonObj, "rc");
            cJSON *cmdp_rcmsg = cJSON_GetObjectItem(cmdp_jsonObj, "rcmsg");
            printf("agent_serviceLogout returns, result code=%d, msg=%s\n", cmdp_rc->valueint, cmdp_rcmsg->valuestring);
            cJSON_Delete(cmdp_jsonObj);
        }
    }
}

void test_kws(int kwsId)
{
	int		count, ret, prev_ret;
	char	filename[255];
	FILE	*fp;
	short	pcm_buf[CHUNK_SIZE];

	printf("Now KWS %d Starts!\n", kwsId);
	printf("%d. kws version, %s\n", kwsId, kws_getVersion());
	printf("%d. current kws_getKeyword() is %d\n", kwsId, kws_getKeyword());
	printf("%d. now kws_setKeyword(%d) returned %d\n", kwsId, kwsId, kws_setKeyword(kwsId));
	sprintf(filename, "./kws/kws_%d.pcm", kwsId);

	count = 0;
	prev_ret = -5;
	fp = fopen(filename, "rb");
	if (fp != NULL) {
		while (!feof(fp)) {
			size_t read_len = fread(pcm_buf, sizeof(short), CHUNK_SIZE, fp);
			ret = kws_detect(pcm_buf, read_len);
			if (ret == KWS_DET_DETECTED) {
				printf("%d. Now Detected!\n", kwsId);
				count++;
			} else {
				if(prev_ret != ret)	{
					printf("%d. kws_detect returns %d\n", kwsId, ret);
					prev_ret = ret;
				}
			}
		}
		fclose(fp);
	}
	printf("Now KWS %d Ends! kws_count=%d\n\n", kwsId, count);
}

void test_all_kws()
{
	printf("kws version, %s\n", kws_getVersion());
	printf("kws_init() returned %d\n", kws_init());

	printf("press enter to kws_0\n");
	getchar();
	test_kws(0);
	printf("press enter to kws_1\n");
	getchar();
	test_kws(1);
	printf("press enter to kws_2\n");
	getchar();
	test_kws(2);
	printf("press enter to kws_3\n");
	getchar();
	test_kws(3);
}
