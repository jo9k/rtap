#ifndef __IJVXQTWIDGETUI_H__
#define __IJVXQTWIDGETUI_H__

#if defined (QT_NAMESPACE)
namespace QT_NAMESPACE {
#endif

class QWidget;

#if defined (QT_NAMESPACE)
}
#endif

JVX_INTERFACE IjvxQtWidgetUi
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxQtWidgetUi(){};

#if defined (QT_NAMESPACE)
	virtual jvxErrorType JVX_CALLINGCONVENTION select_qt(QT_NAMESPACE::QWidget** theWidgetNew, IjvxObject** theObj, 
							     QT_NAMESPACE::QWidget* parent) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_qt(QT_NAMESPACE::QWidget** theWidget) = 0;
#else
	virtual jvxErrorType JVX_CALLINGCONVENTION select_qt(QWidget** theWidgetNew, IjvxObject** theObj, QWidget* parent) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_qt(QWidget** theWidget) = 0;
#endif

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect_qt() = 0;

};


#endif
