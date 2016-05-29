// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
// Interface CjvxHiddenInterfacet
// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 

virtual jvxErrorType JVX_CALLINGCONVENTION request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl){return(_request_hidden_interface(tp, hdl));};

virtual jvxErrorType JVX_CALLINGCONVENTION return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl){return(_return_hidden_interface(tp, hdl));};