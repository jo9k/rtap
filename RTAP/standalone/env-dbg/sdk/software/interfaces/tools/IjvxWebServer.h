#ifndef __IJVXWEBSERVER_H__
#define __IJVXWEBSERVER_H__

JVX_INTERFACE IjvxWebServer_report
{
	
public:

	virtual JVX_CALLINGCONVENTION ~IjvxWebServer_report(){};
	
	// Callbacks for managing web server events
	virtual jvxErrorType JVX_CALLINGCONVENTION report_server_event(
					jvxHandle* context_server, jvxHandle* context_conn,
					jvxWebServerHandlerPurpose purp, jvxSize uniqueId, jvxBool strictConstConnection,
					int header = 0, char *payload = NULL, size_t szFld = 0) = 0;

};

JVX_INTERFACE IjvxWebServer: public IjvxObject, public IjvxHiddenInterface
{
	
public:

	virtual JVX_CALLINGCONVENTION ~IjvxWebServer(){};
	
	virtual jvxErrorType JVX_CALLINGCONVENTION initialize() = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION register_handler(jvxInt32* unique_handler_id, 
									jvxBitField purp, jvxHandle* identification, 									
									IjvxWebServer_report* bwd) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION start() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_request_url_specifier(jvxHandle* server, jvxHandle* conn, jvxString** str_oon_return) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_write_response(jvxHandle* server, jvxHandle* conn, const char* response_text) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_write_header_response(jvxHandle* server, jvxHandle* conn, jvxWebServerResponseType resp) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_write_packet_websocket(jvxHandle* server, jvxHandle* conn, int opcode, const jvxHandle* dField, jvxSize szField) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_read_content(jvxHandle* server, jvxHandle* conn, jvxHandle* readFld, jvxSize* numBytes) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_extract_var_content(const char* fldStringIn, jvxSize numBytesIn, const char* var_name, char* readFldOut, jvxSize* numBytesOut) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION in_connect_url_decode(const char *src, int src_len, jvxString** result) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_stop_single_websocket(jvxHandle* server, jvxHandle* conn) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_stop() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION wait_stop_complete() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unregister_handler(jvxSize unique_handler_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION terminate() = 0;

};

#endif