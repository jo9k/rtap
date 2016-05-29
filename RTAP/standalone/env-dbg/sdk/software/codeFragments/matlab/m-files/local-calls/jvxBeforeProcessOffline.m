function [] = jvxBeforeProcessOffline(isOffline)
     
	global inProcessing;
	jvx_handle = inProcessing.jvx_handle;

	[jvx_handle] = jvxBeforeProcessOffline_local(jvx_handle, isOffline);
	
	inProcessing.jvx_handle = jvx_handle;

	 