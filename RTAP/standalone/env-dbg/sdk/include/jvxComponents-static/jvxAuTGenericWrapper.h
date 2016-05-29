#ifndef __jvxAuTGenericWrapper_H__
#define __jvxAuTGenericWrapper_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTGenericWrapper_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTGenericWrapper_terminate(IjvxObject* cls);
}

#endif
