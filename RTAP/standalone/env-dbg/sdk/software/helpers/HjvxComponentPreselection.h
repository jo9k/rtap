#ifndef __HJVXCOMPONENTPRESELECTION_H__
#define __HJVXCOMPONENTPRESELECTION_H__

#include "helpers/HjvxComponentPreselection_common.h"

// ===================================================================

static void jvx_getReferenceComponentType(jvxComponentType tp, IjvxObject*& theObj, bool addressesStandardComponent, IjvxHost* hostRef)
{
	jvxErrorType res = JVX_NO_ERROR;
	theObj = NULL;
	IjvxToolsHost* theToolsHost = NULL;
	switch(tp)
	{
	case JVX_COMPONENT_AUDIO_TECHNOLOGY:
	case JVX_COMPONENT_AUDIO_DEVICE:
	case JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY:
	case JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE:
	case JVX_COMPONENT_AUDIO_NODE:
		res = hostRef->request_object_component(tp, &theObj);
		break;
	case JVX_COMPONENT_DATALOGGER:

		if(!addressesStandardComponent)
		{
			res = hostRef->request_hidden_interface(JVX_INTERFACE_TOOLS_HOST, reinterpret_cast<jvxHandle**>(&theToolsHost));
			if((res == JVX_NO_ERROR) && theToolsHost)
			{
				theToolsHost->reference_tool(JVX_COMPONENT_DATALOGGER, 0, &theObj);
				hostRef->return_hidden_interface(JVX_INTERFACE_TOOLS_HOST, reinterpret_cast<jvxHandle*>(theToolsHost));
			}
		}
		break;
	case JVX_COMPONENT_RS232:
	case JVX_COMPONENT_CONFIG_PROCESSOR:
	case JVX_COMPONENT_EXTERNAL_CALL:
	case JVX_COMPONENT_UNKNOWN:
	case JVX_COMPONENT_HOST:
	case JVX_COMPONENT_DATALOGREADER:
	case JVX_COMPONENT_RT_AUDIO_FILE_READER:
	case JVX_COMPONENT_RT_AUDIO_FILE_WRITER:
	case JVX_COMPONENT_RESAMPLER:
	case JVX_COMPONENT_DATACONVERTER:
	case JVX_COMPONENT_THREADCONTROLLER:
	case JVX_COMPONENT_CRYPT:
	case JVX_COMPONENT_WEBSERVER:
		break;
	default:
		assert(0);
	}
}

static void jvx_returnReferenceComponentType(jvxComponentType tp, IjvxObject* theObj, bool addressesStandardComponent, IjvxHost* hostRef)
{
	jvxErrorType res = JVX_NO_ERROR;
	IjvxToolsHost* theToolsHost = NULL;

	switch(tp)
	{
	case JVX_COMPONENT_AUDIO_TECHNOLOGY:
	case JVX_COMPONENT_AUDIO_DEVICE:
	case JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY:
	case JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE:
	case JVX_COMPONENT_AUDIO_NODE:
		res = hostRef->return_object_component(tp, theObj);
		break;
	case JVX_COMPONENT_DATALOGGER:

		if(!addressesStandardComponent)
		{
			res = hostRef->request_hidden_interface(JVX_INTERFACE_TOOLS_HOST, reinterpret_cast<jvxHandle**>(&theToolsHost));
			if((res == JVX_NO_ERROR) && theToolsHost)
			{
				theToolsHost->return_reference_tool(JVX_COMPONENT_DATALOGGER, theObj);
				hostRef->return_hidden_interface(JVX_INTERFACE_TOOLS_HOST, reinterpret_cast<jvxHandle*>(theToolsHost));
			}
		}
		break;

	case JVX_COMPONENT_RS232:
	case JVX_COMPONENT_CONFIG_PROCESSOR:
	case JVX_COMPONENT_EXTERNAL_CALL:
	case JVX_COMPONENT_UNKNOWN:
	case JVX_COMPONENT_HOST:
	case JVX_COMPONENT_DATALOGREADER:
	case JVX_COMPONENT_RT_AUDIO_FILE_READER:
	case JVX_COMPONENT_RT_AUDIO_FILE_WRITER:
	case JVX_COMPONENT_RESAMPLER:
	case JVX_COMPONENT_DATACONVERTER:
	case JVX_COMPONENT_THREADCONTROLLER:
	case JVX_COMPONENT_CRYPT:
	case JVX_COMPONENT_WEBSERVER:
		break;
	default:
		assert(0);
	}
}

#endif
