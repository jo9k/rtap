#ifndef __CJVXQTCLASS_H__
#define __CJVXQTCLASS_H__

/* #include "CjvxObject.h" */
#include <interfaces/qt/IjvxQtWidgetUi.h>

template <class T, class Tq, class Tref> class CjvxQtClass: public T, public IjvxQtWidgetUi
{
protected:
	struct
	{
		Tref* theRefClass;
		Tq* theQtClassRef;
		jvxBool always_update_delayed;
	} _common_set_my_qt_props;

public:

	CjvxQtClass(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
		T(description, multipleObjects, module_name, descriptor, featureClass)
	{
		_common_set_my_qt_props.theRefClass = NULL;
		_common_set_my_qt_props.theQtClassRef = NULL;
		_common_set_my_qt_props.always_update_delayed = false;
	};

	~CjvxQtClass()
	{
	};

	void init_reference(Tref* theRef) { _common_set_my_qt_props.theRefClass = theRef; };

#if defined (QT_NAMESPACE)
	virtual jvxErrorType JVX_CALLINGCONVENTION select_qt(QT_NAMESPACE::QWidget** theWidgetNew, IjvxObject** theObj, 
							     QT_NAMESPACE::QWidget* parent)
	{
		jvxErrorType res = static_cast<IjvxStateMachine*>(this)->select();

		if(res == JVX_NO_ERROR)
		{
			_common_set_my_qt_props.theQtClassRef = new Tq(parent, static_cast<IjvxProperties*>(this));
			_common_set_my_qt_props.theQtClassRef->setupWidget();
			_common_set_my_qt_props.theQtClassRef->setReferenceShareWithUi(_common_set_my_qt_props.theRefClass);
			_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_SELECTED, _common_set_my_qt_props.always_update_delayed);
			if(theWidgetNew)
			{
			  *theWidgetNew = static_cast<QT_NAMESPACE::QWidget*>(_common_set_my_qt_props.theQtClassRef);
			}
			if(theObj)
			{
				*theObj = static_cast<IjvxObject*>(this);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_qt(QT_NAMESPACE::QWidget** theWidget)
	{
		if(theWidget)
		{
		  *theWidget = static_cast<QT_NAMESPACE::QWidget*>(_common_set_my_qt_props.theQtClassRef);
		}
		return(JVX_NO_ERROR);
	};
#else
	virtual jvxErrorType JVX_CALLINGCONVENTION select_qt(QWidget** theWidgetNew, IjvxObject** theObj, QWidget* parent)
	{
		jvxErrorType res = static_cast<IjvxStateMachine*>(this)->select();

		if(res == JVX_NO_ERROR)
		{
			_common_set_my_qt_props.theQtClassRef = new Tq(parent, static_cast<IjvxProperties*>(this));
			_common_set_my_qt_props.theQtClassRef->setupWidget();
			_common_set_my_qt_props.theQtClassRef->setReferenceShareWithUi(_common_set_my_qt_props.theRefClass);
			_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_SELECTED, _common_set_my_qt_props.always_update_delayed);
			if(theWidgetNew)
			{
				*theWidgetNew = static_cast<QWidget*>(_common_set_my_qt_props.theQtClassRef);
			}
			if(theObj)
			{
				*theObj = static_cast<IjvxObject*>(this);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_qt(QWidget** theWidget)
	{
		if(theWidget)
		{
			*theWidget = static_cast<QWidget*>(_common_set_my_qt_props.theQtClassRef);
		}
		return(JVX_NO_ERROR);
	};
#endif

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect_qt()
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(T::_common_set.theState == JVX_STATE_PROCESSING)
		{
			stop();
		}
		if(T::_common_set.theState == JVX_STATE_PREPARED)
		{
			postprocess();
		}
		if(T::_common_set.theState == JVX_STATE_ACTIVE)
		{
			deactivate();
		}
		if(T::_common_set.theState == JVX_STATE_SELECTED)
		{
			res = static_cast<IjvxStateMachine*>(this)->unselect();
			assert(res == JVX_NO_ERROR);
			_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_INIT, _common_set_my_qt_props.always_update_delayed);
			delete(_common_set_my_qt_props.theQtClassRef);
			_common_set_my_qt_props.theQtClassRef = NULL;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}

		return(res);
	};

	// ===========================================================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION activate()
	{
		jvxErrorType res = T::activate();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_ACTIVE, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate()
	{
		jvxErrorType res = T::deactivate();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_SELECTED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
	{
		jvxErrorType res = T::prepare(theEndpoint, numEndpoints);
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PREPARED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION start()
	{
		jvxErrorType res = T::start();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PROCESSING, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION stop()
	{
		jvxErrorType res = T::stop();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PREPARED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess()
	{
		jvxErrorType res = T::postprocess();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_ACTIVE, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	// ===========================================================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION activate_unlock()
	{
		jvxErrorType res = T::unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_ACTIVE, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate_unlock()
	{
		jvxErrorType res = T::unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_SELECTED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare_unlock()
	{
		jvxErrorType res = T::unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PREPARED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION start_unlock()
	{
		jvxErrorType res = T::_unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PROCESSING, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION stop_unlock()
	{
		jvxErrorType res = T::unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_PREPARED, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess_unlock()
	{
		jvxErrorType res = T::unlock_state();
		if(res == JVX_NO_ERROR)
		{
			if(_common_set_my_qt_props.theQtClassRef)
			{
				_common_set_my_qt_props.theQtClassRef->updateWindow_sync(JVX_STATE_ACTIVE, _common_set_my_qt_props.always_update_delayed);
			}
		}
		return(res);
	};

	// ===========================================================================================

	jvxErrorType request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl)
	{
		jvxErrorType res = JVX_NO_ERROR;

		switch(tp)
		{
		case JVX_INTERFACE_QT_WIDGET_UI:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxQtWidgetUi*>(this));
			break;

		default:
			res = T::request_hidden_interface(tp, hdl);
		}
		return(res);
	};

	jvxErrorType return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl)
	{
		jvxErrorType res = JVX_NO_ERROR;

		switch(tp)
		{

		case JVX_INTERFACE_QT_WIDGET_UI:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxQtWidgetUi*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;

		default:
			res = T::return_hidden_interface(tp, hdl);
		}
		return(res);
	}
};

#endif
