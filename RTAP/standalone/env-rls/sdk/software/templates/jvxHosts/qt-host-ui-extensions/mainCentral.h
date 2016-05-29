#ifndef __MAINCENTRAL_H__
#define __MAINCENTRAL_H__

#ifndef JVX_DONT_DERIVE_FROM_UI_DESIGNER
#include "ui_mainCentral.h"
#else
#include <QWidget>
#endif

#include "interfaces/qt/mainWindow_UiExtension.h"

#include "templates/common/CjvxWebServerHost.h"

#ifdef JVX_ADD_HOST_WEBSERVER
#include <QTimer>
#endif

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

class mainCentral: public QWidget, public mainWindow_UiExtension
#ifndef JVX_DONT_DERIVE_FROM_UI_DESIGNER
, public Ui::Form_mainCentral
#endif
#ifdef JVX_ADD_HOST_WEBSERVER
, public CjvxWebServerHost, public IjvxWebServerHost_hooks
#endif
{

protected:	
	IjvxHost* theHostRef;
	IjvxSubModuleInstall* bwdRef;
	IjvxMainWindowControl* ctrlRef;
	IjvxReport* theReport;
	IjvxToolsHost* theToolsHost;

#ifdef JVX_ADD_HOST_WEBSERVER
	struct
	{
		IjvxWebServer* hdl;
		IjvxObject* obj;
		jvxErrorType res_mthread;
		QTimer* myTimer;
		jvxInt16 closeProcedureState;
	} theWebserver;

#endif
	Q_OBJECT

public:

	mainCentral(QWidget* parent);

	virtual void init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* submoduleInstall, IjvxMainWindowControl*);

	virtual void terminate_submodule();

	virtual void return_widget(QWidget** refWidget);


	virtual void inform_bootup_complete(jvxBool* wantsToAdjustSize);

	virtual void inform_about_to_shutdown();

	virtual void inform_internals_have_changed(jvxComponentType tp,
		IjvxObject* theObj,
		jvxPropertyCategoryType cat,
		jvxSize propId,
		bool onlyContent,
		jvxComponentType tpTo,
		jvxPropertyCallPurpose purp);
	
	virtual void inform_active(jvxComponentType tp);
	
	virtual void inform_inactive(jvxComponentType tp);
	
	virtual void inform_sequencer_started();
	
	virtual void inform_sequencer_stopped();
	
	virtual void inform_update_window();
	
	virtual void inform_update_window_periodic();

#ifdef QT_ALLOW_QT_WIDGETS
#ifndef QT_WIDGETS_NOT_HANDLED
	
	virtual void add_new_subwidget(QWidget* theNewWidget, const char* txt);

	virtual void remove_subwidget(QWidget* removeIt, QWidget** removeMeAlso) ;
#endif
#endif

	virtual jvxErrorType inform_sequencer_callback(jvxInt32 functionId);
	virtual jvxErrorType inform_close_triggered();

#ifdef JVX_ADD_HOST_WEBSERVER
	
	jvxErrorType synchronizeWebServerEvents(jvxHandle* context_server, jvxHandle* context_conn, jvxWebServerHandlerPurpose purp, jvxSize uniqueId,jvxBool strictConstConnection, int header, char *payload, size_t szFld);
	
	jvxErrorType hook_safeConfigFile();
	jvxErrorType hook_startSequencer();
	jvxErrorType hook_stopSequencer();

	jvxErrorType hook_operationComplete();

	jvxErrorType hook_ws_started();
	jvxErrorType hook_ws_stopped();


#endif

signals:

#ifdef JVX_ADD_HOST_WEBSERVER
	void emit_report_event_request(jvxHandle*, jvxHandle*, jvxWebServerHandlerPurpose, jvxSize, jvxBool, int, char*, size_t);
#endif

public slots:

#ifdef JVX_ADD_HOST_WEBSERVER
	void slot_report_event_request(jvxHandle*, jvxHandle*, jvxWebServerHandlerPurpose, jvxSize, jvxBool strictConstConnection, int header, char *payload, size_t szFld);
	void slot_postPropertyStreams();
#endif

};

#ifdef JVX_PROJECT_NAMESPACE
}
#endif

#endif

