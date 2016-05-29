#include "mainCentral.h"

#include <QScrollArea>

#include "jvx.h"
#include "typedefs/TjvxSubModuleTypeIds.h"

#include <iostream>

Q_DECLARE_METATYPE(jvxHandle*);
Q_DECLARE_METATYPE(jvxWebServerHandlerPurpose);
Q_DECLARE_METATYPE(jvxSize);
Q_DECLARE_METATYPE(jvxBool);

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif




mainCentral::mainCentral(QWidget* parent): QWidget(parent)
{
	qRegisterMetaType<jvxHandle*>("jvxHandle*");
	qRegisterMetaType<jvxWebServerHandlerPurpose>("jvxWebServerHandlerPurpose");
	qRegisterMetaType<jvxSize>("jvxSize");
	qRegisterMetaType<jvxBool>("jvxBool");

	theHostRef = NULL;
	bwdRef = NULL;
	ctrlRef = NULL;
	theReport = NULL;

	theToolsHost = NULL;

#ifdef JVX_ADD_HOST_WEBSERVER
		theWebserver.myTimer = NULL;
		theWebserver.closeProcedureState = 0;
#endif
}

// #################################################################
void
mainCentral::init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* bwd, IjvxMainWindowControl* ctrl)
{
	theHostRef = theHost;
	bwdRef = bwd;
	ctrlRef = ctrl;
	theHostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));

	assert(theHostRef);
	assert(theReport);

#ifndef JVX_DONT_DERIVE_FROM_UI_DESIGNER
	this->setupUi(this);
#endif

#ifdef JVX_ADD_HOST_WEBSERVER
	connect(this, SIGNAL(emit_report_event_request(jvxHandle*, jvxHandle*, jvxWebServerHandlerPurpose, jvxSize, jvxBool, int , char *, size_t )), 
		this, SLOT(slot_report_event_request(jvxHandle*, jvxHandle*, jvxWebServerHandlerPurpose, jvxSize, jvxBool, int , char *, size_t)), Qt::BlockingQueuedConnection);
#endif

	bwd->set_link(static_cast<mainWindow_UiExtension*>(this), JVX_MAIN_CENTRAL_EXTENSION_UI);
}

void
mainCentral::terminate_submodule( )
{
	theHostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
	theReport = NULL;

	bwdRef->release_link(static_cast<mainWindow_UiExtension*>(this), JVX_MAIN_CENTRAL_EXTENSION_UI);

	theHostRef = NULL;
	bwdRef = NULL;
	ctrlRef = NULL;

}

void
mainCentral::return_widget(QWidget** refWidget)
{
	*refWidget = static_cast<QWidget*>(this);
}

#ifdef QT_ALLOW_QT_WIDGETS
#ifndef QT_WIDGETS_NOT_HANDLED

void
mainCentral::add_new_subwidget(QWidget* theNewWidget, const char* txt)
{
#ifndef JVX_DONT_DERIVE_FROM_UI_DESIGNER
	QScrollArea* newScrollArea = new QScrollArea(tabWidget_components);
    newScrollArea->setBackgroundRole(QPalette::Window);
    newScrollArea->setFrameShadow(QFrame::Plain);
    newScrollArea->setFrameShape(QFrame::NoFrame);
    newScrollArea->setWidgetResizable(true);

    //vertical box that contains all the checkboxes for the filters

    //techArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //techArea->setLayout(new QVBoxLayout(techArea));
    newScrollArea->setWidget(theNewWidget);
	this->tabWidget_components->addTab(newScrollArea, txt);
#endif
}

void
mainCentral::remove_subwidget(QWidget* removeIt, QWidget** removeMeAlso )
{
#ifndef JVX_DONT_DERIVE_FROM_UI_DESIGNER
	jvxSize i;
	QScrollArea* myScroll = NULL;
	QWidget* theRemoveWidget = NULL;
	for(i = 0; i < this->tabWidget_components->count(); i++)
	{
		QWidget* sglWidget = this->tabWidget_components->widget(i);
		myScroll = dynamic_cast<QScrollArea*>(sglWidget);
		if(myScroll)
		{
			theRemoveWidget = myScroll->widget();
			if(theRemoveWidget == removeIt)
			{
				if(removeMeAlso)
				{
					*removeMeAlso = sglWidget;
				}
				this->tabWidget_components->removeTab(i);
				break;
			}
		}
	}
#endif
}

