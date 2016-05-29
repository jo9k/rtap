#ifndef __IJVXQTWIDGETHOST_H__
#define __IJVXQTWIDGETHOST_H__

JVX_INTERFACE IjvxQtWidgetUi;

JVX_INTERFACE IjvxQtWidgetHost
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxQtWidgetHost(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION select_component_qt_thread(IjvxQtWidgetUi* theObjectSm) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect_component_qt_thread(IjvxQtWidgetUi* theObjectSm) = 0;

};

#endif
