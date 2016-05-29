#ifndef __JVX_CONNECT_SERVER_ST_H__
#define __JVX_CONNECT_SERVER_ST_H__

#include "jvx.h"

typedef enum
{
	JVX_CONNECT_SERVER_SOCKET_TYPE_TCP = 0,
	JVX_CONNECT_SERVER_SOCKET_TYPE_UDP = 1,
	JVX_CONNECT_SERVER_SOCKET_TYPE_BTH = 2
} jvx_connect_server_socket_type;

typedef enum
{
	JVX_SERVER_ERROR_NO_ERROR = 0,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_FAILED = 1,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_SETOPT_I = 2,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_SETOPT_II = 3,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_BIND = 4,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_LISTEN = 5,
	JVX_SERVER_ERROR_ID_CREATE_SOCKET_ACCEPT = 6
} jvx_connect_server_error_codes;

typedef enum
{
	JVX_CONNECT_SERVER_STATE_NONE = 0,
	JVX_CONNECT_SERVER_STATE_INIT = 1,
	JVX_CONNECT_SERVER_STATE_STARTED = 2,
	JVX_CONNECT_SERVER_STATE_WAIT_FOR_SHUTDOWN = 3
} jvx_connect_server_state;

typedef struct
{
	int disableNagleAlgorithmTcp;
} jvx_connect_server_spec_tcp;

typedef sockaddr_in udpTarget_server;

#ifdef OS_SUPPORTS_BTH
typedef struct
{
	const JVX_GUID* guidPtr;
} jvx_connect_server_spec_bth;
#endif


typedef struct
{
	jvxErrorType (*callback_report_startup_complete)(jvxHandle* privateData);
	jvxErrorType (*callback_report_shutdow_complete)(jvxHandle* privateData);

	jvxErrorType (*callback_provide_data_and_length)(jvxHandle* privateData, jvxInt32 channelId, char**, jvxSize*);
	jvxErrorType (*callback_report_data_and_read)(jvxHandle* privateData, jvxInt32 channelId, char*, jvxSize, const char*);

	jvxErrorType (*callback_report_denied_incoming)(jvxHandle* privateData, const char* description);

	jvxErrorType (*callback_report_connect_incoming)(jvxHandle* privateData, jvxInt32 channelId, const char* description);
	jvxErrorType (*callback_report_disconnect_incoming)(jvxHandle* privateData, jvxInt32 channelId);

	jvxErrorType (*callback_report_error)(jvxHandle* privateData, jvxInt32 errCode, const char* errDescription);

} jvx_connect_server_callback_type;


/** API FUNCTIONS **/
jvxErrorType jvx_connect_server_initialize_st(jvxHandle** hdlServerOnReturn, 
								   jvx_connect_server_socket_type socket_type, 
								   jvx_connect_server_callback_type* report_callback, 
								   jvxHandle* privData, 
								   jvxInt32 numConnectionsMax);

jvxErrorType jvx_connect_server_start_listening_st(jvxHandle* hdlServer, 
								   jvxHandle* config_specific,
								   int port);

jvxErrorType jvx_connect_server_whoami_st(jvxHandle* hdlServer, char* whoami_name, jvxSize szText_name, char* whoami_ip, jvxSize szText_ip);

jvxErrorType jvx_connect_server_preset_udp_target_st(jvxHandle* hdlServer, const char* server, int port, udpTarget_server* theTarget);

jvxErrorType jvx_connect_server_send_st(jvxHandle* hdlServer, int idChannel, char* buf, jvxSize* szFld, udpTarget_server* wheretosendto);
jvxErrorType jvx_connect_server_stop_one_connection_st(jvxHandle* hdlServer, jvxInt32 idConnection);

jvxErrorType jvx_connect_server_optional_wait_for_shutdown_st(jvxHandle* hdlServer);

jvxErrorType jvx_connect_server_trigger_shutdown_st(jvxHandle* hdlServer);

jvxErrorType jvx_connect_server_get_state_st(jvxHandle* hdlServer, jvx_connect_server_state* state_on_return);

jvxErrorType jvx_connect_server_terminate_st(jvxHandle* hdlServer);

jvxErrorType jvx_connect_server_lock_state(jvxHandle* hdlServer);

jvxErrorType jvx_connect_server_unlock_state(jvxHandle* hdlServer);

jvxErrorType jvx_connect_server_get_state(jvxHandle* hdlServer, jvx_connect_server_state* stateOnReturn);

#endif
