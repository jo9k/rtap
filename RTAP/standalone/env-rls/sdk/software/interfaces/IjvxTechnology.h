#ifndef __IJVXTECHNOLOGY_H__
#define __IJVXTECHNOLOGY_H__

JVX_INTERFACE IjvxTechnology: public IjvxObject, public IjvxHiddenInterface, public IjvxStateMachine
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxTechnology(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION request_device(jvxSize, IjvxDevice**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_device(IjvxDevice*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION number_devices(jvxSize*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_device(jvxSize, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_device(jvxSize, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name_device(jvxSize, jvxString**) = 0;
};

#endif
