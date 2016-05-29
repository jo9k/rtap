#ifndef __jvxAuTAsio_H__
#define __jvxAuTAsio_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTAsio_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTAsio_terminate(IjvxObject* cls);
}

#endif
