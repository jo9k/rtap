#ifndef __IJVXHOSTTYPEHANDLER_H__
#define __IJVXHOSTTYPEHANDLER_H__

JVX_INTERFACE IjvxHostTypeHandler
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxHostTypeHandler(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION add_type_host(jvxComponentType* tp, jvxSize numtp, const char* description, const char* tokenConfig, jvxComponentTypeClass classType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION number_types_host(jvxSize* num, jvxComponentTypeClass classType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_type_host(jvxSize idx, jvxString** theDescription, jvxString** theTokenInConfig, jvxComponentType* tp, jvxSize numtp, jvxComponentTypeClass classType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_all_host_types() = 0;
};

#endif