/*
 * GInside SDK API header file
 *
 */

#ifndef  __GINSIDE_H__
#define  __GINSIDE_H__


#include  <string>


#ifdef  __cplusplus
extern "C" {
#endif


#ifdef  LINUX
#define  DLL_EXPORT
#else
#define  DLL_EXPORT		extern "C" __declspec(dllexport)
#endif


DLL_EXPORT void agent_setServerInfo(std::string serverIP, std::string grpcPort, std::string restPort);
DLL_EXPORT const char* agent_getVersion();

typedef struct
{
    int rc;
    std::string rcmsg;
    std::string uuid;
} REGISTER_CODE;

DLL_EXPORT REGISTER_CODE agent_register(std::string client_id, std::string client_key, std::string client_secret, std::string user_id);
DLL_EXPORT REGISTER_CODE agent_init(std::string client_id, std::string client_key, std::string client_secret, std::string uuid);

DLL_EXPORT REGISTER_CODE agent_unregister();

DLL_EXPORT void agent_setLocation(std::string Longitude, std::string Latitude, std::string Address);

DLL_EXPORT void agent_sendText(std::string msg);

DLL_EXPORT void agent_sendVoice(short data[], int size);

typedef void (*agent_onCommand)(std::string actionType, std::string payload);
typedef void (*agent_onEvent)(int eventMask, std::string opt);
DLL_EXPORT void agent_setCommandEventCallback(agent_onCommand aoc, agent_onEvent aoe);

DLL_EXPORT void agent_sendCommand(std::string cmd);

DLL_EXPORT void agent_updateMediaStatus(int ch, std::string state, int playtime);

DLL_EXPORT void agent_startVoice();
DLL_EXPORT void agent_startVoiceToText();   // STT Only API
DLL_EXPORT void agent_startVoicePartial();  // 2022.05.20 partial result
DLL_EXPORT void agent_stopVoice();
DLL_EXPORT std::string agent_getTTS(std::string msg);   // 2021.10.20 - remove sampleRates
// DLL_EXPORT std::string agent_getTTS(std::string msg, int sampleRates=0);    // 2021/04/20: sampleRates 0:default 24k
DLL_EXPORT std::string agent_getTTS_options(std::string msg, int sampleRates, std::string language, int speed); // 2021.10.11: language, speed

DLL_EXPORT std::string agent_serviceLogin(std::string service_type, std::string return_url);
DLL_EXPORT std::string agent_serviceLoginStatus(std::string service_type);
DLL_EXPORT std::string agent_serviceLogout(std::string service_type);

DLL_EXPORT void agent_debugmode(bool enable);

DLL_EXPORT void agent_reset();

DLL_EXPORT void agent_setConfig(std::string config);
DLL_EXPORT void agent_setCustomVersion(std::string appName, std::string version);

#define  KWS_ERR_SUCCESS			0
#define  KWS_ERR_NO_DIRECTORY		-1
DLL_EXPORT int kws_setModelPath(std::string path);

#define  KWS_ERR_SUCCESS			0
#define  KWS_ERR_FAILURE			-1
#define  KWS_ERR_UNAUTHORIZED		-2

DLL_EXPORT int kws_init();


#define  KWS_DET_ERROR				-1
#define  KWS_DET_SILENCE			0
#define  KWS_DET_START				1
#define  KWS_DET_SPEECH				2
#define  KWS_DET_END				3
#define  KWS_DET_DETECTED			4

DLL_EXPORT int kws_detect(short data[], int size);

DLL_EXPORT void kws_reset();
DLL_EXPORT int kws_error();
DLL_EXPORT const char *kws_getVersion();
DLL_EXPORT int kws_getKeyword();
DLL_EXPORT int kws_setKeyword(int kwsId);

#ifdef  __cplusplus
}
#endif


#endif	/* end of __GINSIDE_H__ */
