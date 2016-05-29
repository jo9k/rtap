#ifndef __CONFIGUREAUDIO_UIEXTENSION_H__
#define __CONFIGUREAUDIO_UIEXTENSION_H__

/* 
 * Delegate interface for extension of the host for socket connections
 *///================================================================
 class QFrame;
 
JVX_INTERFACE configureAudio_UiExtension
{
public:
	
	configureAudio_UiExtension(){};
	
	virtual ~configureAudio_UiExtension(){};

	virtual void init_submodule(IjvxHost* theHost, IjvxSubModuleInstall*) = 0;
	virtual void terminate_submodule() = 0;

	virtual void start_show(jvxComponentType tp, QFrame* placeHere) = 0;
	virtual void stop_show(QFrame* placeHere) = 0;

	virtual void refreshPropertyIds() = 0;
	virtual void refreshPropertyIds_run() = 0;

	virtual void updateWindow() = 0;
	virtual void updateWindow_periodic() = 0;
};

#endif
