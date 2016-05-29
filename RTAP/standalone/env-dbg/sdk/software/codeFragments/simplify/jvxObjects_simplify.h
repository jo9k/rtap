// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
// Interface CjvxObject
// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
virtual jvxErrorType JVX_CALLINGCONVENTION state(jvxState* stat){return(CjvxObject::_state(stat));};

virtual jvxErrorType JVX_CALLINGCONVENTION lock_state(){return(CjvxObject::_lock_state());};

virtual jvxErrorType JVX_CALLINGCONVENTION unlock_state(){return(CjvxObject::_unlock_state());};

virtual jvxErrorType JVX_CALLINGCONVENTION description(jvxString** str){return(CjvxObject::_description(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION descriptor(jvxString** str){return(CjvxObject::_descriptor(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION module_name(jvxString** str){return(CjvxObject::_module_name(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION feature_class(jvxBitField* ft){return(CjvxObject::_feature_class(ft));};

virtual jvxErrorType JVX_CALLINGCONVENTION version(jvxString** str){return(CjvxObject::_version(str));};

virtual jvxErrorType JVX_CALLINGCONVENTION request_specialization(jvxHandle** obj, jvxComponentType* cT, jvxBool* mult){return(CjvxObject::_requestComponentSpecialization(obj, cT, mult));};

virtual jvxErrorType JVX_CALLINGCONVENTION lasterror(jvxError** err){return(CjvxObject::_lasterror(err));};

virtual jvxErrorType JVX_CALLINGCONVENTION number_info_tokens(jvxSize* num){return(_number_info_tokens(num));};;

virtual jvxErrorType JVX_CALLINGCONVENTION info_token(jvxSize idx, jvxString** fldStr){return(_info_token(idx, fldStr));};



