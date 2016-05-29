#ifndef __CJVXWEBSERVERHOST_H__
#define __CJVXWEBSERVERHOST_H__

#include "templates/common/TjvxWebServerHost.h"
#include "helpers/HjvxHostsAndProperties.h"
#include <iostream>

#ifndef JVX_NUMBER_DIGITS_FLOAT
#define JVX_NUMBER_DIGITS_FLOAT 4
#endif

/*
#define JVX_NUMBER_CHAR_REPLACE 3

static char JVX_CHAR_REPLACE_THIS[JVX_NUMBER_CHAR_REPLACE] =
{
' ', '+', '-'
};

static const char* JVX_CHAR_REPLACE_BY[JVX_NUMBER_CHAR_REPLACE] =
{
"_", "_", "_"
};
*/

class IjvxWebServerHost_hooks
{
public:

	~IjvxWebServerHost_hooks(){};

	virtual jvxErrorType synchronizeWebServerEvents(jvxHandle* context_server, jvxHandle* context_conn, jvxWebServerHandlerPurpose purp, jvxSize uniqueId,
		jvxBool strictConstConnection, int header, char *payload, size_t szFld) = 0;

	virtual jvxErrorType hook_safeConfigFile() = 0;
	virtual jvxErrorType hook_startSequencer() = 0;
	virtual jvxErrorType hook_stopSequencer() = 0;

	virtual jvxErrorType hook_operationComplete() = 0;

	// If support for web sockets...
/*	virtual jvxErrorType hook_wsConnect(jvxHandle* context_server, jvxHandle* context_conn){return(JVX_ERROR_UNSUPPORTED);};
	virtual jvxErrorType hook_wsReady(jvxHandle* context_server, jvxHandle* context_conn){return(JVX_ERROR_UNSUPPORTED);};
	virtual jvxErrorType hook_wsData(jvxHandle* context_server, jvxHandle* context_conn, int header, char *payload, size_t szFld){return(JVX_ERROR_UNSUPPORTED);};
	virtual jvxErrorType hook_wsClose(jvxHandle* context_server, jvxHandle* context_conn){return(JVX_ERROR_UNSUPPORTED);};
	*/
	virtual jvxErrorType hook_ws_started() = 0;
	virtual jvxErrorType hook_ws_stopped() = 0;
};



class CjvxWebServerHost: public IjvxWebServer_report
{
private:

	IjvxWebServer* hdl;
	IjvxHost* hostHdl;

	struct
	{
		theJvxWebServerHostIds theIds;
	} notifyIds;

	IjvxWebServerHost_hooks* theHooks;

	typedef struct
	{
		std::string prop_descr;
		jvxComponentType selector[1];
		jvxInt32 idClient;
		jvxBool validPropertyHandle;
		jvxPropertyDescriptor descriptorPropertyHandle;
		jvxHandle* bufSend;
		jvxSize bufSize;
	} oneWsPropertyStream;

	typedef struct
	{
		jvxHandle* theServer;
		jvxHandle* selector[1];
		jvxBool isConnected;
		std::vector<oneWsPropertyStream> theStreams;
	} oneWsConnection;

	struct
	{		
		jvxSize uniqueId;
		std::vector<oneWsConnection> openConnectionsWs;
	} theWebserver;
public:
	CjvxWebServerHost()
	{
		jvxSize i;
		hdl = NULL;
		for(i = 0; i < JVX_NUMBER_WEBSERVER_HOST_DEFINITIONS; i++)
		{
			notifyIds.theIds[i] = -1;
		}
		theHooks = NULL;
		theWebserver.uniqueId = 1;
	};

	jvxErrorType start_webserver(IjvxWebServer* server, IjvxHost* hostRef,
		IjvxWebServerHost_hooks* theHooksPass = NULL)
	{
		jvxSize i;
		jvxErrorType resL = JVX_NO_ERROR;
		if((hdl == NULL) && (server))
		{
			hdl = server;
			hostHdl = hostRef;

			resL = hdl->initialize();

			if(resL == JVX_NO_ERROR)
			{
				// Set synchronous reference
				theHooks = theHooksPass;

				// Regular expression match
				for(i = 0; i < JVX_NUMBER_WEBSERVER_HOST_DEFINITIONS; i++)
				{

					resL = hdl->register_handler(&notifyIds.theIds[i],
						theJvxWebServerHostDefinitions[i].purp,
						(jvxHandle*)theJvxWebServerHostDefinitions[i].expression.c_str(),
						static_cast<IjvxWebServer_report*>(this));
					assert(resL == JVX_NO_ERROR);
				}
				resL = hdl->start();
			}
		}
		return resL;
	};

	jvxErrorType stop_webserver()
	{
		jvxErrorType resL = JVX_NO_ERROR;
		if(hdl)
		{
			resL = hdl->wait_stop_complete();
			assert(resL == JVX_NO_ERROR);

			resL = hdl->terminate();
			assert(resL == JVX_NO_ERROR);

			hdl = NULL;
		}
		return resL;
	};