#endif
#endif

/*
 * Callback right after a component has been activated.
 *///=======================================================================
void 
mainCentral::inform_active(jvxComponentType tp)
{
	std::cout << "Component activated: " << jvxComponentType_str[tp] << std::endl;
}

/*
 * Callback right after a component has been deactivated.
 *///=======================================================================
void
mainCentral::inform_inactive(jvxComponentType tp)
{
	std::cout << "Component deactivated: " << jvxComponentType_str[tp] << std::endl;
}

/*
 * Callback right after the sequencer has been started
 *///=======================================================================
void
mainCentral::inform_sequencer_started()
{
}

/*
 * Callback right after the sequencer has been stopped
 *///=======================================================================
void
mainCentral::inform_sequencer_stopped()
{
}

/*
 * Callback to indiate that the main window is supposed to be redrawn
 *///=======================================================================

void
mainCentral::inform_update_window()
{
}

/*
 * Callback to make periodic updates of the window (in case it is running)
 *///=====================================================================
void
mainCentral::inform_update_window_periodic()
{
#ifdef JVX_ADD_HOST_WEBSERVER
	//postPropertyStreams();
#endif
}

/*
 * Callback to indicate that booting has been completed
 *///=====================================================================
void
mainCentral::inform_bootup_complete(jvxBool* wantsToAdjust)
{
	jvxErrorType resL = JVX_NO_ERROR;
	if(theHostRef)
	{
		resL = theHostRef->request_hidden_interface(JVX_INTERFACE_TOOLS_HOST, reinterpret_cast<jvxHandle**>(&theToolsHost));
#ifdef JVX_ADD_HOST_WEBSERVER
		if((resL == JVX_NO_ERROR) && theToolsHost)
		{
			resL = theToolsHost->reference_tool(JVX_COMPONENT_WEBSERVER, 0, &theWebserver.obj);
			if((resL == JVX_NO_ERROR) && theWebserver.obj)
			{
				theWebserver.obj->request_specialization(reinterpret_cast<jvxHandle**>(&theWebserver.hdl), NULL, NULL);
			}
		}
#endif
	}

#ifdef JVX_ADD_HOST_WEBSERVER
	// Start web server
	resL = start_webserver(theWebserver.hdl, theHostRef, static_cast<IjvxWebServerHost_hooks*>(this));
	assert(resL == JVX_NO_ERROR);

#endif

	if(wantsToAdjust)
		*wantsToAdjust = true;
}

void
mainCentral::inform_about_to_shutdown()
{
}

jvxErrorType 
mainCentral::inform_close_triggered()
{
#ifdef JVX_ADD_HOST_WEBSERVER
	jvxSize i;
	jvxErrorType resL = JVX_NO_ERROR;
	// Stop web server
	if(theWebserver.hdl)
	{
		switch(theWebserver.closeProcedureState)
		{
		case 0:
			theWebserver.hdl->trigger_stop();

			trigger_stop_all_websockets();
			theWebserver.closeProcedureState = 1;
			return(JVX_ERROR_NOT_READY);
		case 1:
			if(check_status_all_websockets_closed() == JVX_NO_ERROR)
			{
				theWebserver.closeProcedureState = 2;				
			}
			return(JVX_ERROR_NOT_READY);
		case 2:
			resL = theWebserver.hdl->wait_stop_complete();
			if(resL == JVX_NO_ERROR)
			{
				theWebserver.closeProcedureState = 3;
			}
			return(JVX_ERROR_NOT_READY);
		case 3:
			theToolsHost->return_reference_tool(JVX_COMPONENT_WEBSERVER, theWebserver.obj);
			theWebserver.obj = NULL;
			theWebserver.hdl = NULL;
			break;
		default: 
			assert(0);
		}
	}
	
#endif
	return(JVX_NO_ERROR);
}

