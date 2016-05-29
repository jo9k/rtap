#ifndef __jvxTDataLogReader_H__
#define __jvxTDataLogReader_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataLogReader_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataLogReader_terminate(IjvxObject* cls);
}

#endif
