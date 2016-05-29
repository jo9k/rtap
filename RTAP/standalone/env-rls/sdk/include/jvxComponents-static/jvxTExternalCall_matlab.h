#ifndef __jvxTExternalCall_matlab_H__
#define __jvxTExternalCall_matlab_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTExternalCall_matlab_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTExternalCall_matlab_terminate(IjvxObject* cls);
}

#endif
