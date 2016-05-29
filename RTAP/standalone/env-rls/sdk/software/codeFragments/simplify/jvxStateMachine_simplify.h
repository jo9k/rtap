	virtual jvxErrorType JVX_CALLINGCONVENTION initialize(IjvxHiddenInterface* hostRef){return(CjvxObject::_initialize(hostRef));};

	virtual jvxErrorType JVX_CALLINGCONVENTION select(){return(CjvxObject::_select());};

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect()
	{
		if(_common_set.theState == JVX_STATE_PROCESSING)
		{
			stop();
		}
		if(_common_set.theState == JVX_STATE_PREPARED)
		{
			postprocess();
		}
		if(_common_set.theState == JVX_STATE_ACTIVE)
		{
			deactivate();
		}
		return(CjvxObject::_unselect());};

	virtual jvxErrorType JVX_CALLINGCONVENTION terminate()
	{
		jvxErrorType res = CjvxObject::_terminate();
#ifdef JVX_RESET_PROPERTIES_ON_TERMINATE

		// If we have a static library, the properties need to be reset in case of object termination
		if(res == JVX_NO_ERROR)
		{
			_reset_properties();
		}
#endif
		return(res);
	};

