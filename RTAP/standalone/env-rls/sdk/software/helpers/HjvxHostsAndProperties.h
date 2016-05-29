#ifndef __HJVXHOSTANDPROPERTIES_H__
#define __HJVXHOSTANDPROPERTIES_H__

typedef struct
{
	IjvxObject* theObj;
	jvxHandle* theHdl;
	IjvxProperties* theProps;
} jvx_propertyReferenceTriple;

static void jvx_initPropertyReferenceTriple(jvx_propertyReferenceTriple* theTuple)
{
	theTuple->theObj = NULL;
	theTuple->theHdl = NULL;
	theTuple->theProps = NULL;
};

static bool jvx_isValidPropertyReferenceTriple(jvx_propertyReferenceTriple* theTuple)
{
	if((theTuple->theObj != NULL) && (theTuple->theHdl != NULL) && (theTuple->theProps != NULL))
	{
		return(true);
	}
	return(false);
};

static jvxErrorType jvx_getReferencePropertiesObject(IjvxHost* theHost, jvx_propertyReferenceTriple* theTriple, jvxComponentType tp)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvx_initPropertyReferenceTriple(theTriple);

	res = theHost->request_object_component(tp, &theTriple->theObj);
	if((res == JVX_NO_ERROR) && (theTriple->theObj))
	{
		theTriple->theObj->request_specialization(&theTriple->theHdl, NULL, NULL);
		if(theTriple->theHdl)
		{
			switch(tp)
			{
			case JVX_COMPONENT_AUDIO_TECHNOLOGY:
			case JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY:
				((IjvxTechnology*)theTriple->theHdl)->request_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle**>(&theTriple->theProps));
				break;
			case JVX_COMPONENT_AUDIO_DEVICE:
			case JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE:
				((IjvxDevice*)theTriple->theHdl)->request_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle**>(&theTriple->theProps));
				break;
			case JVX_COMPONENT_AUDIO_NODE:
				((IjvxNode*)theTriple->theHdl)->request_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle**>(&theTriple->theProps));
				break;
			default:
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
				break;
			}
		}
	}
	if(res != JVX_NO_ERROR)
	{
		if(theTriple->theObj)
		{
			theHost->return_object_component(tp, theTriple->theObj);
		}
	}

	return(res);
}

static jvxErrorType jvx_returnReferencePropertiesObject(IjvxHost* theHost, jvx_propertyReferenceTriple* theTriple, jvxComponentType tp)
{
	jvxErrorType res = JVX_ERROR_INVALID_ARGUMENT;
	if(theTriple->theObj)
	{
		if(theTriple->theHdl)
		{
			if(theTriple->theProps)
			{
				switch(tp)
				{
				case JVX_COMPONENT_AUDIO_TECHNOLOGY:
				case JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY:
					((IjvxTechnology*)theTriple->theHdl)->return_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle*>(theTriple->theProps));
					res = JVX_NO_ERROR;
					break;

				case JVX_COMPONENT_AUDIO_DEVICE:
				case JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE:
					((IjvxDevice*)theTriple->theHdl)->return_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle*>(theTriple->theProps));
					res = JVX_NO_ERROR;
					break;
				case JVX_COMPONENT_AUDIO_NODE:
					((IjvxNode*)theTriple->theHdl)->return_hidden_interface(JVX_INTERFACE_PROPERTIES, reinterpret_cast<jvxHandle*>(theTriple->theProps));
					res = JVX_NO_ERROR;
					break;
				default:
					res = JVX_ERROR_ELEMENT_NOT_FOUND;
				}
			}
			theHost->return_object_component(tp, theTriple->theObj);
		}
	}
	return(res);
}


#endif