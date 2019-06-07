
/*
 *  Quectel WIFI at command handler
 *
 *  file name: quectel_wifi_at_handler.h
 *
 *  Design by: hui.chen 
 *
 *  Date: 2016-06-30
 */

#ifndef __QUECTEL_WIFI_AT_HANDLER__
#define __QUECTEL_WIFI_AT_HANDLER__

#ifdef __cplusplus
extern "C" {
#endif

#include "quectel_at_cmd_defs.h"
#include "quectel_wifi_defs.h"

#define Q_QWIFI_AT_CMD_FWD_REQ_TBL    \
		Q_AT_CMD_REQ_TMP("+QWIFI")	\
		Q_AT_CMD_REQ_TMP("+QWSSIDHEX")\
		Q_AT_CMD_REQ_TMP("+QWSSID")\
		Q_AT_CMD_REQ_TMP("+QWBCAST") \
		Q_AT_CMD_REQ_TMP("+QWAUTH") \
		Q_AT_CMD_REQ_TMP("+QWMOCH")\
		Q_AT_CMD_REQ_TMP("+QWISO")\
		Q_AT_CMD_REQ_TMP("+QWDHCP")\
		Q_AT_CMD_REQ_TMP("+QWNAT") \
		Q_AT_CMD_REQ_TMP("+QWCLICNT") \
		Q_AT_CMD_REQ_TMP("+QWRSTD") \
		Q_AT_CMD_REQ_TMP("+QWCLIP") \
		Q_AT_CMD_REQ_TMP("+QWSERVER")\
		Q_AT_CMD_REQ_TMP("+QWCLILST") \
		Q_AT_CMD_REQ_TMP("+QWCLIRM")\
		Q_AT_CMD_REQ_TMP("+QWSETMAC")\
		Q_AT_CMD_REQ_TMP("+QLINUXCMD")\
		Q_AT_CMD_REQ_TMP("+QWTOCLIEN")\
		Q_AT_CMD_REQ_TMP("+QWTOCLI")\
		Q_AT_CMD_REQ_TMP("+QWPARAM")\
		Q_AT_CMD_REQ_TMP("+QDATAFWD")\
		Q_AT_CMD_REQ_TMP("+QWWAN")\
		Q_AT_CMD_REQ_TMP("+QSETMAC")\
		Q_AT_CMD_REQ_TMP("+QWIFICFG")\
		Q_AT_CMD_REQ_TMP("+QWWM")\
		Q_AT_CMD_REQ_TMP("+QWSTASSID")\
		Q_AT_CMD_REQ_TMP("+QWSTAAUTH")\
		Q_AT_CMD_REQ_TMP("+QWSTAINFO")\
		Q_AT_CMD_REQ_TMP("+QWSTATUS")\
		Q_AT_CMD_REQ_TMP("+QWMAXSTA")\

#define QWIFI_QCMAP_START_TIME      500
#define CLIENT_NUM		20
#define MAC_ADDR_LEN	6

typedef struct {
    char     *cmdName;
    int       nameLen;
    fuc_handler  fuc_hdl;
    parse_handler parser;  
    at_handler handler;
    query_handler querier;
    exec_handler  exec;
} _CMD_TABLE;

typedef struct {
    int auth;
    int encrypt;
    union {
    	struct {
    		int passwordindex;
    		char password1[28];
    		char password2[28];
    		char password3[28];
    		char password4[28];
    	}wep_passwd;
    	struct {
    		int is_psk;
    		char passwd[64];
    	}aes_passwd;
    } passwd;
} qwauth_passwd_type;

typedef struct {
	int host_ip;
	int start_ip;
	int end_ip;
	int leasetime; 
} qwdhcp_s_type;

typedef struct 
{
    int mac;
    int ip;
    int hostname;
} qwstainfo_s_type;

typedef struct {
    uint8_t    im_op;      /* operation to perform */
    uint16_t   im_reason;  /* 802.11 reason code */
    uint8_t    im_macaddr[MAC_ADDR_LEN];
}ieee80211_mlme_req_type;

typedef struct {
	char *buffer;
	int len;
	int ip[4];
	short port;
	char dport_flag;
	char string_flag;
} qdatafwd_s_type;

typedef struct{
	int ploy;
	char* wifimac;
}wifi_macbwl_type;

typedef enum{
	IS_NULL = 0,
	NOT_EXIST,
	IS_TURE
}wifi_macbwl_state;

extern boolean quectel_parse_wifi_cmd_input(AtCmd *cmd, atfwd_param_s_type *param);
extern boolean quectel_handle_wifi_at_cmd(atfwd_param_s_type *param, quec_AtCmdResponse *response);

extern int quectel_parse_wificfg_Ex(AtCmd *cmd, atfwd_param_s_type *param);
extern void quectel_handle_wificfg_Ex(void *param, quec_AtCmdResponse *response);

extern qwifi_cmd_para_type qwifi_cmd;
//extern int backup_wifi_configrations(void);
#ifdef __cplusplus
}
#endif
#endif /* __QUECTEL_WIFI_AT_HANDLER__ */
