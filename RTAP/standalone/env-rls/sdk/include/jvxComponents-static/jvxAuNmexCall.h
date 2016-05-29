#ifndef __jvxAuNmexCall_H__
#define __jvxAuNmexCall_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAuNmexCall_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAuNmexCall_terminate(IjvxObject* cls);
}

#endif
