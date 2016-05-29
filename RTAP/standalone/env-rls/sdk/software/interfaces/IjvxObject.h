#ifndef __IJVXOBJECT_H__
#define __IJVXOBJECT_H__

JVX_INTERFACE IjvxObject: public IjvxDeallocate, public IjvxInformation
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxObject(){};
	
	virtual jvxErrorType JVX_CALLINGCONVENTION state(jvxState*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION lock_state() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unlock_state() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description(jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor(jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name(jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION feature_class(jvxBitField* ft) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION version(jvxString**) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION lasterror(jvxError**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION request_specialization(jvxHandle** obj, jvxComponentType*, jvxBool* multipleObjects) = 0;

};

#endif