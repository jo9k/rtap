#ifndef __CJVXOBJECT_H__
#define __CJVXOBJECT_H__

#include "jvx.h"

class CjvxObject
{
protected:

	struct
	{
		std::string theDescription;
		std::string theDescriptor;
		std::string theModuleName;
		jvxBitField theFeatureClass;
		jvxState theState;
		jvxComponentType theComponentType;
		jvxHandle* theObjectSpecialization;
		bool theMultipleObjects;

		jvxInt32 theErrorid;	// -1 = no info
		jvxInt32 theErrorlevel; // -1 = no info
		std::string theErrordescr;

		//IjvxHost* theHostRef;
		IjvxHiddenInterface* theHostRef;
		IjvxObject* thisisme;

		std::vector<std::string> theInformation;
		struct
		{
			JVX_MUTEX_HANDLE the_lock;
		} _safeAccessStateBound;

	} _common_set;

	/*
	struct
	{
		jvxSize theNumberEndpoints;
		IjvxDataProcessor** theEndpoints;
	} _common_set_processing;
	*/

public:

	//CjvxObject(const char* descr, bool multipleObjects, const char* module_name, const char* descriptor = NULL, jvxBitField featClass = (jvxBitField)JVX_FEATURE_CLASS_NONE)
	CjvxObject(const char* descr, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featClass )
	{
		_common_set.theDescription = descr;
		_common_set.theModuleName = module_name;
		_common_set.theFeatureClass = featClass;
		if(descriptor)
		{
			_common_set.theDescriptor = descriptor;
		}
		else
		{
			_common_set.theDescriptor = descr;
		}

		_common_set.theState = JVX_STATE_NONE;
		_common_set.theComponentType = JVX_COMPONENT_UNKNOWN;
		_common_set.theObjectSpecialization = NULL;
		_common_set.theMultipleObjects = multipleObjects;

		_common_set.theErrorid = -1;
		_common_set.theErrorlevel = -1;
		_common_set.theErrordescr = "";

		_common_set.theHostRef = NULL;

		//_common_set.theInformation;
		/*
		_common_set_processing.theEndpoints = NULL;
		_common_set_processing.theNumberEndpoints = NULL;
		*/

		_common_set.thisisme = NULL;

		JVX_INITIALIZE_MUTEX(_common_set._safeAccessStateBound.the_lock);
	}

	~CjvxObject()
	{
		JVX_TERMINATE_MUTEX(_common_set._safeAccessStateBound.the_lock);
	};


	// =========================================================================

	inline jvxErrorType _state(jvxState* stat)
	{
		if(stat)
		{
			*stat = _common_set.theState;
		}
		return(JVX_NO_ERROR);
	}

