#ifndef __CJVXHOST_H__
#define __CJVXHOST_H__

#include "jvx.h"
#include "templates/common/CjvxDeallocate.h"
#include "templates/common/CjvxObject.h"
#include "templates/common/CjvxProperties.h"

#include "templates/common/CjvxSequencer.h"
#include "templates/common/CjvxConfigurationLine.h"

#include "templates/common/CjvxHostTypeHandler.h"

#include "pcg_CjvxHost_pcg.h"

#define JVX_HBDX1_CHECK_CALLING_THREAD_ID \
	JVX_THREAD_ID theThreadId = JVX_GET_CURRENT_THREAD_ID(); \
	if(this->_common_set_host.threadId_registered != theThreadId) \
	{ \
		return(JVX_ERROR_THREADING_MISMATCH); \
	}



class CjvxHost: public IjvxHost, public IjvxHostTypeHandler, public IjvxSequencer, public CjvxObject, public CjvxDeallocate, public IjvxProperties, public CjvxProperties, public CjvxHostTypeHandler, public CjvxSequencer, public CjvxHost_genpcg
{
public:


protected:

	typedef struct
	{
		jvxHandle* theHdl;
		jvxInterfaceType selector[1];
	} oneInterface;

	typedef struct
	{
		jvxComponentType selector[1];
	} oneBlacklistEntry;

	struct
	{
		std::vector<oneInterface> externalInterfaces;

		struct
		{
			std::vector<oneOtherComponent> availableOtherComponents;
		} otherComponents;

		IjvxReport* reportUnit;
		IjvxReportOnConfig* reportOnConfig;

		JVX_THREAD_ID threadId_registered;

#ifdef QT_ALLOW_QT_WIDGETS
		IjvxQtWidgetHost* theWidgetHost;
#endif

		std::vector<oneBlacklistEntry> blacklist_load;
		jvxBool doUnload;

	} _common_set_host;


public:

