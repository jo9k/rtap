#ifndef __CJVXAUDIODEVICE_H__
#define __CJVXAUDIODEVICE_H__

#include "jvx.h"
#include "templates/common/CjvxObject.h"
#include "templates/common/CjvxDeallocate.h"
#include "templates/common/CjvxProperties.h"
#include "templates/common/CjvxSequencerControl.h"
#include "templates/common/CjvxDataProcessorSelector.h"

#include "pcg_CjvxAudioDevice_pcg.h"


#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

class CjvxAudioDevice: public IjvxDevice, public CjvxObject, public CjvxDeallocate,
	public IjvxProperties, public CjvxProperties,
	public IjvxSequencerControl, public CjvxSequencerControl,
	public IjvxConfiguration,
	public CjvxAudioDevice_genpcg
{
protected:

	struct
	{
		/*
		jvxInt32 samplerate;
		jvxInt32 buffersize;
		jvx_oneSelectionOption_cpp inputchannels;
		jvx_oneSelectionOption_cpp outputchannels;
		*/
		IjvxDataProcessor* referene_audio_node;
		std::vector<jvxDataFormat> formats;
	} _common_set_audio_device;

    struct
    {
        jvxInt32 samplerate;
        jvxInt32 buffersize;
        jvxDataFormat format;
        jvxInt32 numInputs;
        jvxInt32 numOutputs;
        jvxSize minChanCntInput;
        jvxSize minChanCntOutput;
        jvxSize maxChanCntInput;
        jvxSize maxChanCntOutput;
        jvxBitField inChannelsSelectionMask;
        jvxBitField outChannelsSelectionMask;
    } _inproc;

public:

	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
	// Component class interface member functions
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
	JVX_CALLINGCONVENTION CjvxAudioDevice(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxAudioDevice();

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot)
	{
		return(_is_ready(suc, reasonIfNot));
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints);

	virtual jvxErrorType JVX_CALLINGCONVENTION start()
	{
		return(_start());
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION stop()
	{
		return(_stop());
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	// Interface properties

	virtual jvxErrorType JVX_CALLINGCONVENTION number_properties(jvxSize* num)
	{
		return(_number_properties(num));
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION get_revision(jvxSize* revision)
	{
		return _get_revision(revision);
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION lock_properties()
	{
		return(_lock_properties());
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION trylock_properties()
	{
		return(_trylock_properties());
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION unlock_properties()
	{
		return(_unlock_properties());
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

	virtual jvxErrorType JVX_CALLINGCONVENTION set_property_ext_ref(jvxHandle* fld,
		jvxSize numElements,
		jvxDataFormat format,					 
		const char* descriptor,
		jvxPropertyDecoderHintType htTp)
	{
		return(_set_property_ext_ref(fld,
			numElements,
			format,
			descriptor,
			htTp));
	}


	virtual jvxErrorType JVX_CALLINGCONVENTION set_property(jvxHandle* fld,
								jvxSize numElements,
								jvxDataFormat format,
								jvxSize propId,
								jvxPropertyCategoryType category,
								jvxSize offsetStart,
								jvxBool contentOnly,
								jvxPropertyCallPurpose callPurpose);

	virtual jvxErrorType JVX_CALLINGCONVENTION set_property(jvxHandle* fld,
					  jvxSize offsetStart,
					  jvxSize numElements,
					  jvxDataFormat format,
					  jvxBool contentOnly,
					  const char* descriptor)
	{
		return(_set_property(fld,offsetStart, numElements,format,contentOnly, descriptor, static_cast<IjvxObject*>(this)));
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
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION get_property(jvxHandle* fld,
					  jvxSize offsetStart,
					  jvxSize numElements,
					  jvxDataFormat format,
					  jvxBool contentOnly,
					  const char* descriptor)
	{
		return(_get_property(fld,offsetStart, numElements,format,contentOnly, descriptor, static_cast<IjvxObject*>(this)));
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION request_takeover_property(jvxComponentType thisismytype,
									     IjvxObject* thisisme,
									     jvxHandle* fld,
									     jvxSize numElements,
									     jvxDataFormat format,
									     jvxSize offsetStart,
									     jvxBool onlyContent,
									     jvxPropertyCategoryType cat,
									     jvxInt16 propId)
	{return(_request_takeover_property(thisismytype,
					   thisisme,
					   fld,
					   numElements,
					   format,
					   offsetStart,
					   onlyContent,
					   cat,
					   propId));
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION freeze_properties(jvxSize *props,jvxPropertyCategoryType *cats,jvxSize num)
	{
		return(_freeze_properties(props,cats,num));
	}

	virtual jvxErrorType JVX_CALLINGCONVENTION unfreeze_properties(jvxSize *props,jvxPropertyCategoryType *cats,jvxSize num)
	{
		return(_unfreeze_properties(props,cats,num));
	}

	// Interface IjvxConfiguration

	virtual jvxErrorType JVX_CALLINGCONVENTION put_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionToContainAllSubsectionsForMe,
								     const char* filename = "",
								     jvxInt32 lineno = -1 );

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionWhereToAddAllSubsections);

	// All other interfaces

#define JVX_RESET_PROPERTIES_ON_TERMINATE
#include "codeFragments/simplify/jvxStateMachine_simplify.h"
#undef JVX_RESET_PROPERTIES_ON_TERMINATE

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxSequencerControl_simplify.h"

#define JVX_INTERFACE_SUPPORT_CONFIGURATION
#define JVX_INTERFACE_SUPPORT_PROPERTIES
#define JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
#include "codeFragments/simplify/jvxHiddenInterface_simplify.h"
#undef JVX_INTERFACE_SUPPORT_CONFIGURATION
#undef JVX_INTERFACE_SUPPORT_PROPERTIES
#undef JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL

	void updateDependentVariables_lock(jvxSize propId,
					   jvxPropertyCategoryType category,
					   jvxBool updateAll,
					   jvxPropertyCallPurpose callPurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC);

	virtual jvxErrorType activate_lock();

//	virtual jvxErrorType prepare_lock();
//	virtual jvxErrorType prepare_unlock();

//	virtual jvxErrorType postprocess_lock();
//	virtual jvxErrorType postprocess_unlock();

	virtual jvxErrorType deactivate_lock();

};

#ifdef JVX_PROJECT_NAMESPACE
}
#endif

#endif
