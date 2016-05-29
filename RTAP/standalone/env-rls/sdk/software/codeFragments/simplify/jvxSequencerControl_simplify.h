	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
	// Interface CjvxConditions
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
	
virtual jvxErrorType JVX_CALLINGCONVENTION number_conditions(jvxSize* num){return(_number_conditions(num));};

virtual jvxErrorType JVX_CALLINGCONVENTION description_condition(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* statesAllowed){return(_description_condition(idx, uniqueId, description, statesAllowed));};

virtual jvxErrorType JVX_CALLINGCONVENTION status_condition(jvxSize uniqueId, jvxBool* fulfilled){return(_status_condition(uniqueId, fulfilled, static_cast<IjvxObject*>(this)));};

virtual jvxErrorType JVX_CALLINGCONVENTION number_commands(jvxSize* num){return(_number_commands(num));};
	
virtual jvxErrorType JVX_CALLINGCONVENTION description_command(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* statesAllowed) {return(_description_command(idx, uniqueId, description, statesAllowed));};

virtual jvxErrorType JVX_CALLINGCONVENTION trigger_command(jvxSize uniqueId){return(_trigger_command(uniqueId, static_cast<IjvxObject*>(this)));};
	
virtual jvxErrorType JVX_CALLINGCONVENTION number_relative_jumps(jvxSize* num){return(_number_relative_jumps(num));};
	
virtual jvxErrorType JVX_CALLINGCONVENTION description_relative_jump(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid){return(_description_relative_jump(idx, uniqueId, description, stateValid));};

virtual jvxErrorType JVX_CALLINGCONVENTION trigger_relative_jump(jvxSize uniqueId, jvxSize* pc_increment){return(_trigger_relative_jump(uniqueId, pc_increment, static_cast<IjvxObject*>(this)));};
