#ifndef __CJVXAUDIONODE_H__
#define __CJVXAUDIONODE_H__

#include "jvx.h"
#include "templates/common/CjvxObject.h"
#include "templates/common/CjvxDeallocate.h"
#include "templates/common/CjvxProperties.h"
#include "templates/common/CjvxSequencerControl.h"
#include "templates/common/CjvxDataProcessor.h"
#include "pcg_CjvxAudioNode_pcg.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

class CjvxAudioNode: public IjvxNode, public CjvxObject, public CjvxDeallocate,
	public IjvxProperties, public CjvxProperties,
	public IjvxSequencerControl, public CjvxSequencerControl,
	public IjvxDataProcessor, public CjvxDataProcessor,
	public IjvxConfiguration,
	public CjvxAudioNode_genpcg
{
protected:
	struct
	{
		struct
		{
			struct
			{
				jvxInt32 min;
				jvxInt32 max;
			} buffersize;

			struct
			{
				jvxInt16 min;
				jvxInt16 max;
			} number_input_channels;

			struct
			{
				jvxInt16 min;
				jvxInt16 max;
			} number_output_channels;

			struct
			{
				jvxInt32 min;
				jvxInt32 max;
			} samplerate;

			struct
			{
				jvxDataFormat min;
				jvxDataFormat max;
			} format;
		} preferred;

		struct
		{
			jvxSize buffersize;
			jvxInt16 number_input_channels;
			jvxInt16 number_output_channels;
			jvxInt32 samplerate;
			jvxDataFormat format;
		} processing;

		jvxBool acceptOnlyExactMatchLinkDataInput;
		jvxBool acceptOnlyExactMatchLinkDataOutput;
	} _common_set_audio_params;

public:

	// ===================================================================================================
	// ===================================================================================================
	JVX_CALLINGCONVENTION CjvxAudioNode(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxAudioNode();

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot);

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints);

	virtual jvxErrorType JVX_CALLINGCONVENTION start();

	virtual jvxErrorType JVX_CALLINGCONVENTION stop();

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess();

	// ===================================================================================================
	// Interface IjvxDataProcessor
	// ===================================================================================================
#ifdef ACCEPT_ONLY_ACCEPT_MATCH_DATA_STRUCT_INPUT
#pragma error ACCEPT_ONLY_ACCEPT_MATCH_DATA_STRUCT_INPUT
#endif

#ifdef ACCEPT_ONLY_ACCEPT_MATCH_DATA_STRUCT_OUTPUT
#pragma error ACCEPT_ONLY_ACCEPT_MATCH_DATA_STRUCT_INPUT
#endif

	virtual jvxErrorType JVX_CALLINGCONVENTION reference_object(IjvxObject** refObject);

	virtual jvxErrorType JVX_CALLINGCONVENTION act_as_component(jvxComponentType* iActAsComponent);

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare_sender_to_receiver(jvxLinkDataDescriptor* theData);

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare_complete_receiver_to_sender(jvxLinkDataDescriptor* theData);

	virtual jvxErrorType JVX_CALLINGCONVENTION supports_multithreading(jvxBool* supports);

	/* TODO: Add member functions for multithreading negotiations */

	virtual jvxErrorType JVX_CALLINGCONVENTION process_mt(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender, jvxSize* channelSelect , jvxSize numEntriesChannels, jvxInt32 offset_input, jvxInt32 offset_output, jvxInt32 numEntries);

	virtual jvxErrorType JVX_CALLINGCONVENTION reconfigured_receiver_to_sender(jvxLinkDataDescriptor* theData);

	virtual jvxErrorType JVX_CALLINGCONVENTION process_st(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender);

	virtual jvxErrorType JVX_CALLINGCONVENTION last_error_process(char* fld_text, jvxSize fldSize, jvxErrorType* err, jvxInt32* id_error, jvxLinkDataDescriptor* theData);

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess_sender_to_receiver(jvxLinkDataDescriptor* theData);

	virtual jvxErrorType JVX_CALLINGCONVENTION before_postprocess_receiver_to_sender(jvxLinkDataDescriptor* theData);

	// ===================================================================================================
	// Interface IjvxConfiguration
	// ===================================================================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION put_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionToContainAllSubsectionsForMe,
								     const char* filename,
								     jvxInt32 lineno);

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration(IjvxConfigProcessor* processor,
								     jvxHandle* sectionWhereToAddAllSubsections);


	// ===================================================================================================

#include "codeFragments/simplify/jvxStateMachine_simplify.h"

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxProperties_simplify.h"

#include "codeFragments/simplify/jvxSequencerControl_simplify.h"


#define JVX_INTERFACE_SUPPORT_DATAPROCESSOR
#define JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
#define JVX_INTERFACE_SUPPORT_PROPERTIES
#define JVX_INTERFACE_SUPPORT_CONFIGURATION
#include "codeFragments/simplify/jvxHiddenInterface_simplify.h"
#undef JVX_INTERFACE_SUPPORT_DATAPROCESSOR
#undef JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
#undef JVX_INTERFACE_SUPPORT_PROPERTIES
#undef JVX_INTERFACE_SUPPORT_CONFIGURATION

	jvxErrorType reportPreferredParameters(jvxPropertyCategoryType cat, jvxInt16 propId);
};

#ifdef JVX_PROJECT_NAMESPACE
}
#endif

#endif
