#ifndef __jvxTThreadController_H__
#define __jvxTThreadController_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxTThreadController_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxTThreadController_terminate(IjvxObject* cls);
}

#endif
