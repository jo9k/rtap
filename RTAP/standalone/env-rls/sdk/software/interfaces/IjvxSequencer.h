#ifndef __IJVXSEQUENCER_H__
#define __IJVXSEQUENCER_H__

JVX_INTERFACE IjvxSequencer_report
{
 public:
        virtual JVX_CALLINGCONVENTION ~IjvxSequencer_report(){};

	// Report the status of the sequencer if desired
	virtual jvxErrorType JVX_CALLINGCONVENTION report_event(jvxUInt64 event_mask, const char* description, jvxInt16 sequenceId, jvxInt16 stepId, jvxSequencerQueueType tp, jvxSequencerElementType stp = JVX_SEQUENCER_TYPE_COMMAND_NONE) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION sequencer_callback(jvxInt16 sequenceId, jvxInt16 stepId, jvxSequencerQueueType tp, jvxInt32 functionId) = 0; 
};

JVX_INTERFACE IjvxSequencer
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxSequencer(){};

	// ============================================================================
	// Edit sequences
	// ============================================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION state_sequencer(jvxState* theState) = 0;

	// Reset all sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION reset_sequences() = 0;

	// Add a new - then empty - sequence
	virtual jvxErrorType JVX_CALLINGCONVENTION add_sequence(const char* description) = 0;

	// Remove a previously added sequence
	virtual jvxErrorType JVX_CALLINGCONVENTION remove_sequence(jvxSize handleId) = 0;

	// Switch the order of two sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION switch_sequences(jvxSize idx1, jvxSize idx2) = 0;

	// Mark a sequence to be involved in the overall process
	virtual jvxErrorType JVX_CALLINGCONVENTION mark_sequence_process(jvxSize in_idxsequence, jvxBool runit) = 0;

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION number_sequences(jvxSize* num) = 0;

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION description_sequence(jvxSize idx, jvxString** description, jvxSize* numStepsRun, jvxSize* numStepsLeave, jvxBool*) = 0;

	// ============================================================================
	// Edit steps
	// ============================================================================

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
		jvxInt32 position = -1) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION edit_step_sequence(
		jvxSize in_idx_sequence,
		jvxSize in_idx_step,
		jvxSequencerQueueType tp,
		jvxSequencerElementType in_elementType,
		jvxComponentType in_targetComponentType,
		const char* description = "No description",
		jvxInt32 in_functionId =  -1,
		jvxInt64 timeout_ms = -1,
		const char* in_label_step = "UNKNOWN",
		const char* in_label_cond1 = "UNKNOWN",
		const char* in_label_cond2 = "UNKNWON",
		jvxBitField whatToSet = (jvxBitField)-1) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_step_sequence(jvxSize in_idx_sequence, jvxSequencerQueueType tp, jvxSize position) = 0;

	// Switch the order of two steps
	virtual jvxErrorType JVX_CALLINGCONVENTION switch_steps_sequence(jvxSize in_idxsequence, jvxSequencerQueueType tp, jvxSize idx1, jvxSize idx2) = 0;

	// Browse registered sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION description_step_sequence(jvxSize idx_seq, jvxSize idx_steps, jvxSequencerQueueType tp,
		jvxString** description,
		jvxSequencerElementType* out_elementType, jvxComponentType* out_targetComponentType,
		jvxInt32* out_functionId, jvxInt64* out_timeout_ms,  jvxString** label_name, jvxString** cond_label_true, jvxString** cond_label_false) = 0;

	// ============================================================================
	// Control process
	// ============================================================================

	// Run a process which is composed of a list of sequences
	virtual jvxErrorType JVX_CALLINGCONVENTION run_process(jvxInt64 period_ms, IjvxSequencer_report* report, jvxUInt64 eventMask, bool triggerExtern) = 0;

	// Initiate a termination of the current process. This will not wait until process is over
	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_abort_process() = 0;

	// Wait for a process to complete
	virtual jvxErrorType JVX_CALLINGCONVENTION wait_completion_process(jvxInt64 timeout_wait_ms) = 0;

	// Enable looping of currently setup process
	virtual jvxErrorType JVX_CALLINGCONVENTION enable_loop_process(jvxBool enable) = 0;

	// Return the current status of the sequencer
	virtual jvxErrorType JVX_CALLINGCONVENTION status_process(jvxSequencerStatus* status, jvxInt32* idxSequence, jvxSequencerQueueType* qTp, jvxInt32* idxStep, jvxBool* loopEnabled) = 0;

	// Functions specific to external trigger mode

	// If externally triggered, procede with next step
	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_step_process_extern(jvxInt64 timestamp_us, jvxSequencerElementType* sequencer_element_type_on_leave = NULL) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_complete_process_extern(jvxInt64 timestamp_us) = 0;

	// If externally triggered, find out where state machine is stuck
	virtual jvxErrorType JVX_CALLINGCONVENTION read_step_type_process_extern(jvxSequencerElementType* theElementType) = 0;

};

#endif
