	virtual jvxErrorType JVX_CALLINGCONVENTION number_devices(jvxSize* num){return(CjvxTechnology::_number_devices(num));};

	virtual jvxErrorType JVX_CALLINGCONVENTION description_device(jvxSize idx, jvxString** str){return(CjvxTechnology::_description_device(idx, str));};

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_device(jvxSize idx, jvxString** str){return(CjvxTechnology::_descriptor_device(idx, str));};

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name_device(jvxSize idx, jvxString** str){return(CjvxTechnology::_module_name_device(idx, str));};
