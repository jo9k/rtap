
virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot)
{
	return(JVX_ERROR_UNSUPPORTED);
};


virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
{
	return(JVX_ERROR_UNSUPPORTED);
};


virtual jvxErrorType JVX_CALLINGCONVENTION start()
{
	return(JVX_ERROR_UNSUPPORTED);
};

virtual jvxErrorType JVX_CALLINGCONVENTION stop()
{
	return(JVX_ERROR_UNSUPPORTED);
};

virtual jvxErrorType JVX_CALLINGCONVENTION postprocess()
{
	return(JVX_ERROR_UNSUPPORTED);
};