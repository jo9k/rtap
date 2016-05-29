#ifndef __IJVXTOOLSHOST_H__
#define __IJVXTOOLSHOST_H__

JVX_INTERFACE IjvxToolsHost
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxToolsHost(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION number_tools(jvxComponentType, jvxSize* num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_tool(jvxComponentType, jvxSize idx, jvxString** name) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_tool(jvxComponentType, jvxSize, IjvxObject** theObject, jvxBitField stateMask = (jvxBitField)JVX_STATE_DONTCARE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_reference_tool(jvxComponentType, IjvxObject* theObject) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION instance_tool(jvxComponentType, jvxSize, IjvxObject** theObject) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_instance_tool(jvxComponentType, jvxSize, IjvxObject* theObject) = 0;
};

#endif