#ifndef __jvxTDataConverter_H__
#define __jvxTDataConverter_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataConverter_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTDataConverter_terminate(IjvxObject* cls);
}

#endif
