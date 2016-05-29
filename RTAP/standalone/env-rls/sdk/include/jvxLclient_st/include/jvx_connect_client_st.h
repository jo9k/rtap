#ifndef __JVX_CONNECT_CLIENT_ST_H__
#define __JVX_CONNECT_CLIENT_ST_H__

#include "jvx.h"

typedef enum
{
	JVX_CONNECT_CLIENT_SOCKET_TYPE_NONE = 0,
	JVX_CONNECT_CLIENT_SOCKET_TYPE_TCP,
	JVX_CONNECT_CLIENT_SOCKET_TYPE_UDP,
#ifdef JVX_WITH_PCAP
	JVX_CONNECT_CLIENT_SOCKET_TYPE_PCAP,
#endif
	JVX_CONNECT_CLIENT_SOCKET_TYPE_BTH
} jvx_connect_client_socket_type;

typedef enum
{
	JVX_CLIENT_ERROR_NO_ERROR = 0,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_FAILED = 1,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_SETOPT_I = 2,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_SETOPT_II = 3,
	JVX_CLIENT_ERROR_ID_RESOLVE_HOSTNAME = 4,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_BIND = 5,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_LISTEN = 6,
	JVX_CLIENT_ERROR_ID_CREATE_SOCKET_ACCEPT = 7
} jvx_connect_client_error_codes;

typedef enum
{
	JVX_CONNECT_CLIENT_STATE_NONE = 0,
	JVX_CONNECT_CLIENT_STATE_INIT = 1,
	JVX_CONNECT_CLIENT_STATE_STARTED = 2,
	JVX_CONNECT_CLIENT_STATE_CONNECTED = 3,
	JVX_CONNECT_CLIENT_STATE_WAIT_FOR_SHUTDOWN = 4
} jvx_connect_client_state;

typedef enum
{
	JVX_CONNECT_CLIENT_CONNECTION_SUCCESS = 0,
	JVX_CONNECT_CLIENT_CONNECTION_FAILED_TIMEOUT = 1,
	JVX_CONNECT_CLIENT_CONNECTION_FAILED_REFUSED = 2,
	JVX_CONNECT_CLIENT_CONNECTION_FAILED_ENDED = 3
} jvx_connect_client_fail_reason;

typedef struct
{
	int disableNagleAlgorithmTcp;
} jvx_connect_client_spec_tcp;

typedef sockaddr_in udpTarget_client;

typedef struct
{
	jvxUInt8 mac[6];
} jvx_connect_client_spec_pcap;

#ifdef OS_SUPPORTS_BTH
typedef struct
{
	const JVX_GUID* guidPtr;
} jvx_connect_client_spec_bth;
#endif

struct jvx_connect_client_if;
struct jvx_connect_client_family;

typedef struct
{
	jvxErrorType (*callback_report_startup_complete)(jvxHandle* privateData, jvxSize ifidx);

	jvxErrorType (*callback_provide_data_and_length)( jvxHandle* privateData, char**, jvxSize*, jvxSize ifidx);
	jvxErrorType (*callback_report_data_and_read)( jvxHandle* privateData, char*, jvxSize, const char* descr, jvxSize ifidx);

	jvxErrorType (*callback_report_unsuccesful_outgoing)( jvxHandle* privateData, jvx_connect_client_fail_reason reason, jvxSize ifidx);

	jvxErrorType (*callback_report_connect_outgoing)( jvxHandle* privateData, jvxSize ifidx);
	jvxErrorType (*callback_report_disconnect_outgoing)(jvxHandle* privateData, jvxSize ifidx);

	jvxErrorType (*callback_report_error)(jvxHandle* privateData, jvxInt32 errCode, const char* errDescription, jvxSize ifidx);

} jvx_connect_client_callback_type;


/** API FUNCTIONS **/
jvxErrorType jvx_connect_client_family_initialize_st(jvx_connect_client_family** hdlFamilyReturn, 
								   jvx_connect_client_socket_type socket_type);

jvxErrorType jvx_connect_client_family_number_interfaces(jvx_connect_client_family* hdlFamily, jvxSize* num);

jvxErrorType jvx_connect_client_family_name_interfaces(jvx_connect_client_family* hdlFamily, char* fld, jvxSize szFld, jvxSize idx);

jvxErrorType jvx_connect_client_family_description_interfaces(jvx_connect_client_family* hdlFamily, char* fld, jvxSize szFld, jvxSize idx);

jvxErrorType jvx_connect_client_family_terminate_st(jvx_connect_client_family* hdlFamily);

// ==================================================================================

jvxErrorType jvx_connect_client_interface_initialize_st(jvx_connect_client_family* hdlFamily,
										   jvx_connect_client_if** hdlClientOut, 
										   jvxHandle* config_specific,
										   jvx_connect_client_callback_type* report_callback,
										   jvxHandle* priv,
										   int timeout_ms, 
										   jvxSize idx);

jvxErrorType jvx_connect_client_interface_connect_st(jvx_connect_client_if* hdlClient, const char* config_target, int port);

jvxErrorType jvx_connect_client_set_thread_priority(jvx_connect_client_if* hdlClient, JVX_THREAD_PRIORITY prio);

jvxErrorType jvx_connect_client_field_constraint(jvx_connect_client_if* hdlClient, jvxSize* bytesPrepend, jvxSize* fldMinSize, jvxSize* fldMaxSize);

jvxErrorType jvx_connect_client_allocate_udp_target_st(jvx_connect_client_if* hdlClient, const char* server, int port, udpTarget_client* theTarget);

jvxErrorType jvx_connect_client_send_st(jvx_connect_client_if* hdlClient, char* buf, jvxSize* szFld, udpTarget_client* wheretosendto);

jvxErrorType jvx_connect_client_info_servername_port_st(jvx_connect_client_if* hdlClient, char* buf, jvxSize szFld, int* port);

jvxErrorType jvx_connect_client_trigger_shutdown_st(jvx_connect_client_if* hdlClient);

jvxErrorType jvx_connect_client_optional_wait_for_shutdown_st(jvx_connect_client_if* hdlClient);

jvxErrorType jvx_connect_client_interface_terminate_st(jvx_connect_client_if* hdlClient);

jvxErrorType jvx_connect_client_get_state_st(jvx_connect_client_if* hdlClient, jvx_connect_client_state* state_on_return);

jvxErrorType jvx_connect_client_lock_state(jvx_connect_client_if* hdlClient);

jvxErrorType jvx_connect_client_unlock_state(jvx_connect_client_if* hdlClient);

jvxErrorType jvx_connect_client_get_state(jvx_connect_client_if* hdlClient, jvx_connect_client_state* stateOnReturn);


#endif
