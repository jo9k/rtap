#ifndef _TJVXWEBSERVERHOST_H__
#define _TJVXWEBSERVERHOST_H__

typedef struct
{
	std::string expression;
	jvxBitField purp;
} jvxOneHttpRequestDefinition;

#define JVX_NUMBER_WEBSERVER_HOST_DEFINITIONS  5

static jvxOneHttpRequestDefinition theJvxWebServerHostDefinitions[JVX_NUMBER_WEBSERVER_HOST_DEFINITIONS] = 
{
	{"/jvx/host/general/", JVX_WEB_SERVER_URI_POST_HANDLER},
	{"/jvx/host/components/", JVX_WEB_SERVER_URI_POST_HANDLER},
	{"/jvx/host/sequencer/", JVX_WEB_SERVER_URI_POST_HANDLER},
	{"/jvx/host/properties/", JVX_WEB_SERVER_URI_POST_HANDLER},
	{"/jvx/host/socket/", (JVX_WEB_SERVER_URI_WEBSOCKET_CONNECT_HANDLER | JVX_WEB_SERVER_URI_WEBSOCKET_READY_HANDLER | JVX_WEB_SERVER_URI_WEBSOCKET_DATA_HANDLER | JVX_WEB_SERVER_URI_WEBSOCKET_CLOSE_HANDLER)}
};

typedef jvxInt32 theJvxWebServerHostIds[JVX_NUMBER_WEBSERVER_HOST_DEFINITIONS];

#endif

