#pragma once

#include <thread>
#include <string>
#include <vector>
#include <cpprest/ws_client.h>

namespace cppr_wsclient = web::websockets::client;
namespace cppr_pplx=pplx;
class Kws;

#define ACTION_TYPE_STOP_VOICE "stop_voice"
#define ACTION_TYPE_DIALOG_RESPONSE "exec_dialogkit"
#define ACTION_TYPE_START_VOICE "start_voice"
#define ACTION_TYPE_PLAY_MEDIA "play_media"
#define ACTION_TYPE_CONTROL_MEDIA "control_media"
#define ACTION_TYPE_HARDWARE_CONTROL "hardware_control"
#define ACTION_TYPE_SET_TIMER "set_timer"
#define ACTION_TYPE_WEBVIEW_URL "webview_url"

class WsConGinside{
    private:
        const int error_type_socket_close=1;
        const std::string ws_proto_name="device-protocol";
        std::string m_uuid;
        std::string m_clientid; 
        std::string m_clientkey;
        std::string m_clientsecret;
        std::string m_clienttype;
        std::string m_desturi;
        std::string m_authouri;
        std::string m_loginuri;
        std::string m_ttsuri;
        bool m_locationSet=false;
        std::string m_longitude;
        std::string m_latitude;
        std::string m_address;
        std::vector<std::string> m_arrayDevSts;// device status,aka SU:xxx
        cppr_wsclient::websocket_client_config config;
        cppr_wsclient::websocket_callback_client* pclient=nullptr;

        //for gserver handshaking at the beginning of session
        cppr_pplx::task_completion_event<void>* p_tce;
        //flag for if handshaking is done
        volatile bool is_hs_done=false;
        //for asyncronous listening
        cppr_pplx::task_completion_event<void> bintce;
        std::thread listener_thread;

        std::string _getLocParam();
        int _startListen(bool isOnce=false);
        int startListen();
        void sendMsg(const char* buf,int size,bool isBin=false);
        void addDevSts(std::vector<std::string>& stss);
        void delDevSts(std::vector<std::string>& stss);
        int startAsyncListen();
        int stopAsyncListen();


        static int g_nCurrentKwsModel;
        static std::string g_KwsModelDir;
        static Kws *g_pKws[5];
   
    protected:
        volatile bool is_user_stopped=false;

    public:
        static const int EVT_GOTO_STANDBY=1;
        static const int EVT_DISCONNECTED=2;
        static const int EVT_SEVER_ERROR=-1;
        static const int EVT_INIT_FAIL=-2;
        static const int ERR_SUCCESS=0;
        static const int ERR_NEED_INIT_INADVANCE=-1;
        static const int ERR_NEED_SESSION_START_INADVANCE=-2;
        static const int ERR_INVALID_PARAM=-3;
        static const int ERR_GENERAL_ERROR=-10;

        //send voice data to server
        void agent_sendVoice(const char* buf,int size);

        //send general json command to server
        void agent_sendCommand(const std::string& cmd);

        //send text msg to server
        void agent_sendTxt(const std::string& msg);

        //send 'startvoice' command to server
        void agent_startVoice();

        //send 'stopvoice' command to server
        void agent_stopVoice();
        
        //send client-side media status to server
        void agent_updateMediaStatus(int ch,const std::string& state,int playtime);

        //get uuid from auth sever
        std::string agent_register(const std::string& client_id="",const std::string& client_key=""
                                 ,const std::string& client_secret="",const std::string& user_id="");

        //del uuid from auth server
        std::string agent_unregister();

        //start 
        std::string agent_init(const std::string& client_id="",const std::string& client_key=""
                                 ,const std::string& client_secret="",const std::string& uuid="");
        
        //stop
        void agent_reset();

        //sevice login
        std::string agent_serviceLogin(const std::string& service_type,const std::string& return_url="");

        //get servicelogin status
        std::string agent_serviceLoginStatus(const std::string& service_type);

        //service logout
        std::string agent_serviceLogout(const std::string& service_type);

        //toggle debug msg
        void agent_debug(bool enable);

        //sdk version
        std::string agent_getVersion();

        //add location info each voice,txt command
        void agent_setLocation(const std::string& longitude, const std::string& latitude, const std::string& address)
        {
            m_locationSet=true;
            m_longitude=longitude;
            m_latitude=latitude;
            m_address=address;
        };

        std::string agent_getTTS(const std::string& msg);
        //handler command 
        virtual void onCommand(std::string& actionType,std::string& cmd)=0;
        //handler event
        virtual void onEvent(int evt,std::string opt="")=0;
        //handler sound data
        virtual void onMediaData(const char* buf,int size)=0;


        void agent_setServerInfo(const std::string& server_ip,const std::string& server_port) noexcept
        {
            m_desturi="https://"+server_ip+":"+server_port+"/channel";
            m_authouri="https://"+server_ip+":"+server_port+"/v2/authorize";
            m_loginuri="https://"+server_ip+":"+server_port+"/v2/serviceLogin";
            m_ttsuri="https://"+server_ip+":"+server_port+"/v2/text-to-speech";
        }

        WsConGinside();
        WsConGinside(const std::string& clientid,const std::string& clientkey,const std::string& clientsecret,const std::string& desturi
        ,const std::string& authouri,const std::string& clienttype="GINSIDE")
        :m_clientid(clientid)
        ,m_clientkey(clientkey),m_clientsecret(clientsecret)
        ,m_desturi(desturi),m_authouri(authouri)
        ,m_clienttype(clienttype)
        {
            config.add_subprotocol(ws_proto_name);
            pclient=new cppr_wsclient::websocket_callback_client(config);
        };

         ~WsConGinside(){
            delete pclient;
            pclient=nullptr;
        };

        std::vector<std::string>& getDevSts()
        {
            return m_arrayDevSts;
        };

        WsConGinside& setClientID(const std::string& clientid) noexcept
        {
            m_clientid=clientid;
            return *this;
        };

        WsConGinside& setClientkey(const std::string& clientkey) noexcept
        {
            m_clientkey=clientkey;
            return *this;
        };

        WsConGinside& setClientSecret(const std::string& clientsecret) noexcept
        {
            m_clientsecret=clientsecret;
            return *this;
        };

        WsConGinside& setClientType(const std::string& clienttype) noexcept
        {
            m_clienttype=clienttype;
            return *this;
        };

        //kws
        #define  KWS_ERR_SUCCESS			0
        #define  KWS_ERR_NO_DIRECTORY		-1
        int kws_setModelPath(std::string path);

        #define  KWS_ERR_SUCCESS			0
        #define  KWS_ERR_FAILURE			-1
        #define  KWS_ERR_UNAUTHORIZED		-2

        int kws_init();


        #define  KWS_DET_ERROR				-1
        #define  KWS_DET_SILENCE			0
        #define  KWS_DET_START				1
        #define  KWS_DET_SPEECH				2
        #define  KWS_DET_END				3
        #define  KWS_DET_DETECTED			4

        int kws_detect(short data[], int size);

        void kws_reset();
        int kws_error();
        const char *kws_getVersion();
        int kws_getKeyword();
        int kws_setKeyword(int kwsId);
};