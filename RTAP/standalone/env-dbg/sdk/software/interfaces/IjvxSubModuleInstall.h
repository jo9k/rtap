#ifndef __JVXSUBMODULEINSTALL_H__
#define __JVXSUBMODULEINSTALL_H__

JVX_INTERFACE IjvxSubModuleInstall
{
public:
	IjvxSubModuleInstall(){};
	virtual jvxErrorType set_link(jvxHandle* linkRef, jvxSize tp = 0) = 0;
	virtual jvxErrorType release_link(jvxHandle* linkRef, jvxSize tp = 0) = 0;
};

#endif
