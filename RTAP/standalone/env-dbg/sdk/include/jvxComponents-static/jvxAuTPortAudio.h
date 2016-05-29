#ifndef __jvxAuTPortAudio_H__
#define __jvxAuTPortAudio_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTPortAudio_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTPortAudio_terminate(IjvxObject* cls);
}

#endif
