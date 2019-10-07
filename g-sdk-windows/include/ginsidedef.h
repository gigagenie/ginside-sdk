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

#define  CMD_REQ_EXEC		"Req_EXEC"  // Device --> Service
#define  CMD_REQ_OAUTH		"Req_OAuth" // Device --> Service
#define  CMD_SND_ERR		"Snd_ERR"	// ?


namespace INSIDE_EVENT
{
	const int RESULT_INIT_SUCCESS = 0;
	const int VOICE_START = 201;
	const int VOICE_STOP = 202;
	const int SERVER_ERROR = 205;
}


#endif		// end of __GINSIDEDEF_H__
