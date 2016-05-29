#ifndef __IJVXDEALLOCATE_H__
#define __IJVXDEALLOCATE_H__

JVX_INTERFACE IjvxDeallocate
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxDeallocate(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxString*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxError*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxStringList* lst) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxDataList* err) = 0;

//	virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxSelectionList* lst) = 0;
};

#endif