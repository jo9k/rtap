#ifndef __MAINWINDOW_UIEXTENSION_H__
#define __MAINWINDOW_UIEXTENSION_H__

#include "jvx.h"


/*
 * Delegate interface for extension of the host for socket connections
 *///================================================================
JVX_INTERFACE mainWindow_UiExtension
{
public:
	mainWindow_UiExtension(){};
	
	virtual ~mainWindow_UiExtension(){};

	virtual void init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* , IjvxMainWindowControl*) = 0;

	virtual void terminate_submodule() = 0;

	virtual void return_widget(QWidget** refWidget) = 0;

#ifdef QT_ALLOW_QT_WIDGETS
#ifndef QT_WIDGETS_NOT_HANDLED

	virtual void add_new_subwidget(QWidget* theNewWidget, const char* txt) = 0;

	virtual void remove_subwidget(QWidget* removeIt, QWidget** removeMeAlso) = 0;

#endif
#endif

	virtual void inform_bootup_complete(jvxBool* wantsToAdjustSize) = 0;

	virtual void inform_about_to_shutdown() = 0;

	virtual void inform_active(jvxComponentType tp) = 0;

	virtual void inform_inactive(jvxComponentType tp) = 0;

	virtual void inform_sequencer_started() = 0;

	virtual void inform_sequencer_stopped() = 0;

	virtual void inform_update_window() = 0;

	virtual void inform_update_window_periodic() = 0;

	virtual void inform_internals_have_changed(jvxComponentType tp,
                                                   IjvxObject* theObj,
                                                   jvxPropertyCategoryType cat,
                                                   jvxSize propId,
                                                   bool onlyContent,
                                                   jvxComponentType tpTo,
											jvxPropertyCallPurpose purpose) = 0;

	virtual jvxErrorType inform_sequencer_callback(jvxInt32 functionId) = 0;

	virtual jvxErrorType inform_close_triggered() = 0;

};

#endif
