#ifndef __jvxAppTCoreController_H__
#define __jvxAppTCoreController_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAppTCoreController_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAppTCoreController_terminate(IjvxObject* cls);
}

#endif
