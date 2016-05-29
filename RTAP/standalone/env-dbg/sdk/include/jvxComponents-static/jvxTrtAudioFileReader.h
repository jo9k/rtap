#ifndef __jvxTrtAudioFileReader_H__
#define __jvxTrtAudioFileReader_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTrtAudioFileReader_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTrtAudioFileReader_terminate(IjvxObject* cls);
}

#endif
