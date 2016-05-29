#ifndef __jvxAuTNetwork_slave_H__
#define __jvxAuTNetwork_slave_H__

extern "C"
{
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTNetwork_slave_init(IjvxObject** retObject);
	jvxErrorType JVX_CALLINGCONVENTION jvxAuTNetwork_slave_terminate(IjvxObject* cls);
}

#endif
