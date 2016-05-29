#ifndef __jvxTDataLogger_H__
#define __jvxTDataLogger_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataLogger_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataLogger_terminate(IjvxObject* cls);
}

#endif