/*
 * Callback to indicate that one of the properties has changed internally
 *///=====================================================================
void
mainCentral::inform_internals_have_changed(jvxComponentType tp, IjvxObject* theObj, jvxPropertyCategoryType cat, jvxSize propId, bool onlyContent, jvxComponentType tpTo, jvxPropertyCallPurpose purpose)
{
	
}

jvxErrorType 
mainCentral::inform_sequencer_callback(jvxInt32 functionId)
{
	return(JVX_NO_ERROR);
}

// ********************************************************************************

#ifdef JVX_ADD_HOST_WEBSERVER
	
jvxErrorType 
mainCentral::synchronizeWebServerEvents(jvxHandle* context_server, jvxHandle* context_conn, jvxWebServerHandlerPurpose purp, jvxSize uniqueId, jvxBool strictConstConnection, int header, char *payload, size_t szFld)
{
	theWebserver.res_mthread = JVX_ERROR_UNSUPPORTED;
	//if(!theWebserver.closeProcedureStarted)
	//{
		emit emit_report_event_request(context_server, context_conn, purp, uniqueId, strictConstConnection, header, payload, szFld);
	//}
	return(theWebserver.res_mthread );
}

jvxErrorType 
mainCentral::hook_safeConfigFile()
{
	if(ctrlRef)
	{
		ctrlRef->trigger_operation(JVX_MAINWINDOWCONTROLLER_TRIGGER_SAFE_CONFIG, NULL);
	}
	return(JVX_NO_ERROR);
}

jvxErrorType 
mainCentral::hook_startSequencer()
{
	if(ctrlRef)
	{
		ctrlRef->trigger_operation(JVX_MAINWINDOWCONTROLLER_TRIGGER_START, NULL);
	}
	return(JVX_NO_ERROR);
}

jvxErrorType 
mainCentral::hook_stopSequencer()
{
	if(ctrlRef)
	{
		ctrlRef->trigger_operation(JVX_MAINWINDOWCONTROLLER_TRIGGER_STOP, NULL);
	}
	return(JVX_NO_ERROR);
}

jvxErrorType 
mainCentral::hook_operationComplete()
{
	if(ctrlRef)
	{
		ctrlRef->trigger_operation(JVX_MAINWINDOWCONTROLLER_TRIGGER_UPDATE_WINDOW, NULL);
	}
	return(JVX_NO_ERROR);
}

jvxErrorType 
mainCentral::hook_ws_started()
{
	theWebserver.myTimer = new QTimer(this);
	connect(theWebserver.myTimer, SIGNAL(timeout()), this, SLOT(slot_postPropertyStreams()));
	theWebserver.myTimer->start(200);
	return(JVX_NO_ERROR);
}

jvxErrorType 
mainCentral::hook_ws_stopped()
{
	theWebserver.myTimer->stop();
	disconnect(theWebserver.myTimer, SIGNAL(timeout()), this, SLOT(slot_postPropertyStreams()));
	delete(theWebserver.myTimer);
	theWebserver.myTimer = NULL;
	return(JVX_NO_ERROR);
}

void
mainCentral::slot_postPropertyStreams()
{
	postPropertyStreams();
}

void
mainCentral::slot_report_event_request(jvxHandle* context_server, jvxHandle* context_conn, jvxWebServerHandlerPurpose purp, jvxSize uniqueId, jvxBool strictConstConnection, int header, char *payload, size_t szFld)
{
	theWebserver.res_mthread  = JVX_ERROR_UNSUPPORTED;
	if(theWebserver.hdl)
	{
		theWebserver.res_mthread  = report_event_request_core(context_server, context_conn, purp, uniqueId, strictConstConnection, header, payload, szFld);
	}
}

#endif

#ifdef JVX_PROJECT_NAMESPACE
}
#endif
