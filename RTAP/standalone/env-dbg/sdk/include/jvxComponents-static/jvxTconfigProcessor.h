#ifndef __jvxTconfigProcessor_H__
#define __jvxTconfigProcessor_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTconfigProcessor_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTconfigProcessor_terminate(IjvxObject* cls);
}

#endif
