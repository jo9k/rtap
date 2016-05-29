// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
// Interface CjvxDeallocate
// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxString*str){return(CjvxDeallocate::_deallocate(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxError*err){return(CjvxDeallocate::_deallocate(err));};

virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxStringList* str){return(CjvxDeallocate::_deallocate(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION deallocate(jvxDataList* dblLst){return(CjvxDeallocate::_deallocate(dblLst));};

