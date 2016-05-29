	jvxErrorType request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl)
	{
		jvxErrorType res = JVX_NO_ERROR;

		switch(tp)
		{
#ifdef JVX_INTERFACE_SUPPORT_PROPERTIES
		case JVX_INTERFACE_PROPERTIES:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxProperties*>(this));
			break;

#endif

#ifdef JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
		case JVX_INTERFACE_SEQUENCERCONTROL:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxSequencerControl*>(this));
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_CONFIGURATION
		case JVX_INTERFACE_CONFIGURATION:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxConfiguration*>(this));
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_DATAPROCESSOR_SELECTOR
		case JVX_INTERFACE_DATAPROCESSOR_SELECTOR:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxDataProcessorSelector*>(this));
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_DATAPROCESSOR
		case JVX_INTERFACE_DATAPROCESSOR:
			*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxDataProcessor*>(this));
			break;
#endif

		default:
#ifdef JVX_INTERFACE_SUPPORT_BASE_CLASS
			res = JVX_INTERFACE_SUPPORT_BASE_CLASS::request_hidden_interface(tp, hdl);
#else
			res = _request_hidden_interface(tp, hdl);
#endif
		}
		return(res);
	};

	jvxErrorType return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl)
	{
		jvxErrorType res = JVX_NO_ERROR;

		switch(tp)
		{
#ifdef JVX_INTERFACE_SUPPORT_PROPERTIES
		case JVX_INTERFACE_PROPERTIES:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxProperties*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
		case JVX_INTERFACE_SEQUENCERCONTROL:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxSequencerControl*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;

#endif

#ifdef JVX_INTERFACE_SUPPORT_CONFIGURATION
		case JVX_INTERFACE_CONFIGURATION:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxConfiguration*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_DATAPROCESSOR
		case JVX_INTERFACE_DATAPROCESSOR:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxDataProcessor*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
#endif

#ifdef JVX_INTERFACE_SUPPORT_DATAPROCESSOR_SELECTOR
		case JVX_INTERFACE_DATAPROCESSOR_SELECTOR:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxDataProcessorSelector*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
#endif

		default:
#ifdef JVX_INTERFACE_SUPPORT_BASE_CLASS
			res = JVX_INTERFACE_SUPPORT_BASE_CLASS::return_hidden_interface(tp, hdl);
#else
			res = _return_hidden_interface(tp, hdl);
#endif
		}
		return(res);
	}