	JVX_CALLINGCONVENTION CjvxHost(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxHost();

	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
	// Module control
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

//	virtual jvxErrorType JVX_CALLINGCONVENTION addSystemPath(const char*);

//	virtual jvxErrorType JVX_CALLINGCONVENTION clearSystemPath();

//	virtual jvxErrorType JVX_CALLINGCONVENTION systemPaths(jvxStringList** lstOnReturn);


	virtual jvxErrorType JVX_CALLINGCONVENTION initialize(IjvxHiddenInterface* theOtherHost);

	virtual jvxErrorType JVX_CALLINGCONVENTION add_external_component(IjvxObject* theObj, const char* locationDescription, jvxBool allowMultipleInstance, jvxInitObject_tp funcInit, jvxTerminateObject_tp funcTerm);

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_external_component(IjvxObject* theObj);

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_report_target(IjvxReport* hdl);

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_report_on_config(IjvxReportOnConfig* callbackStruct);

	virtual jvxErrorType JVX_CALLINGCONVENTION add_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype);

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype);

	virtual jvxErrorType JVX_CALLINGCONVENTION add_component_load_blacklist(jvxComponentType theTp);

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_component_load_blacklist(jvxComponentType theTp);

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_widget_host(jvxHandle* theWidgetHost, jvxWidgetHostType widgetType);

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot);


	virtual jvxErrorType JVX_CALLINGCONVENTION terminate();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	// Some state transitions are not supported as not used

	virtual jvxErrorType JVX_CALLINGCONVENTION select();

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect();


	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION start()
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION stop()
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess()
	{
		return(JVX_ERROR_UNSUPPORTED);
	};

	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
	// Control sub components main API
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --

	virtual jvxErrorType JVX_CALLINGCONVENTION number_components(jvxComponentType, jvxSize* num);

	virtual jvxErrorType JVX_CALLINGCONVENTION description_component(jvxComponentType, jvxSize idx, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_component(jvxComponentType, jvxSize idx, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name_component(jvxComponentType, jvxSize idx, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION feature_class_component(jvxComponentType, jvxSize idx, jvxBitField*);


	virtual jvxErrorType JVX_CALLINGCONVENTION select_component(jvxComponentType, jvxSize);

	virtual jvxErrorType JVX_CALLINGCONVENTION selection_component(jvxComponentType, jvxInt16*);

	virtual jvxErrorType JVX_CALLINGCONVENTION activate_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready_component(jvxComponentType, jvxBool* ready, jvxString** reasonIfNot);

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION state_component(jvxComponentType, jvxState*);


	virtual jvxErrorType JVX_CALLINGCONVENTION description_component(jvxComponentType, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_component(jvxComponentType, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name_component(jvxComponentType, jvxString**);

	virtual jvxErrorType JVX_CALLINGCONVENTION feature_class_component(jvxComponentType, jvxBitField*);


	virtual jvxErrorType JVX_CALLINGCONVENTION request_object_component(jvxComponentType, IjvxObject** );

	virtual jvxErrorType JVX_CALLINGCONVENTION return_object_component(jvxComponentType, IjvxObject* );


	virtual jvxErrorType JVX_CALLINGCONVENTION request_hidden_interface_component(jvxComponentType, jvxInterfaceType tp, jvxHandle** iface);

	virtual jvxErrorType JVX_CALLINGCONVENTION return_hidden_interface_component(jvxComponentType, jvxInterfaceType tp, jvxHandle* iface);

/*	virtual jvxErrorType JVX_CALLINGCONVENTION number_properties_component(jvxComponentType, jvxSize* num);

	virtual jvxErrorType JVX_CALLINGCONVENTION lock_properties_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION trylock_properties_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION unlock_properties_component(jvxComponentType);

	virtual jvxErrorType JVX_CALLINGCONVENTION description_property_component(jvxComponentType, jvxSize idx,
		jvxPropertyCategory* category, jvxUInt64* allowStateMask,
		jvxUInt64* allowThreadingMask, jvxDataFormat* format, jvxSize* num,
		jvxBool* readonly, jvxDecoderHintType* decTp, jvxSize* handleIdx, jvxString** descrion);

	virtual jvxErrorType JVX_CALLINGCONVENTION set_property_component(jvxComponentType, jvxHandle* fld, jvxSize numElements,
		jvxDataFormat , jvxUInt16 propId, jvxPropertyCategory category, jvxSize offsetStart);

	virtual jvxErrorType JVX_CALLINGCONVENTION get_property_component(jvxComponentType, jvxHandle* fld, jvxSize numElements,
		jvxDataFormat , jvxUInt16 propId, jvxPropertyCategory category, jvxSize offsetStart);
*/
	virtual jvxErrorType JVX_CALLINGCONVENTION request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl);

	virtual jvxErrorType JVX_CALLINGCONVENTION return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl);

		// ====================================================================================
	// Interface IjvxSequencer
	// ====================================================================================

		// Return the current status of the sequencer
	virtual jvxErrorType JVX_CALLINGCONVENTION state_sequencer(jvxState * stat)
	{
		return(CjvxSequencer::_state_sequencer(stat));
	};

	// Reset all sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION reset_sequences()
	{
		return(CjvxSequencer::_reset_sequences());
	};

	// Add a new - then empty - sequence
	virtual jvxErrorType JVX_CALLINGCONVENTION add_sequence(const char* description)
	{
		return(CjvxSequencer::_add_sequence(description));
	};

	// Remove a previously added sequence
	virtual jvxErrorType JVX_CALLINGCONVENTION remove_sequence(jvxSize handleId)
	{
		return(CjvxSequencer::_remove_sequence(handleId));
	};

	// Add a step element which is run
	virtual jvxErrorType JVX_CALLINGCONVENTION insert_step_sequence_at(
		jvxSize in_handleId_sequence,
		jvxSequencerQueueType tp,
		jvxSequencerElementType in_elementType,
		jvxComponentType in_targetComponentType,
		const char* description = "No description",
		jvxInt32 in_functionId = -1,
		jvxInt64 timeout_ms = -1,
		const char* in_label_step = "UNKNOWN",
		const char* in_label_cond1 = "UNKNOWN",
		const char* in_label_cond2 = "UNKNWON",
		jvxInt32 position = -1)
	{
		return(CjvxSequencer::_insert_step_sequence_at(in_handleId_sequence, tp, in_elementType, in_targetComponentType, description,in_functionId, timeout_ms, in_label_step, in_label_cond1, in_label_cond2, position));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION edit_step_sequence(
		jvxSize in_idx_sequence,
		jvxSize in_idx_step,
		jvxSequencerQueueType tp,
		jvxSequencerElementType in_elementType,
		jvxComponentType in_targetComponentType,
		const char* description = "No description",
		jvxInt32 in_functionId = -1,
		jvxInt64 timeout_ms = -1,
		const char* in_label_step = "UNKNOWN",
		const char* in_label_cond1 = "UNKNOWN",
		const char* in_label_cond2 = "UNKNWON",
		jvxBitField whatToSet = -1)
	{
		return(_edit_step_sequence(in_idx_sequence, in_idx_step, tp, in_elementType, in_targetComponentType, description, in_functionId,
			timeout_ms, in_label_step, in_label_cond1, in_label_cond2, whatToSet));
	};

		virtual jvxErrorType JVX_CALLINGCONVENTION remove_step_sequence(jvxSize in_idx_sequence, jvxSequencerQueueType tp, jvxSize in_idx_step)
	{
		return(_remove_step_sequence(in_idx_sequence, tp, in_idx_step));
	};
	// Switch the order of two steps
	virtual jvxErrorType JVX_CALLINGCONVENTION switch_steps_sequence(jvxSize in_handleId_sequence, jvxSequencerQueueType tp, jvxSize idx1, jvxSize idx2)
	{
		return(CjvxSequencer::_switch_steps_sequence(in_handleId_sequence, tp, idx1, idx2));
	};


	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_abort_process()
	{
		return(CjvxSequencer::_trigger_abort_process());
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION enable_loop_process(jvxBool enable)
	{
		return(_enable_loop_process(enable));
	};

	// Wait for a certain process to complete
	virtual jvxErrorType JVX_CALLINGCONVENTION wait_completion_process(jvxInt64 timeout_wait_ms)
	{
		return(CjvxSequencer::_wait_completion_process(timeout_wait_ms));
	};

	// Return the current status of the sequencer
	virtual jvxErrorType JVX_CALLINGCONVENTION status_process(jvxSequencerStatus * status,jvxInt32 * seqId, jvxSequencerQueueType* qTp, jvxInt32 * stepId,jvxBool* loop_enabled)
	{
		return(CjvxSequencer::_status_process(status, seqId,qTp, stepId,loop_enabled));
	};

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION number_sequences(jvxSize* num)
	{
		return(_number_sequences(num));
	};

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION description_sequence(jvxSize idx, jvxString** description, jvxSize* numStepsRun, jvxSize* numStepsLeave, jvxBool* markedForProcess)
	{
		return(_description_sequence(idx, description, numStepsRun, numStepsLeave, markedForProcess));
	};

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION description_step_sequence(jvxSize idx_seq, jvxSize idx_step, jvxSequencerQueueType tp, jvxString** description,
		jvxSequencerElementType* out_elementType, jvxComponentType* out_targetComponentType,
		jvxInt32* out_functionId, jvxInt64* out_timeout_ms, jvxString** label_name, jvxString** cond_label_true, jvxString** cond_label_false)
	{
		return(_description_step_sequence(idx_seq, idx_step, tp, description, out_elementType, out_targetComponentType, out_functionId, out_timeout_ms,
			label_name, cond_label_true, cond_label_false));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION mark_sequence_process(jvxSize in_idxsequence, jvxBool runit)
	{
		return(_mark_sequence_process(in_idxsequence, runit));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION switch_sequences(jvxSize idx1, jvxSize idx2)
	{
		return(_switch_sequences(idx1, idx2));
	};

	// Run a process which is composed of a list of sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION run_process(jvxInt64 period_ms, IjvxSequencer_report* report, jvxUInt64 eventMask, bool triggerExtern = false)
	{
		return(_run_process(period_ms, report, reinterpret_cast<jvxHandle*>(this), eventMask, triggerExtern));
	};

	//! External trigger of sequencer if externally triggered
	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_step_process_extern(jvxInt64 timestamp_us, jvxSequencerElementType* sequencer_element_type_on_leave)
	{
		return(_trigger_step_process_extern(timestamp_us, sequencer_element_type_on_leave));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_complete_process_extern(jvxInt64 timestamp_us)
	{
		return(_trigger_complete_process_extern(timestamp_us));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION read_step_type_process_extern(jvxSequencerElementType* theElementType)
	{
		return(_read_step_type_process_extern(theElementType));
	};


	// ====================================================================================
	// Interface: IjvxHostTypeHandler
	// ====================================================================================

	jvxErrorType add_type_host(jvxComponentType* tp, jvxSize numtp, const char* description, const char* tokenConfig, jvxComponentTypeClass classType)
	{
		return(_add_type_host(static_cast<IjvxObject*>(this), tp, numtp, description, tokenConfig, classType));
	};

	jvxErrorType number_types_host(jvxSize* num, jvxComponentTypeClass classType)
	{
		return(_number_types_host(num, classType));
	};

	jvxErrorType description_type_host(jvxSize idx, jvxString** theDescription, jvxString** theTokenInConfig, jvxComponentType* tp, jvxSize numtp, jvxComponentTypeClass classType)
	{
		return(_description_type_host(idx, theDescription, theTokenInConfig, tp, numtp, classType));
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_all_host_types()
	{
		return(_remove_all_host_types());
	};

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxProperties_simplify.h"
	
	virtual jvxErrorType JVX_CALLINGCONVENTION clear_configuration_entries(){return(JVX_ERROR_UNSUPPORTED);};

	virtual jvxErrorType JVX_CALLINGCONVENTION set_configuration_entry(const char* entryname, jvxHandle* fld, jvxConfigSectionTypes tp, jvxInt32 id){return(JVX_ERROR_UNSUPPORTED);};

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration_entry(const char* entryname, jvxHandle** fld, jvxConfigSectionTypes* tp, jvxSize id){return(JVX_ERROR_UNSUPPORTED);};

	// ==============================================================================
	// Some local function definitions
	// ==============================================================================

protected:

#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
	virtual jvxErrorType loadAllComponents();
	virtual jvxErrorType unloadAllComponents();
#endif
	//virtual jvxErrorType removeAllRemainingComponents();


	void reset();

	// =======================================================================================
	// Some private member functions
	// =======================================================================================

	//jvxErrorType prepareScannerDevice(IjvxScannerDevice* theDevice);
	jvxErrorType prepareOneDevice(jvxComponentType tp);

	static jvxErrorType cb_startup(jvxHandle* privateData_queue, jvxInt64 timestamp_us);

	static jvxErrorType cb_period_expired(jvxHandle* privateData_queue, jvxInt64 timestamp_us);

	static jvxErrorType cb_wokeup(jvxHandle* privateData_queue, jvxInt64 timestamp_us);

	static jvxErrorType cb_stopped(jvxHandle* privateData_queue, jvxInt64 timestamp_us);

};

#endif
