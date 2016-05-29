#ifndef __jvxTResampler_H__
#define __jvxTResampler_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTResampler_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTResampler_terminate(IjvxObject* cls);
}

#endif