	jvxErrorType report_server_event(
		jvxHandle* context_server, jvxHandle* context_conn,
		jvxWebServerHandlerPurpose purp, jvxSize uniqueId,
		jvxBool strictConstConnection, int header, char *payload, size_t szFld)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(theHooks)
		{
			res = theHooks->synchronizeWebServerEvents(context_server, context_conn, purp, uniqueId,
				strictConstConnection, header, payload, szFld);
		}
		else
		{
			res = report_event_request_core(context_server, context_conn, purp, uniqueId,
				strictConstConnection, header, payload, szFld);
		}
		return(res);
	};

	jvxErrorType report_event_request_core(
		jvxHandle* context_server, jvxHandle* context_conn,
		jvxWebServerHandlerPurpose purp, jvxSize uniqueId,
		jvxBool strictConstConnection, int header, char *payload, size_t szFld)

	{
		jvxErrorType res = JVX_NO_ERROR, resL = JVX_NO_ERROR;
		jvxString* fldStr = NULL;
		char oneToken[JVX_MAXSTRING];

		std::string response;
		std::string in_params;
		std::string purpose;
		std::string comp_target;
		std::string descriptor;
		std::string stateSwitch;
		std::string value;

		std::string contentonly = "no";
		jvxComponentType cpType = JVX_COMPONENT_UNKNOWN;
		jvxSize sz;
		jvxStateSwitch operTp = JVX_STATE_SWITCH_NONE;
		jvxInt32 idx = -1;
		std::string url;


		hdl->in_connect_request_url_specifier(context_server, context_conn, &fldStr);
		if(fldStr)
		{
			url = fldStr->bString;
			hdl->deallocate(fldStr);
		}
		switch(purp)
		{
		case JVX_WEB_SERVER_URI_GET_HANDLER:
			if(uniqueId == notifyIds.theIds[0])
			{
				// Address /jvx/host/general/
			}
			else if(uniqueId == notifyIds.theIds[1])
			{
				// Address /jvx/host/components/
			}
			else if(uniqueId == notifyIds.theIds[2])
			{
				// Address /jvx/host/sequencer/
			}
			else if(uniqueId == notifyIds.theIds[3])
			{
				// Address /jvx/host/properties/
			}
			else
			{
				// Unmatched
			}
			break;
		case JVX_WEB_SERVER_URI_POST_HANDLER:

			resL = JVX_NO_ERROR;
			while(resL == JVX_NO_ERROR)
			{
				memset(oneToken, 0, JVX_MAXSTRING);
				sz = JVX_MAXSTRING-1;
				resL = hdl->in_connect_read_content(context_server, context_conn, oneToken, &sz);
				in_params += oneToken;
			}
			if(!in_params.empty())
			{
				purpose = this->extractStringParameters(in_params, "purpose");
				if(uniqueId == notifyIds.theIds[0])
				{
					// Address /jvx/host/general/
					if(purpose == "state")
					{
						this->response_host__state(context_server, context_conn);
						break;
					}
					if(purpose == "safe_config")
					{
						this->response_host__safe_config(context_server, context_conn);
						break;
					}
					std::cout << "Failed to match input parameters " << in_params << "!" << std::endl;
				}
				else if(uniqueId == notifyIds.theIds[1])
				{
					// Address /jvx/host/components/
					comp_target = extractStringParameters(in_params, "component_target");
					componentTargetStringToComponentType(comp_target, cpType);
					if(purpose == "description")
					{
						this->response_components__description(context_server, context_conn, cpType);
						break;
					}
					if(purpose == "state")
					{
						this->response_components__state(context_server, context_conn, cpType);
						break;
					}
					if(purpose == "switch_state")
					{
						stateSwitch = extractStringParameters(in_params, "operation");
						operationStringToStateSwitchType(stateSwitch, operTp);
						this->response_components__switch_state(context_server, context_conn, cpType, operTp, idx);
						break;
					}
					std::cout << "Failed to match input parameters " << in_params << "!" << std::endl;
				}
				else if(uniqueId == notifyIds.theIds[2])
				{
					// Address /jvx/host/sequencer/
					if(purpose == "state")
					{
						this->response_sequencer__start(context_server, context_conn);
						break;
					}
					if(purpose == "start")
					{
						this->response_sequencer__start(context_server, context_conn);
						break;
					}
					if(purpose == "stop")
					{
						this->response_sequencer__stop(context_server, context_conn);
						break;
					}
					std::cout << "Failed to match input parameters " << in_params << "!" << std::endl;
				}
				else if(uniqueId == notifyIds.theIds[3])
				{
					comp_target = extractStringParameters(in_params, "component_target");
					componentTargetStringToComponentType(comp_target, cpType);

					if(purpose == "description")
					{
						this->response_properties__description(context_server, context_conn, cpType);
						break;
					}
					if(purpose == "get-content")
					{
						descriptor = extractStringParameters(in_params, "property_descriptor");
						contentonly = extractStringParameters(in_params, "content_only");
						this->response_properties__get_content(context_server, context_conn, cpType, descriptor, (contentonly == "yes"));
						break;
					}
					if(purpose == "set-content")
					{
						descriptor = extractStringParameters(in_params, "property_descriptor");
						contentonly = extractStringParameters(in_params, "content_only");
						value = extractStringParameters(in_params, "value");
						this->response_properties__set_content(context_server, context_conn, cpType, descriptor, value);
					}
					std::cout << "Failed to match input parameters " << in_params << "!" << std::endl;
				}
				else
				{
					// Unmatched
				}
			}
			break;
		case JVX_WEB_SERVER_URI_WEBSOCKET_CONNECT_HANDLER:
			res = hook_wsConnect(context_server, context_conn);
			break;
		case JVX_WEB_SERVER_URI_WEBSOCKET_DATA_HANDLER:
			res = hook_wsData(context_server, context_conn,header, payload, szFld);
			break;
		case JVX_WEB_SERVER_URI_WEBSOCKET_READY_HANDLER:
			res = hook_wsReady(context_server, context_conn);
			break;
		case JVX_WEB_SERVER_URI_WEBSOCKET_CLOSE_HANDLER:
			res = hook_wsClose(context_server, context_conn);
			break;
		default:
			assert(0);
		}			
		return(JVX_NO_ERROR);
	};

	// Web socket handling	
	jvxErrorType hook_wsConnect(jvxHandle* context_server, jvxHandle* context_conn)
	{
		// We should arrive here within the QT thread, therefore no threading problem!!
		oneWsConnection theConn;
		theConn.isConnected = false;
		theConn.selector[0] = context_conn;
		theConn.theServer = context_server;

		if(this->theWebserver.openConnectionsWs.size() == 0)
		{
			if(theHooks)
			{
				theHooks->hook_ws_started();
			}
		}
		this->theWebserver.openConnectionsWs.push_back(theConn);
		return(JVX_NO_ERROR);
	};
	
	jvxErrorType hook_wsReady(jvxHandle* context_server, jvxHandle* context_conn)
	{
		jvxSize i;

		std::vector<oneWsConnection>::iterator elm = jvx_findItemSelectorInList<oneWsConnection,jvxHandle*>(theWebserver.openConnectionsWs, context_conn, 0);
		if(elm != theWebserver.openConnectionsWs.end())
		{
			elm->isConnected = true;
		}
		else
		{
			std::cout << "Unexpected problem in mainCentral::hook_wsReady: unknown confirmed connection handle" << std::endl;
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType hook_wsData(jvxHandle* context_server, jvxHandle* context_conn, int flags, char *payload, size_t szFld)
	{
		jvxErrorType res = JVX_NO_ERROR;
		char fldIn[JVX_MAXSTRING];
		jvxSize numcopy = 0;
		jvxSize i;
		std::string resp;
		std::string command;
		std::string f_expr;
		std::vector<std::string> args;
		std::string prop_descr;
		jvxComponentType tp = JVX_COMPONENT_UNKNOWN;
		jvxBool startit = false;
		jvxBool stopit = false;
		jvxSize idRet = 0;

		// We should find (http://tools.ietf.org/html/rfc6455):
		// - (flags & 0x80) == 1
		// - flags &= 0x7f => Opcode
		// Translations in Opcode meaning -> jvxWebServerWebSocketFrameType in TjvxWebServer.h
		if(flags & 0x80)
		{
			jvxInt8 opcode = (jvxInt8)(flags & 0x7f);
			switch(opcode)
			{
			case JVX_WEBSOCKET_OPCODE_CONNECTION_CLOSE:
				res = JVX_ERROR_ABORT;
				break;
			case JVX_WEBSOCKET_OPCODE_TEXT:
				memset(fldIn, 0, JVX_MAXSTRING);
				numcopy = JVX_MIN(szFld, JVX_MAXSTRING-1);
				if(numcopy == szFld)
				{
					memcpy(fldIn,payload, numcopy);
					command = fldIn;

					jvx_parseCommandIntoToken(command, f_expr, args);
					if(f_expr == "keepalive")
					{
						// This message is ok, respond with "ok"
						resp = "ok:keepalive";
					}
					else
					{
						if(args.size() == 2)
						{
							// Find component type
							for(i = 0; i < JVX_COMPONENT_LIMIT; i++)
							{
								std::string compTo = jvxComponentType_txt(i);
								if(args[0] == compTo)
								{
									tp = (jvxComponentType)i;
									prop_descr = args[1];
									break;
								}
							}
						}

						if(tp != JVX_COMPONENT_UNKNOWN)
						{
							if(f_expr == "start")
							{
								startit = true;
							}
							else if(f_expr == "stop")
							{
								stopit = true;
							}
							else
							{
								res = JVX_ERROR_INVALID_ARGUMENT;
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}

						if(res == JVX_NO_ERROR)
						{
							if(startit)
							{
								std::vector<oneWsConnection>::iterator elm = jvx_findItemSelectorInList<oneWsConnection,jvxHandle*>(theWebserver.openConnectionsWs, context_conn, 0);
								if(elm != theWebserver.openConnectionsWs.end())
								{
									std::vector<oneWsPropertyStream>::iterator elmT = jvx_findItemSelectorInList<oneWsPropertyStream,jvxComponentType>(elm->theStreams, tp, 0);
									if(elmT == elm->theStreams.end())
									{
										oneWsPropertyStream newStream;
										newStream.prop_descr = prop_descr;
										newStream.selector[0] = tp;
										newStream.idClient = theWebserver.uniqueId;
										theWebserver.uniqueId++;
										idRet = newStream.idClient;
										newStream.validPropertyHandle = false;
										jvx_initPropertyDescription(newStream.descriptorPropertyHandle);
										newStream.bufSend = NULL;
										newStream.bufSize = 0;

										elm->theStreams.push_back(newStream);
									}
									else
									{
										res = JVX_ERROR_DUPLICATE_ENTRY;
									}
								}
								else
								{
									res = JVX_ERROR_ELEMENT_NOT_FOUND;
								}
							}

							if(stopit)
							{
								std::vector<oneWsConnection>::iterator elm = jvx_findItemSelectorInList<oneWsConnection,jvxHandle*>(theWebserver.openConnectionsWs, context_conn, 0);
								if(elm != theWebserver.openConnectionsWs.end())
								{
									std::vector<oneWsPropertyStream>::iterator elmT = jvx_findItemSelectorInList<oneWsPropertyStream,jvxComponentType>(elm->theStreams, tp, 0);
									if(elmT != elm->theStreams.end())
									{
										JVX_SAFE_DELETE_FLD(elmT->bufSend, jvxByte);
										elmT->bufSize = 0;
										idRet = elmT->idClient;
										elm->theStreams.erase(elmT);
									}
									else
									{
										res = JVX_ERROR_ELEMENT_NOT_FOUND;
									}
								}
								else
								{
									res = JVX_ERROR_ELEMENT_NOT_FOUND;
								}
							}
						}

						if(res == JVX_NO_ERROR)
						{
							resp = "ok:" + jvx_int2String(idRet);
						}
						else
						{
							resp = "error: " + jvxErrorType_descr(res);
							res = JVX_NO_ERROR; // Keep link open
						}
					}// else: if(command == "keepalive")
					if(res == JVX_NO_ERROR)
					{
						hdl->in_connect_write_packet_websocket(context_server, context_conn, JVX_WEBSOCKET_OPCODE_TEXT, reinterpret_cast<const jvxHandle*>(resp.c_str()), resp.size());
					}
				}
				else
				{
					std::cout << "mainCentral::hook_wsData: Too many input characters, skipping message" << std::endl;
					res = JVX_ERROR_SIZE_MISMATCH;
				}


				break;
			default:
				// Do nothing
				break;
			}
		}

		// Do nothing, just return the info to keep 
		return(res);
	};

	jvxErrorType hook_wsClose(jvxHandle* context_server, jvxHandle* context_conn)
	{
		jvxSize j;
		std::vector<oneWsConnection>::iterator elm = this->theWebserver.openConnectionsWs.begin();
		elm = jvx_findItemSelectorInList<oneWsConnection, jvxHandle*>(theWebserver.openConnectionsWs, context_conn, 0);
		if(elm != this->theWebserver.openConnectionsWs.end())
		{
			// Remove all fields present in this connection
			for(j = 0; j < elm->theStreams.size(); j++)
			{

				if(elm->theStreams[j].bufSend)
				{
					JVX_SAFE_DELETE_FLD(elm->theStreams[j].bufSend, jvxByte);
					elm->theStreams[j].bufSize = 0;
				}
			}
			this->theWebserver.openConnectionsWs.erase(elm);
		}

		if(this->theWebserver.openConnectionsWs.size() == 0)
		{
			if(theHooks)
			{
				theHooks->hook_ws_stopped();
			}
		}

		return(JVX_NO_ERROR);
	};

	void postPropertyStreams()
	{
		jvxSize i,j;
		jvxErrorType resL = JVX_NO_ERROR;
		jvxBool allocateFieldNew = false;
		jvxBool secondPollProperty = false;
		jvxBool sendContent = false;
		jvxBool isValid = false;
		jvxBool obtainDescriptionProp = false;

		//	if(!theWebserver.closeProcedureState == 0)
		//	{
		for(i = 0; i < this->theWebserver.openConnectionsWs.size(); i++)
		{
			if(this->theWebserver.openConnectionsWs[i].isConnected)
			{
				// Post all properties
				for(j = 0; j < this->theWebserver.openConnectionsWs[i].theStreams.size(); j++)
				{
					allocateFieldNew = false;
					secondPollProperty = false;
					sendContent = false;
					isValid = false;
					obtainDescriptionProp = false;
					if(this->theWebserver.openConnectionsWs[i].theStreams[j].validPropertyHandle)
					{
						isValid = false;
						resL = jvx_get_property(hostHdl, 
							((jvxByte*)this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend + sizeof(jvxPropStreamProtocolHeader)),
							0,
							this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.num,
							this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.format,
							true,
							this->theWebserver.openConnectionsWs[i].theStreams[j].selector[0],
							this->theWebserver.openConnectionsWs[i].theStreams[j].prop_descr.c_str(),
							&isValid);

						if(resL == JVX_NO_ERROR)
						{
							if(isValid)
							{
								sendContent = true;								
							}
						}
						else
						{
							obtainDescriptionProp = true;
						}
					}
					else
					{
						obtainDescriptionProp = true;
					}


					if(obtainDescriptionProp)
					{
						// IjvxHost* theHost, jvxPropertyDescriptor& theStr, jvxComponentType tp, const char* descriptor)
						resL = jvx_getPropertyDescription(hostHdl, 
							this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle,
							this->theWebserver.openConnectionsWs[i].theStreams[j].selector[0],
							this->theWebserver.openConnectionsWs[i].theStreams[j].prop_descr.c_str());
						if(resL == JVX_NO_ERROR)
						{
							switch(this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.format)
							{
							case JVX_DATAFORMAT_8BIT:
							case JVX_DATAFORMAT_16BIT_LE:
							case JVX_DATAFORMAT_32BIT_LE:
							case JVX_DATAFORMAT_24BIT_LE:
							case JVX_DATAFORMAT_64BIT_LE:
							case JVX_DATAFORMAT_DATA:
								allocateFieldNew = true;
								break;
							default:
								break;
							}
						}

						if(allocateFieldNew)
						{
							this->theWebserver.openConnectionsWs[i].theStreams[j].validPropertyHandle = true;
							if(this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend)
							{
								JVX_SAFE_DELETE_FLD(this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend, jvxByte);
								this->theWebserver.openConnectionsWs[i].theStreams[j].bufSize = 0;
							}
							this->theWebserver.openConnectionsWs[i].theStreams[j].bufSize = sizeof(jvxPropStreamProtocolHeader) + 
								this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.num * 
								jvxDataFormat_size[this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.format];
							JVX_SAFE_NEW_FLD(this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend, jvxByte, this->theWebserver.openConnectionsWs[i].theStreams[j].bufSize);
							memset(this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend, 0, sizeof(jvxByte)* this->theWebserver.openConnectionsWs[i].theStreams[j].bufSize);
							jvxPropStreamProtocolHeader* header = (jvxPropStreamProtocolHeader*)this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend;
							header->fam_hdr.proto_family = JVX_PROTOCOL_TYPE_PROPERTY_STREAM;
							header->propid = this->theWebserver.openConnectionsWs[i].theStreams[j].idClient;
							header->dataformat = this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.format;
							header->numberElements = this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.num;
							header->offsetElements = 0;
							secondPollProperty = true;
							sendContent = true;
						}

						if(secondPollProperty)
						{
							isValid = false;
							resL = jvx_get_property(hostHdl, 
								((jvxByte*)this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend + sizeof(jvxPropStreamProtocolHeader)),
								0,
								this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.num,
								this->theWebserver.openConnectionsWs[i].theStreams[j].descriptorPropertyHandle.format,
								true,
								this->theWebserver.openConnectionsWs[i].theStreams[j].selector[0],
								this->theWebserver.openConnectionsWs[i].theStreams[j].prop_descr.c_str(),
								&isValid);
							if(resL == JVX_NO_ERROR)
							{
								if(isValid)
								{
									sendContent = true;
								}
								else
								{
									this->theWebserver.openConnectionsWs[i].theStreams[j].validPropertyHandle = true;
								}
							}
							else
							{
								this->theWebserver.openConnectionsWs[i].theStreams[j].validPropertyHandle = true;
							}
						}
					}
					if(sendContent)
					{
						hdl->in_connect_write_packet_websocket(
							this->theWebserver.openConnectionsWs[i].theServer,
							this->theWebserver.openConnectionsWs[i].selector[0],
							JVX_WEBSOCKET_OPCODE_BINARY, 
							this->theWebserver.openConnectionsWs[i].theStreams[j].bufSend, 
							this->theWebserver.openConnectionsWs[i].theStreams[j].bufSize);
					}
				}// for(j = 0; j < this->theWebserver.openConnectionsWs[i].theStreams.size(); j++)
			}
		}
		//}
	};

	// ======================================================================

	jvxErrorType componentTargetStringToComponentType(std::string componentToken, jvxComponentType& cpType)
	{
		jvxSize i;
		cpType = JVX_COMPONENT_UNKNOWN;
		for(i = 0; i < JVX_COMPONENT_LIMIT; i++)
		{
			if(jvxComponentType_str[i] == componentToken)
			{
				cpType = (jvxComponentType) i;
				return JVX_NO_ERROR;
			}
		}
		return JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	jvxErrorType operationStringToStateSwitchType(std::string operToken, jvxStateSwitch& ssType)
	{
		jvxSize i;
		ssType = JVX_STATE_SWITCH_NONE;
		for(i = 0; i < JVX_STATE_SWITCH_LIMIT; i++)
		{
			if(jvxStateSwitch_str[i] == operToken)
			{
				ssType = (jvxStateSwitch) i;
				return JVX_NO_ERROR;
			}
		}
		return JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	void writeResponseLeadIn(jvxHandle* server, jvxHandle* conn, jvxBool callResult = true, std::string err = "--")
	{
		std::string token;

		// ****
		token = "{\"result\":";
		hdl->in_connect_write_response(server, conn, token.c_str());
		token.clear();

		if(callResult)
		{
			token = "\"ok\"";
		}
		else
		{
			token = "\"err\"";
		}
		hdl->in_connect_write_response(server, conn, token.c_str());
		token.clear();

		token = ",\"err\":\"" + err + "\"";
		hdl->in_connect_write_response(server, conn, token.c_str());
		token.clear();
	};

	void writeResponseLeadOut(jvxHandle* server, jvxHandle* conn)
	{
		std::string token;

		// ****
		token = "}";
		hdl->in_connect_write_response(server, conn, token.c_str());
		token.clear();
	};

	// ======================================================================

	// +++

	virtual jvxErrorType response_host__state(jvxHandle* server, jvxHandle* conn)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		if(hostHdl)
		{
			res = hostHdl->state(&stat);

			hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

			if(res == JVX_NO_ERROR)
			{
				writeResponseLeadIn(server, conn, true, "--");
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to get state from host, error type " + jvxErrorType_descr(res));
			}

			token = ",\"state\":\"" + jvxState_txt(stat) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			writeResponseLeadOut(server, conn);
		}

		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_host__safe_config(jvxHandle* server, jvxHandle* conn)
	{
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		if(hostHdl)
		{
			if(theHooks)
			{
				res = theHooks->hook_safeConfigFile();

				hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

				if(res == JVX_NO_ERROR)
				{
					writeResponseLeadIn(server, conn, true, "--");
				}
				else
				{
					writeResponseLeadIn(server, conn, false, "Failed to get state from host");
				}

				writeResponseLeadOut(server, conn);
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to get state from host");

				writeResponseLeadOut(server, conn);
			}
		}
		return JVX_NO_ERROR;
	};

	// +++

	virtual jvxErrorType response_components__description(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		jvxSize sz = 0;
		jvxSize i;
		jvxString* fldStr = NULL;

		if(hostHdl)
		{
			res = hostHdl->number_components(compTp, &sz);

			hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

			if(res == JVX_NO_ERROR)
			{
				writeResponseLeadIn(server, conn, true, "--");
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to get number of components from host, error type " + jvxErrorType_descr(res));
			}

			token = ", \"component_target\":\"" + jvxComponentType_txt(compTp) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			token = ", \"components\":";
			token += "[";
			hdl->in_connect_write_response(server, conn, token.c_str());

			for(i = 0; i < sz; i++)
			{
				token = "";
				hostHdl->description_component(compTp, i, &fldStr);
				if(fldStr)
				{
					token = fldStr->bString;
					hostHdl->deallocate(fldStr);
					fldStr = NULL;
				}
				if(i > 0)
				{
					token = ", \"" + token + "\"";
				}
				else
				{
					token = "\"" + token + "\"";
				}
				hdl->in_connect_write_response(server, conn, token.c_str());
			}

			token = "]";
			hdl->in_connect_write_response(server, conn, token.c_str());

			writeResponseLeadOut(server, conn);
		}

		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_components__state(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		if(hostHdl)
		{
			res = hostHdl->state_component(compTp, &stat);

			hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

			if(res == JVX_NO_ERROR)
			{
				writeResponseLeadIn(server, conn, true, "--");
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to get state for componentfrom host, error type " + jvxErrorType_descr(res));
			}

			token = ", \"component_target\":\"" + jvxComponentType_txt(compTp) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			token = ", \"state\":\"" + jvxState_txt(stat) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			writeResponseLeadOut(server, conn);
		}

		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_components__switch_state(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp, jvxStateSwitch operation, jvxInt32 idx)
	{
		//jvxStateSwitch_str
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		jvxSize sz = 0;
		if(hostHdl)
		{
			switch(operation)
			{
			case JVX_STATE_SWITCH_SELECT:
				res = hostHdl->number_components(compTp, &sz);
				if((idx >= 0) && (idx < sz))
				{
					res = hostHdl->select_component(compTp, idx);
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
				break;
			case JVX_STATE_SWITCH_ACTIVATE:
				res = hostHdl->activate_component(compTp);
				break;
			case JVX_STATE_SWITCH_DEACTIVATE:
				res = hostHdl->deactivate_component(compTp);
				break;
			case JVX_STATE_SWITCH_UNSELECT:
				res = hostHdl->unselect_component(compTp);
				break;
			default:
				res = JVX_ERROR_UNSUPPORTED;
				break;
			}

			hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

			if(res == JVX_NO_ERROR)
			{
				writeResponseLeadIn(server, conn, true, "--");
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to switch state for component, error type " + jvxErrorType_descr(res));
			}

			token = ",\"component_target\":\"" + jvxComponentType_txt(compTp) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			writeResponseLeadOut(server, conn);
		}

		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_sequencer__state(jvxHandle* server, jvxHandle* conn)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		IjvxSequencer* theSeq = NULL;
		jvxSequencerStatus stats = JVX_SEQUENCER_STATUS_NONE;
		jvxInt32 idxSeq = 0;
		jvxSequencerQueueType qTp;
		jvxInt32 idxSt = 0;
		jvxBool loop = false;

		if(hostHdl)
		{
			res = hostHdl->request_hidden_interface(JVX_INTERFACE_SEQUENCER, reinterpret_cast<jvxHandle**>(&theSeq));
			if((res == JVX_NO_ERROR) && theSeq)
			{
				res = theSeq->status_process(&stats, &idxSeq, &qTp, &idxSt, &loop);
				hostHdl->return_hidden_interface(JVX_INTERFACE_SEQUENCER, reinterpret_cast<jvxHandle*>(theSeq));
			}

			hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);
			if(res == JVX_NO_ERROR)
			{
				writeResponseLeadIn(server, conn, true, "--");
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to obtain sequencer status, error type " + jvxErrorType_descr(res));
			}

			token = ",\"status\":";
			token += "\"" + jvxSequencerStatus_str[stats] + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			token = ",\"seq_id\":";
			token += "\"" + jvx_int2String(idxSeq) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			token = ",\"queue_tp\":";
			token += "\"" + jvxSequencerQueueType_str[stats] + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			token = ",\"step_id\":";
			token += "\"" + jvx_int2String(idxSt) + "\"";
			hdl->in_connect_write_response(server, conn, token.c_str());

			writeResponseLeadOut(server, conn);
		}
		return JVX_NO_ERROR;
	};

	// +++

	virtual jvxErrorType response_sequencer__start(jvxHandle* server, jvxHandle* conn)
	{
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		if(hostHdl)
		{
			if(theHooks)
			{
				res = theHooks->hook_startSequencer();

				hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

				if(res == JVX_NO_ERROR)
				{
					writeResponseLeadIn(server, conn, true, "--");
				}
				else
				{
					writeResponseLeadIn(server, conn, false, "Failed to start sequencer");
				}

				writeResponseLeadOut(server, conn);
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "No jook provided for start of sequencer");

				writeResponseLeadOut(server, conn);
			}
		}
		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_sequencer__stop(jvxHandle* server, jvxHandle* conn)
	{
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		if(hostHdl)
		{
			if(theHooks)
			{
				res = theHooks->hook_stopSequencer();

				hdl->in_connect_write_header_response(server, conn, JVX_WEB_SERVER_RESPONSE_JSON);

				if(res == JVX_NO_ERROR)
				{
					writeResponseLeadIn(server, conn, true, "--");
				}
				else
				{
					writeResponseLeadIn(server, conn, false, "Failed to stop sequencer");
				}

				writeResponseLeadOut(server, conn);
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "No jook provided for start of sequencer");

				writeResponseLeadOut(server, conn);
			}
		}
		return JVX_NO_ERROR;
	};

	// +++

	virtual jvxErrorType response_properties__description(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;
		jvxSize sz = 0, i = 0;
		jvxPropertyCategoryType category = JVX_PROPERTY_CATEGORY_UNKNOWN;
		jvxUInt64 settype = 0;
		jvxUInt64 stateAccess = 0;
		jvxDataFormat format = JVX_DATAFORMAT_NONE;
		jvxSize num = 0;
		jvxPropertyAccessType accessType = JVX_PROPERTY_ACCESS_WRITE_ONLY;
		jvxPropertyDecoderHintType decTp = JVX_PROPERTY_DECODER_NONE;
		jvxSize globalIdx = 0;
		jvxPropertyContext ctxt = JVX_PROPERTY_CONTEXT_UNKNOWN;
		jvxString* name = NULL;
		jvxString* description = NULL;
		jvxString* descriptor = NULL;
		jvxBool isValid = false;

		jvx_propertyReferenceTriple theTriple;
		jvx_initPropertyReferenceTriple(&theTriple);

		if(hostHdl)
		{
			res = jvx_getReferencePropertiesObject(hostHdl, &theTriple, compTp);
			if(res == JVX_NO_ERROR)
			{
				if(jvx_isValidPropertyReferenceTriple(&theTriple))
				{
					writeResponseLeadIn(server, conn, true, "--");

					token = ",\"component_target\":\"" + jvxComponentType_txt(compTp) + "\"";
					hdl->in_connect_write_response(server, conn, token.c_str());

					theTriple.theProps->number_properties(&sz);
					token = "\"properties\":[";
					hdl->in_connect_write_response(server, conn, token.c_str());

					for(i = 0; i < sz; i++)
					{
						token = "{";
						hdl->in_connect_write_response(server, conn, token.c_str());

						theTriple.theProps->description_property(i,
							&category,&settype,&stateAccess,&format,
							&num,&accessType,&decTp,&globalIdx,
							&ctxt,&name,&description,&descriptor, &isValid);

						token = "\"name\":\"";
						token += name->bString;
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "\"description\":\"";
						token += description->bString;
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "\"descriptor\":\"";
						token += descriptor->bString;
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						theTriple.theObj->deallocate(name);
						theTriple.theObj->deallocate(description);
						theTriple.theObj->deallocate(descriptor);

						token = "\"format\":\"";
						token += jvxDataFormat_str[format];
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "\"number\":\"";
						token += jvx_int2String(num);
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "\"isvalid\":\"";
						if(isValid)
						{
							token += "yes";
						}
						else
						{
							token += "no";
						}
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "\"decoder_type\":\"";
						token += jvxDecoderHintType_str[decTp];
						token += "\"";
						hdl->in_connect_write_response(server, conn, token.c_str());

						token = "}";
						hdl->in_connect_write_response(server, conn, token.c_str());
					}
					token = "]";
					hdl->in_connect_write_response(server, conn, token.c_str());

					writeResponseLeadOut(server, conn);
				}
				else
				{
					writeResponseLeadIn(server, conn, false, "Failed to obtain valid property handle reference");
					writeResponseLeadOut(server, conn);
				}

				jvx_returnReferencePropertiesObject(hostHdl, &theTriple, compTp);
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to obtain valid property handle reference");
				writeResponseLeadOut(server, conn);
			}
		}
		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_properties__get_content(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp, std::string descriptor, jvxBool contentOnly)
	{
		jvxState stat = JVX_STATE_NONE;
		std::string token;
		jvxErrorType res = JVX_NO_ERROR;

		jvxPropertyCategoryType category = JVX_PROPERTY_CATEGORY_UNKNOWN;
		jvxDataFormat format = JVX_DATAFORMAT_NONE;
		jvxSize num = 0, sz = 0;
		jvxSize globalIdx = 0;
		jvxBool isValid = true;
		jvxSize i;

		jvxByte* copyPtr = NULL;
		jvxSelectionList lst;
		lst.strList = NULL;
		lst.bitFieldSelected = 0;
		lst.bitFieldExclusive = 0;

		jvxString* fldStr = NULL;
		jvxValueInRange fldValRange;

		jvx_propertyReferenceTriple theTriple;
		jvx_initPropertyReferenceTriple(&theTriple);

		if(hostHdl)
		{
			res = jvx_getReferencePropertiesObject(hostHdl, &theTriple, compTp);
			if(res == JVX_NO_ERROR)
			{
				res = theTriple.theProps->description_property(
					descriptor.c_str(),
					&format,
					&num,
					&globalIdx,
					&category);
				if(res == JVX_NO_ERROR)
				{
					res = theTriple.theProps->number_properties(&sz);
					if(res == JVX_NO_ERROR)
					{
						switch(format)
						{
						case JVX_DATAFORMAT_DATA:
						case JVX_DATAFORMAT_8BIT:
						case JVX_DATAFORMAT_16BIT_LE:
						case JVX_DATAFORMAT_24BIT_LE:
						case JVX_DATAFORMAT_32BIT_LE:
						case JVX_DATAFORMAT_64BIT_LE:

							JVX_SAFE_NEW_FLD(copyPtr, jvxByte, jvxDataFormat_size[format]*num);
							res = theTriple.theProps->get_property(copyPtr,
								num, format, globalIdx,
								category,
								0,
								false,
								&isValid,
								NULL, JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);
							break;

						case JVX_DATAFORMAT_SELECTION_LIST:

							res =  theTriple.theProps->get_property(&lst,
								num, format, globalIdx,
								category,
								0,
								contentOnly,
								&isValid,
								NULL, JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);
							break;
						case JVX_DATAFORMAT_STRING:
							res =  theTriple.theProps->get_property(&fldStr,
								num, format, globalIdx,
								category,
								0,
								contentOnly,
								&isValid,
								NULL, JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);
							break;
						case JVX_DATAFORMAT_STRING_LIST:
							res =  theTriple.theProps->get_property(&lst.strList,
								num, format, globalIdx,
								category,
								0,
								contentOnly,
								&isValid,
								NULL, JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);
							break;
						case JVX_DATAFORMAT_VALUE_IN_RANGE:
							res = theTriple.theProps->get_property(&fldValRange,
								num, format, globalIdx,
								category,
								0,
								contentOnly,
								&isValid,
								NULL, JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);
							break;
						default:
							break;
						}
						if(res == JVX_NO_ERROR)
						{
							writeResponseLeadIn(server, conn, true, "--");

							token = ",\"component_target\":\"" + jvxComponentType_txt(compTp) + "\"";
							hdl->in_connect_write_response(server, conn, token.c_str());

							token = ",\"property_descriptor\":\"" + descriptor + "\"";
							hdl->in_connect_write_response(server, conn, token.c_str());

							jvxData* fldData = (jvxData*)copyPtr;
							jvxInt8* fldI8 = (jvxInt8*)copyPtr;
							jvxInt16* fldI16 = (jvxInt16*)copyPtr;
							jvxInt32* fldI32 = (jvxInt32*)copyPtr;
							jvxInt64* fldI64 = (jvxInt64*)copyPtr;

							switch(format)
							{
							case JVX_DATAFORMAT_DATA:

								token = ",\"value\":";
								hdl->in_connect_write_response(server, conn, token.c_str());
								if(num > 1)
								{
									token = "[";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								for(i = 0; i < num; i++)
								{
									token = "";
									if(i > 0)
									{
										token += ",";
									}
									token += jvx_data2String(fldData[i], JVX_NUMBER_DIGITS_FLOAT);
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								if(num > 1)
								{
									token = "]";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								break;

							case JVX_DATAFORMAT_8BIT:

								token = ",\"value\":";
								hdl->in_connect_write_response(server, conn, token.c_str());
								if(num > 1)
								{
									token = "[";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								for(i = 0; i < num; i++)
								{
									token = "";
									if(i > 0)
									{
										token += ",";
									}
									token += jvx_int2String(fldI8[i]);
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								if(num > 1)
								{
									token = "]";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								break;

							case JVX_DATAFORMAT_16BIT_LE:

								token = ",\"value\":";
								hdl->in_connect_write_response(server, conn, token.c_str());
								if(num > 1)
								{
									token = "[";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								for(i = 0; i < num; i++)
								{
									token = "";
									if(i > 0)
									{
										token += ",";
									}
									token += jvx_int2String(fldI16[i]);
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								if(num > 1)
								{
									token = "]";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								break;
							case JVX_DATAFORMAT_24BIT_LE:
							case JVX_DATAFORMAT_32BIT_LE:

								token = ",\"value\":";
								hdl->in_connect_write_response(server, conn, token.c_str());
								if(num > 1)
								{
									token = "[";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								for(i = 0; i < num; i++)
								{
									token = "";
									if(i > 0)
									{
										token += ",";
									}
									token += jvx_int2String(fldI32[i]);
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								if(num > 1)
								{
									token = "]";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								break;
							case JVX_DATAFORMAT_64BIT_LE:

								token = ",\"value\":";
								hdl->in_connect_write_response(server, conn, token.c_str());
								if(num > 1)
								{
									token = "[";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}

								for(i = 0; i < num; i++)
								{
									token = "";
									if(i > 0)
									{
										token += ",";
									}
									token += "\"" + jvx_int642String(fldI64[i]) + "\"";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								if(num > 1)
								{
									token = "]";
									hdl->in_connect_write_response(server, conn, token.c_str());
								}
								break;

							case JVX_DATAFORMAT_SELECTION_LIST:

								if(lst.strList)
								{
									token = ",\"entries\":[";
									hdl->in_connect_write_response(server, conn, token.c_str());

									for(i = 0; i < lst.strList->lStrings; i++)
									{
										token = "\"";
										token += lst.strList->bStrings[i].bString;
										token += "\"";
										hdl->in_connect_write_response(server, conn, token.c_str());
									}
									token = "],";
									hdl->in_connect_write_response(server, conn, token.c_str());

									theTriple.theObj->deallocate(lst.strList);
								}
								token = "\"value\":\"";
								token += jvx_int642String(lst.bitFieldSelected);
								token += "\"";
								hdl->in_connect_write_response(server, conn, token.c_str());
								break;

							case JVX_DATAFORMAT_STRING:

								if(fldStr)
								{
									token = ",\"value\":\"";
									token += fldStr->bString;
									token += "\"";
									hdl->in_connect_write_response(server, conn, token.c_str());

									theTriple.theObj->deallocate(fldStr);
								}
								break;
							case JVX_DATAFORMAT_STRING_LIST:
								if(lst.strList)
								{
									token = ",\"value\":[";
									hdl->in_connect_write_response(server, conn, token.c_str());

									for(i = 0; i < lst.strList->lStrings; i++)
									{
										token = "\"";
										token += lst.strList->bStrings[i].bString;
										token += "\"";
										hdl->in_connect_write_response(server, conn, token.c_str());
									}
									token = "],";
									hdl->in_connect_write_response(server, conn, token.c_str());

									theTriple.theObj->deallocate(lst.strList);
								}
								break;
							case JVX_DATAFORMAT_VALUE_IN_RANGE:
								token = ",\"value\":";
								token += "\"" + jvx_data2String(fldValRange.val,JVX_NUMBER_DIGITS_FLOAT) + "\"";
								hdl->in_connect_write_response(server, conn, token.c_str());

								token = "\",min\":";
								token += "\"" + jvx_data2String(fldValRange.minVal,JVX_NUMBER_DIGITS_FLOAT) + "\"";
								hdl->in_connect_write_response(server, conn, token.c_str());

								token = "\",max\":";
								token += "\"" + jvx_data2String(fldValRange.maxVal,JVX_NUMBER_DIGITS_FLOAT) + "\"";
								hdl->in_connect_write_response(server, conn, token.c_str());

							default:
								assert(0);
							}
							if(copyPtr)
							{
								JVX_SAFE_DELETE_FLD(copyPtr, jvxByte);
							}

							writeResponseLeadOut(server, conn);
						}
						else
						{
							writeResponseLeadIn(server, conn, false, "Failed to get property from component.");
							writeResponseLeadOut(server, conn);
						}
					}
					else
					{
						writeResponseLeadIn(server, conn, false, "Failed to get property number of entries from component.");
						writeResponseLeadOut(server, conn);
					}
				}
				else
				{
					writeResponseLeadIn(server, conn, false, "Failed to get property specification from component.");
					writeResponseLeadOut(server, conn);
				}
			}
			else
			{
				writeResponseLeadIn(server, conn, false, "Failed to get property reference for component.");
				writeResponseLeadOut(server, conn);
			}
		}
		return JVX_NO_ERROR;
	};

	virtual jvxErrorType response_properties__set_content(jvxHandle* server, jvxHandle* conn, jvxComponentType compTp, std::string descriptor, std::string value)
	{
		return JVX_NO_ERROR;
	};

	std::string extractStringParameters(const std::string& in_params, const char* loofor)
	{
		std::string retVal = "notfound";
		jvxSize sz;
		jvxString* fldStr = NULL;
		jvxErrorType resL = JVX_NO_ERROR;
		char oneToken[JVX_MAXSTRING];
		memset(oneToken, 0, JVX_MAXSTRING);
		sz = JVX_MAXSTRING-1;
		resL = hdl->in_connect_extract_var_content(in_params.c_str(), in_params.size(), loofor, oneToken, &sz);
		if(resL == JVX_NO_ERROR)
		{
			retVal = oneToken;
			hdl->in_connect_url_decode(retVal.c_str(), retVal.size(), &fldStr);
			if(fldStr)
			{
				retVal = fldStr->bString;
				hdl->deallocate(fldStr);
			}
		}
		return(retVal);
	};

	virtual jvxErrorType trigger_stop_all_websockets()
	{
		jvxSize i;
		// This is a little bit of a hack: we bypass the official civetweb api
		// to only shutdown the socket - the connection is kept.
		// As a result, the recv thread should return and shutdown comes immediately and
		// requires no incoming packet. DOES THIS WORK IN LINUX?
		for(i = 0; i < theWebserver.openConnectionsWs.size(); i++)
		{
			hdl->trigger_stop_single_websocket(theWebserver.openConnectionsWs[i].theServer, theWebserver.openConnectionsWs[i].selector[0]);
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType check_status_all_websockets_closed()
	{
		if(theWebserver.openConnectionsWs.size() == 0)
		{
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_NOT_READY);
	};
};

#endif
