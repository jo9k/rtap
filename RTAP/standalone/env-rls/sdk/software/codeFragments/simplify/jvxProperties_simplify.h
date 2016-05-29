// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
// Interface CjvxProperties
// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

virtual jvxErrorType JVX_CALLINGCONVENTION number_properties(jvxSize* num)
{
	return _number_properties(num);
}

virtual jvxErrorType JVX_CALLINGCONVENTION get_revision(jvxSize* revision)
{
	return _get_revision(revision);
}

virtual jvxErrorType JVX_CALLINGCONVENTION lock_properties()
{
	return _lock_properties();
}

virtual jvxErrorType JVX_CALLINGCONVENTION trylock_properties()
{
	return _trylock_properties();
}

virtual jvxErrorType JVX_CALLINGCONVENTION unlock_properties()
{
	return _unlock_properties();
}

virtual jvxErrorType JVX_CALLINGCONVENTION description_property(jvxSize idx,
								jvxPropertyCategoryType* category,
								jvxUInt64* settype,
								jvxUInt64* stateAccess,
								jvxDataFormat* format,
								jvxSize* num,
								jvxPropertyAccessType* accessType,
								jvxPropertyDecoderHintType* decTp,
								jvxSize* globalIdx,
								jvxPropertyContext* ctxt,
								jvxString** name,
								jvxString** description,
								jvxString** descriptor,
								jvxBool* isValid)
{
	return(_description_property(idx,
				     category,
				     settype,
				     stateAccess,
				     format,
				     num,
				     accessType,
				     decTp,
				     globalIdx,
				     ctxt,
					 name,
				     description,
				     descriptor,
				     isValid));
}

virtual jvxErrorType JVX_CALLINGCONVENTION description_property(
						   const char* descriptor,
						   jvxDataFormat* format,
						   jvxSize* num,
   						   jvxSize* handleIdx, 
						   jvxPropertyCategoryType* category)
{
	return(_description_property(descriptor, format,
						   num, handleIdx, category));
}

virtual jvxErrorType JVX_CALLINGCONVENTION set_property_ext_ref(jvxHandle* fld,
								jvxBool* isValid,
								jvxSize numElements,
								jvxDataFormat format,
								jvxSize propId,
								jvxPropertyCategoryType category,
								jvxPropertyDecoderHintType htTp)
{
	return(_set_property_ext_ref( fld,
				      isValid,
				      numElements,
				      format,
				      propId,
				      category, 
					  htTp));
}

virtual jvxErrorType JVX_CALLINGCONVENTION set_property(jvxHandle* fld,
							jvxSize numElements,
							jvxDataFormat format,
							jvxSize propId,
							jvxPropertyCategoryType category,
							jvxSize offsetStart,
							jvxBool contentOnly,
							jvxPropertyCallPurpose callPurpose)
{
	return(_set_property(fld,
			     numElements,
			     format,
			     propId,
			     category,
			     offsetStart,
			     contentOnly,
			     callPurpose,
			     static_cast<IjvxObject*>(this)));
}

virtual jvxErrorType JVX_CALLINGCONVENTION get_property(jvxHandle* fld,
							jvxSize numElements,
							jvxDataFormat format,
							jvxSize propId,
							jvxPropertyCategoryType category,
							jvxSize offsetStart,
							jvxBool contentOnly,
							jvxBool* isValid,
							jvxPropertyAccessType* accessType,
							jvxPropertyCallPurpose callPurpose)
{
	return(_get_property(fld,
			     numElements,
			     format,
			     propId,
			     category,
			     offsetStart,
			     contentOnly,
			     isValid,
			     accessType,
			     callPurpose,
			     static_cast<IjvxObject*>(this)));
}

virtual jvxErrorType JVX_CALLINGCONVENTION request_takeover_property(jvxComponentType tpFrom,
								     IjvxObject* objFrom,
								     jvxHandle* fld,
								     jvxSize numElements,
								     jvxDataFormat format,
								     jvxSize offsetStart,
								     jvxBool onlyContent,
								     jvxPropertyCategoryType category,
								     jvxInt16 propId)

{
	return(_request_takeover_property(tpFrom,
					  objFrom,
					  fld,
					  numElements,
					  format,
					  offsetStart,
					  onlyContent,
					  category,
					  propId));
}

virtual jvxErrorType JVX_CALLINGCONVENTION freeze_properties(jvxSize *props,
							     jvxPropertyCategoryType *cats,
							     jvxSize num)
{
	return(_freeze_properties(props,
				  cats,
				  num));
}

virtual jvxErrorType JVX_CALLINGCONVENTION unfreeze_properties(jvxSize *props,
							       jvxPropertyCategoryType *cats,
							       jvxSize num)
{
	return(_unfreeze_properties(props,
				    cats,
				    num));
}
