#ifndef  __GINSIDEDEF_H__
#define  __GINSIDEDEF_H__

// Agent Server Command
#define  CMD_REQ_VOCM		"Req_VOCM"  // Device --> Server
#define  CMD_REQ_CAVM		"Req_CAVM"  // Device --> Server
#define  CMD_RES_VOCM		"Res_VOCM"  // Server --> Device
#define  CMD_REQ_STRV		"Req_STRV"  // Server --> Device
#define  CMD_REQ_STPV		"Req_STPV"  // Server --> Device
#define  CMD_REQ_PLMD		"Req_PLMD"  // Server --> Device
#define  CMD_REQ_UPMD		"Req_UPMD"  // Server --> Device
#define  CMD_UPD_MEST		"Upd_MEST"  // Device --> Server
#define  CMD_REQ_UPDS		"Req_UPDS"  // Server --> Device
#define  CMD_UPD_DSST		"Upd_DSST"  // Device --> Server
#define  CMD_REQ_HWCL		"Req_HWCL"  // Server --> Device
#define  CMD_RES_HWCL		"Res_HWCL"  // Device --> Server
#define  CMD_REQ_PSTM		"Req_PSTM"  // Server --> Device
#define  CMD_SND_ERRO		"Snd_ERRO"  // Server --> Device
#define  CMD_SND_HWEV		"Snd_HWEV"  // Device --> Server
#define  CMD_SND_PHEV		"Snd_PHEV"  // Device --> Server
#define  CMD_REQ_STTM		"Req_STTM"  // Server --> Device
#define  CMD_SND_TMEV		"Snd_TMEV"  // Device --> Server
#define  CMD_REQ_DEAC		"Req_DEAC"  // Server --> Device
#define  CMD_REQ_STDI		"Req_STDI"  // Server --> Device
#define  CMD_SND_SVEV		"Snd_SVEV"  // Server --> Device
#define  CMD_REQ_DSSP		"Req_DSSP"  // Device --> Server
#define  CMD_REQ_SDSP		"Req_SDSP"  // Server --> Device
#define  CMD_SND_FCRS		"Snd_FCRS"  // Device --> Server
#define  CMD_REQ_QFMI		"Req_QFMI"  // Server --> Device
#define  CMD_RES_QFMI		"Res_QFMI"  // Device --> Server
#define  CMD_REQ_TXCM		"Req_TXCM"  // Device --> Service
#define  CMD_REQ_STCK		"Req_STCK"  // Device --> Service
#define  CMD_RES_STCK		"Res_STCK"  // Server --> Device
#define  CMD_SND_PSTT		"Snd_PSTT"  // Server --> Device

#define  CMD_REQ_VOTX		"Req_VOTX"  // Device --> Service(STT ONLY) 21/01/20
#define  CMD_RES_VOTX		"Res_VOTX"  // Server --> Device (STT_ONLY)


#define  CMD_REQ_EXEC		"Req_EXEC"  // Device --> Service
#define  CMD_REQ_OAUTH		"Req_OAuth" // Device --> Service
#define  CMD_SND_ERR        "Snd_ERR"	// ?

#define  CMD_REQ_CONF		"Req_CONF"  // Device --> Server
#define  CMD_RES_CONF       "Res_CONF"  // Server --> Device


namespace INSIDE_EVENT {
const int GRPC_INIT_SUCCESS = 0;
const int GRPC_INIT_FAIL = 1; // add 190925
const int GRPC_DISCONNECTED = 2; // add 190925
const int GO_TO_STANDBY = 3; // v1.0.3_04010916
const int SET_CONFIG_SUCCESS = 4;
const int SET_CONFIG_FAIL = 5;
const int S2S_REQUEST = 6;			// 음성인식 결과에 대해 S2S 요청 (2021.10.11)

const int TIMEOUT_START_VOICE = 601; // req_vocm 전송 후에 res_vocm 을 전달받지 못했을
const int TIMEOUT_STOP_VOICE = 602; // req_strv 수신 후 음성데이터를 보낸 시점부터 10초 뒤에 REQ_STPV 응답을 못받았을 때.
const int TIMEOUT_RECEIVE_DATA = 603; // req_stpv 수신 후에 아무런 데이터를 못받았을 때.

const int SERVER_ERROR = 205;
}

#endif		// end of __GINSIDEDEF_H__
