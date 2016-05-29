#ifndef __IJVXPROPERTIES_H__
#define __IJVXPROPERTIES_H__

JVX_INTERFACE IjvxProperties
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxProperties(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION number_properties(jvxSize* num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION get_revision(jvxSize* revision) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION lock_properties() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trylock_properties() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unlock_properties() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_property(jvxSize idx,
									jvxPropertyCategoryType* category,
									jvxUInt64* allowStateMask,
									jvxUInt64* allowThreadingMask,
									jvxDataFormat* format,
									jvxSize* num,
									jvxPropertyAccessType* accessType,
									jvxPropertyDecoderHintType* decTp,
									jvxSize* handleIdx,
									jvxPropertyContext* context,
									jvxString** name, 
									jvxString** description,
									jvxString** descriptor,
									bool* isValid = NULL) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_property(
						   const char* descriptor,
						   jvxDataFormat* format = NULL,
						   jvxSize* num = NULL,
   						   jvxSize* handleIdx = NULL, 
						   jvxPropertyCategoryType* category = NULL) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION set_property(jvxHandle* fld,
								jvxSize numElements,
								jvxDataFormat,
								jvxSize handleIdx,
								jvxPropertyCategoryType category,
								jvxSize offsetStart,
								jvxBool onlyContent,
								jvxPropertyCallPurpose callPurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION get_property(jvxHandle* fld,
								jvxSize numElements,
								jvxDataFormat,
								jvxSize handleIdx,
								jvxPropertyCategoryType category,
								jvxSize offsetStart,
								jvxBool onlyContent,
								jvxBool* isValid = NULL,
								jvxPropertyAccessType* accessType = NULL,
								jvxPropertyCallPurpose callPurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC) = 0;

	
	virtual jvxErrorType JVX_CALLINGCONVENTION set_property_ext_ref(jvxHandle* fld,
								jvxBool* isValid,
								jvxSize numElements,
								jvxDataFormat format,
								jvxSize propId,
								jvxPropertyCategoryType category,
								jvxPropertyDecoderHintType htTp = JVX_PROPERTY_DECODER_MULTI_CHANNEL_CIRCULAR_BUFFER) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION freeze_properties(jvxSize* handleIdx,
								     jvxPropertyCategoryType* category,
								     jvxSize num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unfreeze_properties(jvxSize* handleIdx,
								       jvxPropertyCategoryType* category,
								       jvxSize num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION request_takeover_property(jvxComponentType tpFrom,
									     IjvxObject* objFrom,
									     jvxHandle* fld,
									     jvxSize numElements,
									     jvxDataFormat format,
									     jvxSize offsetStart,
									     jvxBool onlyContent,
									     jvxPropertyCategoryType category,
									     jvxInt16 propId) = 0;


};

#endif