	inline jvxErrorType _description(jvxString** str)
	{
		HjvxObject_produceString(str, _common_set.theDescription);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _descriptor(jvxString** str)
	{
		HjvxObject_produceString(str, _common_set.theDescriptor);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _module_name(jvxString** str)
	{
		HjvxObject_produceString(str, _common_set.theModuleName);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _feature_class(jvxBitField* ft)
	{
		if(ft)
		{
			*ft = _common_set.theFeatureClass;
		}
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _version(jvxString** str)
	{
		HjvxObject_produceString(str, JVX_VERSION_TOKEN);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _requestComponentSpecialization(jvxHandle** obj, jvxComponentType* compTp, jvxBool* multipleObjects)
	{
		if(obj)
		{
			*obj = _common_set.theObjectSpecialization;
		}

		if(compTp)
		{
			*compTp = _common_set.theComponentType;
		}
		if(multipleObjects)
		{
			*multipleObjects = _common_set.theMultipleObjects;
		}
		return(JVX_NO_ERROR);
	};

	inline void _seterror(jvxErrorType tp)
	{
		_common_set.theErrordescr = jvxErrorType_descr((jvxSize)tp);
	};

	inline jvxErrorType _return_seterr(jvxErrorType tp, jvxInt32 errId = -1, jvxInt32 errLvl = -1)
	{
		_common_set.theErrordescr = jvxErrorType_descr((jvxSize)tp);
		_common_set.theErrorid =  errId;
		_common_set.theErrorlevel = errLvl;
		return(tp);
	};

	inline jvxErrorType _lasterror(jvxError** err_argout)
	{
		HjvxObject_produceError(err_argout, _common_set.theErrorid,
			_common_set.theErrorlevel,  _common_set.theErrordescr);

		_common_set.theErrorid = -1;
		_common_set.theErrorlevel = -1;
		_common_set.theErrordescr = "";

		return(JVX_NO_ERROR);
	};

	// =========================================================================

	inline jvxErrorType _request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl)
	{
		return(JVX_ERROR_UNSUPPORTED);
	};


	inline jvxErrorType _return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl)
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	// =========================================================================

	// State handling functions
	//inline jvxErrorType _initialize(IjvxHost* hostRef = NULL)
	inline jvxErrorType _initialize(IjvxHiddenInterface* hostRef = NULL)
	{
		if(_common_set.theState == JVX_STATE_NONE)
		{
			_common_set.theHostRef = hostRef;
			_common_set.theState = JVX_STATE_INIT;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _select()
	{
		if(_common_set.theState == JVX_STATE_INIT)
		{
			_common_set.theState = JVX_STATE_SELECTED;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	// ===========================================================================

	inline jvxErrorType _activate()
	{
		if(_common_set.theState == JVX_STATE_SELECTED)
		{
			_common_set.theState = JVX_STATE_ACTIVE;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _activate_lock()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _activate();
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};

	// ===========================================================================

	jvxErrorType _is_ready(jvxBool* suc, jvxString** reasonIfNot)
	{
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{
			if(suc)
			{
				*suc = true;
			}
		}
		else
		{
			if(suc)
			{
				*suc = false;
			}
			std::string txt = "Component is in state ";
			txt += jvxState_txt(_common_set.theState);
			HjvxObject_produceString(reasonIfNot, txt);
		}
		return(JVX_NO_ERROR);
	};

	// ===========================================================================

	inline jvxErrorType _prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
	{
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{
			/*
			_common_set_processing.theNumberEndpoints = numEndpoints;
			if(numEndpoints)
			{
				JVX_SAFE_NEW_FLD(_common_set_processing.theEndpoints, IjvxDataProcessor*, _common_set_processing.theNumberEndpoints);
			}
			*/
			_common_set.theState = JVX_STATE_PREPARED;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _prepare_lock(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _prepare(theEndpoint, numEndpoints);
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};
	// ===========================================================================


	inline jvxErrorType _start()
	{
		if(_common_set.theState == JVX_STATE_PREPARED)
		{
			_common_set.theState = JVX_STATE_PROCESSING;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _start_lock()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _start();
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};

	// ===========================================================================

	inline jvxErrorType _stop()
	{
		if(_common_set.theState == JVX_STATE_PROCESSING)
		{
			_common_set.theState = JVX_STATE_PREPARED;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _stop_lock()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _stop();
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};

	// ===========================================================================

	inline jvxErrorType _postprocess()
	{
		if(_common_set.theState == JVX_STATE_PREPARED)
		{
			/*
			JVX_SAFE_DELETE_FLD(_common_set_processing.theEndpoints, IjvxDataProcessor*);
			_common_set_processing.theNumberEndpoints = 0;
			*/
			_common_set.theState = JVX_STATE_ACTIVE;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _postprocess_lock()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _postprocess();
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};

	// ===========================================================================

	inline jvxErrorType _deactivate()
	{
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{
			_common_set.theState = JVX_STATE_SELECTED;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _deactivate_lock()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		res = _deactivate();
		if(res != JVX_NO_ERROR)
		{
			JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		}
		return(res);
	};

	// ===========================================================================

	inline jvxErrorType _unselect()
	{
		if(_common_set.theState == JVX_STATE_SELECTED)
		{
			_common_set.theState = JVX_STATE_INIT;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _terminate()
	{
		if(_common_set.theState == JVX_STATE_INIT)
		{
			_common_set.theState = JVX_STATE_NONE;
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_WRONG_STATE);
	};

	inline jvxErrorType _lock_state()
	{
		JVX_LOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _unlock_state()
	{
		JVX_UNLOCK_MUTEX(_common_set._safeAccessStateBound.the_lock);
		return(JVX_NO_ERROR);
	};

	jvxErrorType _report_text_message(const char* txt, jvxReportPriority prio)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_common_set.theHostRef)
		{
			IjvxReport* theReport = NULL;
			res = _common_set.theHostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if((res == JVX_NO_ERROR) && theReport)
			{
				theReport->report_simple_text_message(txt, prio);
				_common_set.theHostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
		}
		else
		{
			res = JVX_ERROR_UNSUPPORTED;
		}
		return(res);
	};

	jvxErrorType _report_command_request(jvxCommandRequestType req)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_common_set.theHostRef)
		{
			IjvxReport* theReport = NULL;
			res = _common_set.theHostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if((res == JVX_NO_ERROR) && theReport)
			{
				theReport->report_command_request(req);
				_common_set.theHostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
		}
		else
		{
			res = JVX_ERROR_UNSUPPORTED;
		}
		return(res);
	};

	jvxErrorType _report_property_has_changed(jvxPropertyCategoryType cat, jvxInt16 idProp, jvxBool onlyCont, jvxComponentType cpTo = JVX_COMPONENT_UNKNOWN, jvxPropertyCallPurpose callpurp = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_common_set.theHostRef)
		{
			IjvxReport* theReport = NULL;
			res = _common_set.theHostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if((res == JVX_NO_ERROR) && theReport)
			{
				theReport->report_internals_have_changed(_common_set.theComponentType, _common_set.thisisme, cat, idProp, onlyCont, cpTo, callpurp);
				_common_set.theHostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
		}
		else
		{
			res = JVX_ERROR_UNSUPPORTED;
		}
		return(res);
	};

	jvxErrorType _report_take_over_property(jvxHandle* fld, jvxSize numElements, jvxDataFormat format, jvxSize offsetStart, jvxBool onlyContent, jvxPropertyCategoryType cat, jvxInt16 idProp, jvxComponentType cpTo)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_common_set.theHostRef)
		{
			IjvxReport* theReport = NULL;
			res = _common_set.theHostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if((res == JVX_NO_ERROR) && theReport)
			{
				res = theReport->report_take_over_property(_common_set.theComponentType, _common_set.thisisme, 
					fld, numElements, format, offsetStart, onlyContent, 
					cat, idProp, cpTo );
				_common_set.theHostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
		}
		else
		{
			res = JVX_ERROR_UNSUPPORTED;
		}
		return(res);
	};
	virtual jvxErrorType _number_info_tokens(jvxSize* num)
	{
		if(num)
		{
			*num = _common_set.theInformation.size();
		}
		return(JVX_NO_ERROR);
	};


	virtual jvxErrorType _info_token(jvxSize idx, jvxString** fldStr)
	{
		if(idx < _common_set.theInformation.size())
		{
			if(fldStr)
			{
				HjvxObject_produceString(fldStr, _common_set.theInformation[idx]);
			}
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_ID_OUT_OF_BOUNDS);
	};
};

#endif
