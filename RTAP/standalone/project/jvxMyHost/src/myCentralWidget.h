#ifndef __MYCENTRALWIDGET_H__
#define __MYCENTRALWIDGET_H__

#include "jvx.h"
#include "interfaces/qt/mainWindow_UiExtension.h"
#include "ui_myMainWidget.h"

#include "templates/jvxHosts/qt-host-ui-extensions/mainCentral.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

class myCentralWidget: public mainCentral, public Ui::Form_myMainWidget
{
private:

	Q_OBJECT

public:

	myCentralWidget(QWidget* parent): mainCentral(parent){};
	~myCentralWidget(){};
	virtual void init_submodule(IjvxHost* theHost, IjvxSubModuleInstall* submoduleInstall, IjvxMainWindowControl*);

	void inform_update_window();

signals:

public slots:
	void onANGLE();
	void onCFAC();
};

#ifdef JVX_PROJECT_NAMESPACE
}
#endif

#endif
