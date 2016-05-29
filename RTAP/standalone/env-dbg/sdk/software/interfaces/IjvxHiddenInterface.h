#ifndef __IJVXHIDDENINTERFACE_H__
#define __IJVXHIDDENINTERFACE_H__

JVX_INTERFACE IjvxHiddenInterface
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxHiddenInterface(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION request_hidden_interface(jvxInterfaceType, jvxHandle**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_hidden_interface(jvxInterfaceType, jvxHandle*) = 0;
};

#endif