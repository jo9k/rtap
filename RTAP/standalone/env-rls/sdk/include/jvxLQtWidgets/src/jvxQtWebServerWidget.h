#ifndef __JVXQTWEBSERVERQWIDGET_H__
#define __JVXQTWEBSERVERQWIDGET_H__

#include "jvx.h"

#include <QtWidgets/QWidget>

class jvxQtWebServerWidget: public QWidget, public IjvxWebServer_report
{
private:
	
	IjvxWebServer* theLinkServer;

	Q_OBJECT
	
public:
	
	jvxQtWebServerWidget(QWidget* parent);
	
	~jvxQtWebServerWidget();
	
	jvxErrorType initialize(IjvxWebServer* theLink);

	void setupUi();
	
	jvxErrorType terminate();

	virtual jvxErrorType JVX_CALLINGCONVENTION report_event_request(
					jvxHandle* context_server, jvxHandle* context_conn,
					jvxWebServerHandlerPurpose purp, jvxSize uniqueId);

	
signals:
	void emit_request_handler(jvxHandle* , jvxHandle* ,
					jvxWebServerHandlerPurpose , jvxSize );
	
public slots:
	virtual void slot_request_handler(jvxHandle* context_server, jvxHandle* context_conn,
					jvxWebServerHandlerPurpose purp, jvxSize uniqueId) = 0;
	
};

#endif

