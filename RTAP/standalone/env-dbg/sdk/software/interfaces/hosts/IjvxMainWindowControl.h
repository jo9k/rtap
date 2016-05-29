#ifndef __IJVXMAINWINDOWCONTROL_H__
#define __IJVXMAINWINDOWCONTROL_H__

JVX_INTERFACE IjvxMainWindowControl
{
public:
	IjvxMainWindowControl(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_operation(
		jvxMainWindowController_trigger_operation_ids id_operation, 
		jvxHandle* theData = 0) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION register_functional_callback(
		jvxMainWindowController_register_functional_ids id_operation, 
		jvxMainWindowController_uiCallback, 
		jvxHandle* privData, 
		jvxHandle* contentData,
		jvxInt32* id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unregister_functional_callback(jvxInt32 id) = 0;

};

#endif