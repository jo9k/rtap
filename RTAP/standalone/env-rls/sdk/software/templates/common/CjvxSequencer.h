#ifndef _CJVXSEQUENCER_H__
#define _CJVXSEQUENCER_H__

#include "typedefs/TjvxSequencer_cpp.h"
#include "jvx_threads.h"

#include "helpers/tools/HjvxReadConfigEntries.h"
#include "helpers/tools/HjvxWriteConfigEntries.h"

#define JVX_CONFIG_SEQUENCER_SEQ_SECTION "sequencer_section"
#define JVX_CONFIG_SEQUENCER_SEQ_SUBSECTION_PREFIX  "sequencer_subsection_"

#define JVX_CONFIG_SEQUENCER_SEQ_NAMES "sequencer_sequence_names"
#define JVX_CONFIG_SEQUENCER_SEQ_MARKED_FLAG "sequencer_marked_for_process"

#define JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX "sequencer_sequence_steps_"

#define JVX_CONFIG_ONE_SEQUENCE_STEP_DECSCS "sequencer_sequence_step_descriptions"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL "sequencer_sequence_step_label"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_TRUE "sequencer_sequence_step_label_true"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_FALSE "sequencer_sequence_step_label_false"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_TYPES "sequencer_sequence_step_types"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_COMPONENTS "sequencer_sequence_step_components"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_COMMAND_IDS "sequencer_sequence_step_command_ids"
#define JVX_CONFIG_ONE_SEQUENCE_STEP_TIMEOUTS "sequencer_sequence_step_timeouts"

#define JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION \
	if(this->_common_set_sequencer.theState >= JVX_STATE_PROCESSING) \
	{ \
		return(JVX_ERROR_SEQUENCER_TOOK_CONTROL); \
	}


class CjvxSequencer
{
protected:
    struct
	{
        setOfSequences theSequences;
		JVX_MUTEX_HANDLE theAccess;
		jvxState theState;
		jvxSize uniqueId;

		jvx_thread_handler cb_thread_handler;
		jvxHandle* thread_handle;

		struct
		{
			jvxInt32 idxSequence;
			jvxInt32 idxStep;
			jvxBool loop_enabled;
			IjvxHost* hostRef;
			jvxUInt64 eventMask;
			IjvxSequencer_report* report;
			jvxSequencerStatus theSeqState;
			jvxSequencerQueueType currentQueueTp;
			jvxInt64 timestamp_store;
			bool externalTrigger;
			jvxHandle* privateData;
			struct
			{
				 jvxSequencerElementType step_tp;
			} thisStep;

		} inOperation;

	} _common_set_sequencer;

public:

	CjvxSequencer()
	{
		JVX_INITIALIZE_MUTEX(_common_set_sequencer.theAccess);
		_common_set_sequencer.theState = JVX_STATE_INIT;
		_common_set_sequencer.uniqueId = 1;
		_common_set_sequencer.inOperation.idxSequence = -1;
		_common_set_sequencer.inOperation.idxStep = -1;
		_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_NONE;
		_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_NONE;


	}

	~CjvxSequencer()
	{
		JVX_TERMINATE_MUTEX(_common_set_sequencer.theAccess);
	}

	virtual jvxErrorType _init_sequences()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_INIT)
		{
			_common_set_sequencer.theState = JVX_STATE_ACTIVE;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _state_sequencer(jvxState * stat)
	{
		if(stat)
		{
			*stat = _common_set_sequencer.theState;
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType _terminate_sequences()
	{
		jvxBool stopThread = false;
		jvxErrorType res = JVX_NO_ERROR;

		this->_trigger_abort_process();
		this->_wait_completion_process(JVX_INFINITE);
		this->_reset_sequences();

		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		_common_set_sequencer.theState = JVX_STATE_INIT;
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);

		return(res);
	};

	virtual jvxErrorType _reset_sequences()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			_common_set_sequencer.theSequences.all_sequences.clear();
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Add a new - then empty - sequence
	virtual jvxErrorType _add_sequence(const char* description)
	{
		jvxErrorType res = JVX_NO_ERROR;

		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			std::vector<oneSequence>::iterator elm = _common_set_sequencer.theSequences.all_sequences.begin();
			elm = findSequenceNameInList(_common_set_sequencer.theSequences.all_sequences, description);
			if(elm == _common_set_sequencer.theSequences.all_sequences.end())
			{
				oneSequence newElm;
				newElm.currentState = JVX_SEQUENCE_STATE_NONE;
				newElm.description = description;
				//newElm.elmActive;
				//newElm.lstOfEnterSteps;
				//newElm.lstOfLeaveSteps;
				//newElm.lstOfRunningSteps;
				newElm.timer_since_last_increment = -1;
				newElm.stepIdCnt = 0;
				newElm.markedForProcess = false;

				_common_set_sequencer.theSequences.all_sequences.push_back(newElm);
			}
			else
			{
				res = JVX_ERROR_DUPLICATE_ENTRY;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Add a new - then empty - sequence
	virtual jvxErrorType _find_sequence_id_name(const char* description, jvxInt16* id_out)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;

		if(id_out)
		{
			*id_out = -1;
		}

		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			for(i = 0; i < _common_set_sequencer.theSequences.all_sequences.size(); i++)
			{
				if(_common_set_sequencer.theSequences.all_sequences[i].description == description)
				{
					if(id_out)
					{
						*id_out = (jvxInt16)i;
					}
					break;
				}
			}
			if(id_out < 0)
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Remove a previously added sequence
	virtual jvxErrorType _remove_sequence(jvxSize idx)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			std::vector<oneSequence>::iterator elm = _common_set_sequencer.theSequences.all_sequences.begin();
			elm = findSequenceIdInList(_common_set_sequencer.theSequences.all_sequences, idx);
			if(elm != _common_set_sequencer.theSequences.all_sequences.end())
			{
				_common_set_sequencer.theSequences.all_sequences.erase(elm);
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Add a step element which is run
	virtual jvxErrorType _insert_step_sequence_at(
		jvxSize idx,
		jvxSequencerQueueType queueType,
		 jvxSequencerElementType in_elementType,
		jvxComponentType in_targetComponentType,
		const char* description = "No description",
		jvxInt32 in_functionId = -1,
		jvxInt64 timeout_ms = -1,
		const char* in_label_step = "UNKNOWN",
		const char* in_label_cond_true = "UNKNOWN",
		const char* in_label_cond_false = "UNKNWON",
		jvxInt32 posi = -1)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneSequenceStep>* theList = NULL;
		std::vector<oneSequenceStep> newList;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			if(idx < _common_set_sequencer.theSequences.all_sequences.size())
			{
				oneSequenceStep step;
				step.step_tp = in_elementType;
				step.step_target = in_targetComponentType;
				step.step_function_id = in_functionId;
				step.step_label = in_label_step;
				step.step_cond_label_true = in_label_cond_true;
				step.step_cond_label_false = in_label_cond_false;
				step.step_timeout_ms = timeout_ms;
				step.description = description;

				switch(queueType)
				{
				case JVX_SEQUENCER_QUEUE_TYPE_RUN:
					theList = &_common_set_sequencer.theSequences.all_sequences[idx].lstOfRunningSteps;
					break;
				case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
					theList = &_common_set_sequencer.theSequences.all_sequences[idx].lstOfLeaveSteps;
					break;
				default:
					assert(0);
				}
				if((posi >= 0) && (posi < theList->size()))
				{
					for(i = 0; i < posi; i++)
					{
						newList.push_back((*theList)[i]);
					}
					newList.push_back(step);

					for(; i < theList->size(); i++)
					{
						newList.push_back((*theList)[i]);
					}

					switch(queueType)
					{
					case JVX_SEQUENCER_QUEUE_TYPE_RUN:
						_common_set_sequencer.theSequences.all_sequences[idx].lstOfRunningSteps = newList;
						break;
					case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
						_common_set_sequencer.theSequences.all_sequences[idx].lstOfLeaveSteps = newList;
						break;
					default:
						assert(0);
					}
				}
				else
				{
					(*theList).push_back(step);
				}
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _edit_step_sequence(
		jvxSize idxsequence,
		jvxSize idxstep,
		jvxSequencerQueueType tp,
		 jvxSequencerElementType in_elementType,
		jvxComponentType in_targetComponentType,
		const char* description = "No description",
		jvxInt32 in_functionId = -1,
		jvxInt64 timeout_ms = -1,
		const char* in_label_step = "UNKNOWN",
		const char* in_label_cond_true = "UNKNOWN",
		const char* in_label_cond_false = "UNKNWON",
		jvxBitField whatToSet = -1)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneSequenceStep>* theList = NULL;
		std::vector<oneSequenceStep> newList;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			if(idxsequence < _common_set_sequencer.theSequences.all_sequences.size())
			{
				switch(tp)
				{
				case JVX_SEQUENCER_QUEUE_TYPE_RUN:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfRunningSteps;
					break;
				case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfLeaveSteps;
					break;
				default:
					assert(0);
				}
				if(idxstep < theList->size())
				{
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_DESCRIPTION)
					{
						(*theList)[idxstep].description = description;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_LABEL_FALSE)
					{
						(*theList)[idxstep].step_cond_label_false = in_label_cond_false;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_LABEL_TRUE)
					{
						(*theList)[idxstep].step_cond_label_true = in_label_cond_true;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_FID)
					{
						(*theList)[idxstep].step_function_id = in_functionId;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_LABEL)
					{
						(*theList)[idxstep].step_label = in_label_step;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_TARGET)
					{
						(*theList)[idxstep].step_target = in_targetComponentType;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_TIMEOUT)
					{
						(*theList)[idxstep].step_timeout_ms = timeout_ms;
					}
					if(whatToSet & JVX_SEQUENCER_EDIT_STEP_TYPE)
					{
						(*theList)[idxstep].step_tp = in_elementType;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _switch_steps_sequence(jvxSize idxsequence, jvxSequencerQueueType tp, jvxSize idx1, jvxSize idx2)
	{
		jvxInt16 i;
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneSequenceStep>* theList = NULL;
		std::vector<oneSequenceStep> newList;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			if(idxsequence < _common_set_sequencer.theSequences.all_sequences.size())
			{
				switch(tp)
				{
				case JVX_SEQUENCER_QUEUE_TYPE_RUN:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfRunningSteps;
					break;
				case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfLeaveSteps;
					break;
				default:
					assert(0);
				}

				jvxInt16 minIdx = (jvxInt16)JVX_MIN(idx1, idx2);
				jvxInt16 maxIdx = (jvxInt16)JVX_MAX(idx1, idx2);
				if(minIdx < maxIdx)
				{
					if((minIdx < (jvxInt16)theList->size()) && (maxIdx < (jvxInt16)theList->size()))
					{
						for(i = 0; i < minIdx; i++)
						{
							newList.push_back((*theList)[i]);
						}
						newList.push_back((*theList)[maxIdx]);
						for(i = minIdx+1; i < maxIdx; i++)
						{
							newList.push_back((*theList)[i]);
						}
						newList.push_back((*theList)[minIdx]);
						for(i = maxIdx+1; i < (jvxInt16)theList->size(); i++)
						{
							newList.push_back((*theList)[i]);
						}

						switch(tp)
						{
						case JVX_SEQUENCER_QUEUE_TYPE_RUN:
							_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfRunningSteps = newList;
							break;
						case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
							_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfLeaveSteps = newList;
							break;
						default:
							assert(0);
						}
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _remove_step_sequence(jvxSize idxsequence, jvxSequencerQueueType tp, jvxSize idx)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneSequenceStep>* theList = NULL;
		std::vector<oneSequenceStep> newList;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			if(idxsequence < _common_set_sequencer.theSequences.all_sequences.size())
			{
				switch(tp)
				{
				case JVX_SEQUENCER_QUEUE_TYPE_RUN:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfRunningSteps;
					break;
				case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
					theList = &_common_set_sequencer.theSequences.all_sequences[idxsequence].lstOfLeaveSteps;
					break;
				default:
					assert(0);
				}
				std::vector<oneSequenceStep>::iterator elm = findSequenceStepIdInList(*theList, idx);
				if(elm != theList->end())
				{
					theList->erase(elm);
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _reset_sequence(jvxSize in_handleId_sequence)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			std::vector<oneSequence>::iterator elm = _common_set_sequencer.theSequences.all_sequences.begin();
			elm = findSequenceIdInList(_common_set_sequencer.theSequences.all_sequences, in_handleId_sequence);
			if(elm != _common_set_sequencer.theSequences.all_sequences.end())
			{
				elm->lstOfRunningSteps.clear();
				elm->lstOfLeaveSteps.clear();
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _mark_sequence_process(jvxSize idxsequence, jvxBool runit)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneSequenceStep>* theList = NULL;
		std::vector<oneSequenceStep> newList;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			if(idxsequence < _common_set_sequencer.theSequences.all_sequences.size())
			{
				_common_set_sequencer.theSequences.all_sequences[idxsequence].markedForProcess = runit;
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Run a process which is composed of a list of sequences
	virtual jvxErrorType _run_process(jvxInt64 period_ms, IjvxSequencer_report* report, jvxHandle* privateData, jvxUInt64 eventMask = JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_PROCESS, bool externalTrigger = false)
	{
		jvxErrorType res = JVX_NO_ERROR;
		jvxBool foundStartPoint = false;

		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_ACTIVE)
		{
			foundStartPoint = findNextValidStep(0);
			if(!foundStartPoint)
			{
				res = JVX_ERROR_NOT_READY;
			}
			else
			{
				_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_STARTUP;
				_common_set_sequencer.theState = JVX_STATE_PROCESSING;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);

		if(res == JVX_NO_ERROR)
		{
			_common_set_sequencer.inOperation.eventMask = eventMask;
			_common_set_sequencer.inOperation.report = report;
			/*
			_common_set_sequencer.inOperation.idxSequence = 0;
			_common_set_sequencer.inOperation.idxStep = 0;*/

			_common_set_sequencer.inOperation.theSeqState =  JVX_SEQUENCER_STATUS_STARTUP; // has been written before..
			_common_set_sequencer.inOperation.externalTrigger = externalTrigger;
			_common_set_sequencer.inOperation.privateData = privateData;;

			if(_common_set_sequencer.inOperation.externalTrigger)
			{
				_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_IN_OPERATION;
				if(_common_set_sequencer.inOperation.report)
				{
					_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_PROCESS_STARTUP_COMPLETE, "Startup of sequencer thread successfully completed", -1, -1, JVX_SEQUENCER_QUEUE_TYPE_NONE, JVX_SEQUENCER_TYPE_COMMAND_NONE);
				}
			}
			else
			{
				res = jvx_thread_initialize(&_common_set_sequencer.thread_handle, &_common_set_sequencer.cb_thread_handler, privateData, true);
				if(res == JVX_NO_ERROR)
				{
					res = jvx_thread_start(_common_set_sequencer.thread_handle, (int)period_ms);
				}

				if(res != JVX_NO_ERROR)
				{
					jvx_thread_stop(_common_set_sequencer.thread_handle);
					jvx_thread_terminate(_common_set_sequencer.thread_handle);
					JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
					_common_set_sequencer.theState = JVX_STATE_ACTIVE;
					_common_set_sequencer.inOperation.idxSequence = -1;
					_common_set_sequencer.inOperation.idxStep = -1;
					JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
				}
			}
		}

		return(res);
	};


	virtual jvxErrorType _trigger_step_process_extern(jvxInt64 timestamp_us, jvxSequencerElementType* sequencer_element_type_on_leave)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);

		if(sequencer_element_type_on_leave)
		{
			*sequencer_element_type_on_leave = JVX_SEQUENCER_TYPE_COMMAND_NONE;
		}
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			if(_common_set_sequencer.inOperation.externalTrigger)
			{
				if(
					(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_IN_OPERATION) ||
					(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_WANTS_TO_SHUTDOWN)||
					(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS))
				{
					res = _common_set_sequencer.cb_thread_handler.callback_thread_timer_expired(_common_set_sequencer.inOperation.privateData, timestamp_us);
				}
				else if(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE)
				{
					res = JVX_ERROR_PROCESS_COMPLETE;
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
				if(res == JVX_NO_ERROR)
				{
					if(sequencer_element_type_on_leave)
					{
						*sequencer_element_type_on_leave = _common_set_sequencer.inOperation.thisStep.step_tp;
					}
				}
			}
			else
			{
				res = JVX_ERROR_NOT_READY;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _trigger_complete_process_extern(jvxInt64 timestamp_us)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			if(_common_set_sequencer.inOperation.externalTrigger)
			{
				if(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE)
				{
					res = _common_set_sequencer.cb_thread_handler.callback_thread_stopped(_common_set_sequencer.inOperation.privateData, timestamp_us);
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}
			else
			{
				res = JVX_ERROR_NOT_READY;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _read_step_type_process_extern( jvxSequencerElementType* theElementType)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			if(_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_IN_OPERATION)
			{
				if(_common_set_sequencer.inOperation.externalTrigger)
				{
					if(theElementType)
					{
						*theElementType = _common_set_sequencer.inOperation.thisStep.step_tp;
					}
				}
				else
				{
					res = JVX_ERROR_NOT_READY;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _trigger_abort_process()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			if(
				(_common_set_sequencer.inOperation.theSeqState > JVX_SEQUENCER_STATUS_NONE) &&
				(_common_set_sequencer.inOperation.theSeqState < JVX_SEQUENCER_STATUS_WANTS_TO_SHUTDOWN))
			{
				_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_WANTS_TO_SHUTDOWN;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _trigger_force_abort_process()
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			//while(_common_set_sequencer.inOperation.theSeqState ==
			jvx_thread_trigger_stop(_common_set_sequencer.thread_handle);
			_common_set_sequencer.theState = JVX_STATE_PREPARED;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Abort a certain process
	virtual jvxErrorType _wait_completion_process(jvxInt64 timeout_ms)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState == JVX_STATE_PROCESSING)
		{
			if(_common_set_sequencer.inOperation.externalTrigger)
			{
				if(_common_set_sequencer.inOperation.theSeqState != JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE)
				{
					res = JVX_ERROR_NOT_READY;
				}
				else
				{
					_common_set_sequencer.theState = JVX_STATE_ACTIVE;
					_common_set_sequencer.inOperation.idxSequence = -1;
					_common_set_sequencer.inOperation.idxStep = -1;
					_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_NONE;
					_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_NONE;
				}
			}
			else
			{
				// Wait for state machine to safely shutdown
				while(_common_set_sequencer.inOperation.theSeqState != JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE)
				{
					JVX_SLEEP_S(1);
				}
				jvx_thread_trigger_stop(_common_set_sequencer.thread_handle);
				jvx_thread_wait_stop(_common_set_sequencer.thread_handle, timeout_ms);
				_common_set_sequencer.theState = JVX_STATE_ACTIVE;
				_common_set_sequencer.inOperation.idxSequence = -1;
				_common_set_sequencer.inOperation.idxStep = -1;
				_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_NONE;
				_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_NONE;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Return the current status of the sequencer
	virtual jvxErrorType _status_process(jvxSequencerStatus* status,jvxInt32 * seqId, jvxSequencerQueueType* qTp,  jvxInt32 * stepId,jvxBool* loop_enabled)
	{
		if(status)
		{
			*status = _common_set_sequencer.inOperation.theSeqState;
		}
		if(seqId)
		{
			*seqId = _common_set_sequencer.inOperation.idxSequence;
		}
		if(seqId)
		{
			*stepId = _common_set_sequencer.inOperation.idxStep;
		}
		if(loop_enabled)
		{
			*loop_enabled = _common_set_sequencer.inOperation.loop_enabled;
		}
		if(qTp)
		{
			*qTp = _common_set_sequencer.inOperation.currentQueueTp;
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType _enable_loop_process(jvxBool enable)
	{
		_common_set_sequencer.inOperation.loop_enabled = enable;
		return(JVX_NO_ERROR);
	};

    // Browse registered sequences
	virtual jvxErrorType _number_sequences(jvxSize* num)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState >= JVX_STATE_ACTIVE)
		{
			if(num)
			{
				*num = _common_set_sequencer.theSequences.all_sequences.size();
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};


	// Browse registered sequences
	virtual jvxErrorType _description_sequence(jvxSize idx, jvxString** description, jvxSize* numStepsRun, jvxSize* numStepsLeave, jvxBool* markedForProcess)
    {
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState >= JVX_STATE_ACTIVE)
		{
			if(idx < _common_set_sequencer.theSequences.all_sequences.size())
			{
				HjvxObject_produceString(description, _common_set_sequencer.theSequences.all_sequences[idx].description);
				if(numStepsRun)
				{
					*numStepsRun =  _common_set_sequencer.theSequences.all_sequences[idx].lstOfRunningSteps.size();
				}
				if(numStepsLeave)
				{
					*numStepsLeave =  _common_set_sequencer.theSequences.all_sequences[idx].lstOfLeaveSteps.size();
				}
				if(markedForProcess)
				{
					*markedForProcess = _common_set_sequencer.theSequences.all_sequences[idx].markedForProcess;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// Browse registered sequences
	virtual jvxErrorType _description_step_sequence(jvxSize idx_seq, jvxSize idx_steps, jvxSequencerQueueType tp, jvxString** description,
		 jvxSequencerElementType* out_elementType, jvxComponentType* out_targetComponentType,
		jvxInt32* out_functionId, jvxInt64* out_timeout_ms, jvxString** label_name, jvxString** cond_label_true, jvxString** cond_label_false)
	{
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState >= JVX_STATE_ACTIVE)
		{
			if(idx_seq < _common_set_sequencer.theSequences.all_sequences.size())
			{
				std::vector<oneSequenceStep>* theList = NULL;
				switch(tp)
				{
				case JVX_SEQUENCER_QUEUE_TYPE_RUN:
					theList = &_common_set_sequencer.theSequences.all_sequences[idx_seq].lstOfRunningSteps;
					break;
				case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
					theList = &_common_set_sequencer.theSequences.all_sequences[idx_seq].lstOfLeaveSteps;
					break;
				default: assert(0);
				}
				if(idx_steps < (*theList).size())
				{
					HjvxObject_produceString(description, (*theList)[idx_steps].description);
					if(out_elementType)
					{
						*out_elementType = (*theList)[idx_steps].step_tp;
					}
					if(out_targetComponentType)
					{
						*out_targetComponentType = (*theList)[idx_steps].step_target;
					}
					if(out_functionId)
					{
						*out_functionId = (jvxInt32) ((*theList)[idx_steps].step_function_id);
					}
					if(out_timeout_ms)
					{
						*out_timeout_ms = (*theList)[idx_steps].step_timeout_ms;
					}
					HjvxObject_produceString(label_name, (*theList)[idx_steps].step_label);
					HjvxObject_produceString(cond_label_true,  (*theList)[idx_steps].step_cond_label_true);
					HjvxObject_produceString(cond_label_false,  (*theList)[idx_steps].step_cond_label_false);
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	virtual jvxErrorType _switch_sequences(jvxSize idx1, jvxSize idx2)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;
		JVX_LOCK_MUTEX(_common_set_sequencer.theAccess);
		if(_common_set_sequencer.theState >= JVX_STATE_ACTIVE)
		{
			if(
				(idx1 < _common_set_sequencer.theSequences.all_sequences.size()) &&
				(idx2 < _common_set_sequencer.theSequences.all_sequences.size())
				)
			{
				std::vector<oneSequence> newList;
				jvxSize minId = JVX_MIN(idx1, idx2);
				jvxSize maxId = JVX_MAX(idx1, idx2);

				for(i = 0; i < idx1; i++)
				{
					newList.push_back(_common_set_sequencer.theSequences.all_sequences[i]);
				}
				newList.push_back(_common_set_sequencer.theSequences.all_sequences[maxId]);
				for(i = idx1+1; i < idx2; i++)
				{
					newList.push_back(_common_set_sequencer.theSequences.all_sequences[i]);
				}
				newList.push_back(_common_set_sequencer.theSequences.all_sequences[minId]);
				for(i = idx2+1; i < _common_set_sequencer.theSequences.all_sequences.size(); i++)
				{
					newList.push_back(_common_set_sequencer.theSequences.all_sequences[i]);
				}
				_common_set_sequencer.theSequences.all_sequences = newList;
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		JVX_UNLOCK_MUTEX(_common_set_sequencer.theAccess);
		return(res);
	};

	// ==========================================================================================
	// Put/get configuration
	// ==========================================================================================
	virtual jvxErrorType JVX_CALLINGCONVENTION _put_configuration_seq(IjvxConfigProcessor* processor, jvxHandle* sectionToContainAllEntries,
		const char* filename = "", jvxInt32 lineno = -1 )
	{
		jvxErrorType res = JVX_NO_ERROR;
		jvxSize i,j;
		jvxSize num = 0;
		jvxSize numS = 0;
		jvxConfigData* sectionAll = NULL;
		jvxConfigData* datTmp = NULL;
		jvxConfigData* datTmp1 = NULL;
		jvxConfigData* datTmp_steps = NULL;
		jvxDataList* lstD1 = NULL;
		jvxString* fldStr = NULL;
		std::string entry;
		jvxInt16 entryInt16;

		std::vector<std::string> entriesStr, entriesStrLab, entriesStrLabTrue, entriesStrLabFalse;
		std::vector<jvxInt16> entriesInt16, entriesInt16Comp, entriesInt16Comm;
		std::vector<jvxInt64> entriesInt64Timeouts;

		this->_reset_sequences();

		res = processor->getReferenceSubsectionCurrentSection_name(sectionToContainAllEntries, &sectionAll, JVX_CONFIG_SEQUENCER_SEQ_SECTION);
		if((res == JVX_NO_ERROR) && sectionAll)
		{
			processor->getNumberSubsectionsCurrentSection(sectionAll, &num);
			for(i = 0; i < num; i++)
			{
				datTmp = NULL;
				res = processor->getReferenceSubsectionCurrentSection_id(sectionAll, &datTmp, i);
				if((res == JVX_NO_ERROR) && datTmp)
				{
					// Read section-names
					entriesStr.clear();
					if(HjvxConfigProcessor_readEntry_assignmentString(processor, datTmp, JVX_CONFIG_SEQUENCER_SEQ_NAMES, &entry) == JVX_NO_ERROR)
					{
						HjvxConfigProcessor_readEntry_assignmentInt16(processor, datTmp, JVX_CONFIG_SEQUENCER_SEQ_MARKED_FLAG, &entryInt16);

						this->_add_sequence(entry.c_str());
						if(entryInt16 != 0)
						{
							this->_mark_sequence_process(i, true);
						}
						else
						{
							this->_mark_sequence_process(i, false);
						}

						entriesStr.clear();
						entriesStrLab.clear();
						entriesStrLabTrue.clear();
						entriesStrLabFalse.clear();
						entriesInt16.clear();
						entriesInt16Comp.clear();
						entriesInt16Comm.clear();
						entriesInt64Timeouts.clear();

						// Next, add all steps for run sequence
						std::string searchMe = JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"run_" + jvx_int2String((int)i);

						datTmp_steps = NULL;
						res = processor->getReferenceEntryCurrentSection_name(datTmp, &datTmp_steps, searchMe.c_str());
						if((res == JVX_NO_ERROR) && datTmp_steps)
						{
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_DECSCS, &entriesStr);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL, &entriesStrLab);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_TRUE, &entriesStrLabTrue);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_FALSE, &entriesStrLabFalse);

							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TYPES, entriesInt16);
							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMPONENTS, entriesInt16Comp);
							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMMAND_IDS, entriesInt16Comm);
							HjvxConfigProcessor_readEntry_1dInt64List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TIMEOUTS, entriesInt64Timeouts);

							jvxSize numSteps = 0;
							numSteps = entriesStr.size();
							numSteps = JVX_MIN(numSteps, entriesStrLab.size());
							numSteps = JVX_MIN(numSteps, entriesStrLabTrue.size());
							numSteps = JVX_MIN(numSteps, entriesStrLabFalse.size());
							numSteps = JVX_MIN(numSteps, entriesInt16.size());
							numSteps = JVX_MIN(numSteps, entriesInt16Comp.size());
							numSteps = JVX_MIN(numSteps, entriesInt16Comm.size());
							numSteps = JVX_MIN(numSteps, entriesInt64Timeouts.size());

							for(j = 0; j < numSteps; j++)
							{
								this->_insert_step_sequence_at(i, JVX_SEQUENCER_QUEUE_TYPE_RUN, ( jvxSequencerElementType)entriesInt16[j], (jvxComponentType)entriesInt16Comp[j],
									entriesStr[j].c_str(), entriesInt16Comm[j],
									entriesInt64Timeouts[j], entriesStrLab[j].c_str() ,
									entriesStrLabTrue[j].c_str(), entriesStrLabFalse[j].c_str(),
									-1);
							}
						}

						// Next, add all steps for run sequence
						searchMe = JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"leave_" + jvx_int2String((int)i);

						entriesStr.clear();
						entriesStrLab.clear();
						entriesStrLabTrue.clear();
						entriesStrLabFalse.clear();
						entriesInt16.clear();
						entriesInt16Comp.clear();
						entriesInt16Comm.clear();
						entriesInt64Timeouts.clear();

						datTmp_steps = NULL;
						res = processor->getReferenceEntryCurrentSection_name(datTmp, &datTmp_steps, searchMe.c_str());
						if((res == JVX_NO_ERROR) && datTmp_steps)
						{
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_DECSCS, &entriesStr);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL, &entriesStrLab);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_TRUE, &entriesStrLabTrue);
							HjvxConfigProcessor_readEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_FALSE, &entriesStrLabFalse);

							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TYPES, entriesInt16);
							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMPONENTS, entriesInt16Comp);
							HjvxConfigProcessor_readEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMMAND_IDS, entriesInt16Comm);
							HjvxConfigProcessor_readEntry_1dInt64List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TIMEOUTS, entriesInt64Timeouts);

							jvxSize numSteps = 0;
							numSteps = entriesStr.size();
							numSteps = JVX_MIN(numSteps, entriesStrLab.size());
							numSteps = JVX_MIN(numSteps, entriesStrLabTrue.size());
							numSteps = JVX_MIN(numSteps, entriesStrLabFalse.size());
							numSteps = JVX_MIN(numSteps, entriesInt16.size());
							numSteps = JVX_MIN(numSteps, entriesInt16Comp.size());
							numSteps = JVX_MIN(numSteps, entriesInt16Comm.size());
							numSteps = JVX_MIN(numSteps, entriesInt64Timeouts.size());

							for(j = 0; j < numSteps; j++)
							{
								this->_insert_step_sequence_at(i, JVX_SEQUENCER_QUEUE_TYPE_LEAVE, ( jvxSequencerElementType)entriesInt16[j], (jvxComponentType)entriesInt16Comp[j],
									entriesStr[j].c_str(), entriesInt16Comm[j],
									entriesInt64Timeouts[j], entriesStrLab[j].c_str() ,
									entriesStrLabTrue[j].c_str(), entriesStrLabFalse[j].c_str(),
									-1);
							}
						} 	//res = processor->getReferenceEntryCurrentSection_name(datTmp, &datTmp_steps, searchMe.c_str());
					}// if(HjvxConfigProcessor_readEntry_assignmentString(processor, datTmp, JVX_CONFIG_SEQUENCER_SEQ_NAMES, &entry) == JVX_NO_ERROR)
				} //res = processor->getReferenceSubsectionCurrentSection_id(sectionToContainAllEntries, &datTmp, i);
			}//for(i = 0; i < num; i++)
		}
		return(JVX_NO_ERROR);
	};


	virtual jvxErrorType JVX_CALLINGCONVENTION _get_configuration_seq(IjvxConfigProcessor* processor, jvxHandle* sectionWhereToAddAllSubsections, IjvxObject* me_object)
	{
		jvxErrorType res = JVX_NO_ERROR;
		jvxSize i,j;
		jvxSize num = 0;
		jvxSize numStepsRun = 0;
		jvxSize numStepsLeave = 0;
		jvxConfigData* datTmp = NULL;
		jvxConfigData* datTmp1 = NULL;
		jvxConfigData* datTmp_steps = NULL;
		jvxConfigData* sectionAll = NULL;
		jvxBool markedForProcessing = false;

		jvxString* fldStr = NULL;
		std::string entry;
		jvxInt16 entryInt16;

		 jvxSequencerElementType elmType = JVX_SEQUENCER_TYPE_COMMAND_NONE;
		jvxComponentType comType = JVX_COMPONENT_UNKNOWN;
		jvxInt32 idxCommandId = 0;
		jvxInt64 timeoutms = -1;
		jvxString* fldStr_lab = NULL;
		jvxString* fldStr_lab_true = NULL;
		jvxString* fldStr_lab_false = NULL;


		std::vector<std::string> entriesStr, entriesStrLab, entriesStrLabTrue, entriesStrLabFalse;
		std::vector<jvxInt16> entriesInt16, entriesInt16Comp, entriesInt16Comm;
		std::vector<jvxInt64> entriesInt64Timeouts;

		res = processor->createEmptySection(&sectionAll, JVX_CONFIG_SEQUENCER_SEQ_SECTION);
		if(res == JVX_NO_ERROR)
		{
			this->_number_sequences(&num);
			for(i = 0; i < num; i++)
			{
				res = processor->createEmptySection(&datTmp, (JVX_CONFIG_SEQUENCER_SEQ_SUBSECTION_PREFIX + jvx_int2String((int)i)).c_str());
				if(res == JVX_NO_ERROR)
				{
					fldStr = NULL;
					numStepsRun = 0;
					numStepsLeave = 0;
					markedForProcessing = false;

					res = this->_description_sequence(i, &fldStr, &numStepsRun, &numStepsLeave, &markedForProcessing);
					if((res == JVX_NO_ERROR) &&fldStr)
					{
						entry = fldStr->bString;
						me_object->deallocate(fldStr);

						HjvxConfigProcessor_writeEntry_assignmentString(processor, datTmp, JVX_CONFIG_SEQUENCER_SEQ_NAMES, &entry);

						entryInt16 = (jvxInt16)markedForProcessing;
						HjvxConfigProcessor_writeEntry_assignmentInt16(processor, datTmp, JVX_CONFIG_SEQUENCER_SEQ_MARKED_FLAG, &entryInt16);

						processor->createEmptySection(&datTmp_steps, ( JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"run_" + jvx_int2String((int)i)).c_str());
						if(datTmp_steps)
						{
							entriesStr.clear();
							entriesStrLab.clear();
							entriesStrLabTrue.clear();
							entriesStrLabFalse.clear();

							entriesInt16.clear();
							entriesInt16Comp.clear();
							entriesInt16Comm.clear();
							entriesInt64Timeouts.clear();

							for(j = 0; j < numStepsRun; j++)
							{
								res = this->_description_step_sequence(i, j, JVX_SEQUENCER_QUEUE_TYPE_RUN, &fldStr, &elmType, &comType,
									&idxCommandId, &timeoutms, &fldStr_lab, &fldStr_lab_true, &fldStr_lab_false);
								if((res == JVX_NO_ERROR) && fldStr && fldStr_lab && fldStr_lab_true && fldStr_lab_false)
								{
									entriesStr.push_back(fldStr->bString);
									entriesStrLab.push_back(fldStr_lab->bString);
									entriesStrLabTrue.push_back(fldStr_lab_true->bString);
									entriesStrLabFalse.push_back(fldStr_lab_false->bString);

									entriesInt16.push_back(elmType);
									entriesInt16Comp.push_back(comType);
									entriesInt16Comm.push_back((jvxInt16)idxCommandId);
									entriesInt64Timeouts.push_back(timeoutms);

									me_object->deallocate(fldStr);
									me_object->deallocate(fldStr_lab);
									me_object->deallocate(fldStr_lab_true);
									me_object->deallocate(fldStr_lab_false);
									fldStr = NULL;
									fldStr_lab = NULL;
									fldStr_lab_true = NULL;
									fldStr_lab_false = NULL;
								}
								else
								{
									assert(0);
								}
							}
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_DECSCS, &entriesStr);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL, &entriesStrLab);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_TRUE, &entriesStrLabTrue);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_FALSE, &entriesStrLabFalse);

							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TYPES,entriesInt16);
							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMPONENTS, entriesInt16Comp);
							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMMAND_IDS, entriesInt16Comm);
							HjvxConfigProcessor_writeEntry_1dInt64List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TIMEOUTS, entriesInt64Timeouts);

							// Add subsection
							processor->addSubsectionToSection(datTmp, datTmp_steps);
						}//processor->createEmptySection(&datTmp_steps, ( JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"run_" + jvx_int2String((int)i)).c_str());
						datTmp_steps = NULL;

						processor->createEmptySection(&datTmp_steps, ( JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"leave_" + jvx_int2String((int)i)).c_str());
						if(datTmp_steps)
						{
							entriesStr.clear();
							entriesStrLab.clear();
							entriesStrLabTrue.clear();
							entriesStrLabFalse.clear();

							entriesInt16.clear();
							entriesInt16Comp.clear();
							entriesInt16Comm.clear();
							entriesInt64Timeouts.clear();

							for(j = 0; j < numStepsLeave; j++)
							{

								res = this->_description_step_sequence(i, j, JVX_SEQUENCER_QUEUE_TYPE_LEAVE, &fldStr, &elmType, &comType,
									&idxCommandId, &timeoutms, &fldStr_lab, &fldStr_lab_true, &fldStr_lab_false);
								if((res == JVX_NO_ERROR) && fldStr && fldStr_lab && fldStr_lab_true && fldStr_lab_false)
								{
									entriesStr.push_back(fldStr->bString);
									entriesStrLab.push_back(fldStr_lab->bString);
									entriesStrLabTrue.push_back(fldStr_lab_true->bString);
									entriesStrLabFalse.push_back(fldStr_lab_false->bString);

									entriesInt16.push_back(elmType);
									entriesInt16Comp.push_back(comType);
									entriesInt16Comm.push_back((jvxInt16)idxCommandId);
									entriesInt64Timeouts.push_back(timeoutms);

									me_object->deallocate(fldStr);
									me_object->deallocate(fldStr_lab);
									me_object->deallocate(fldStr_lab_true);
									me_object->deallocate(fldStr_lab_false);
									fldStr = NULL;
									fldStr_lab = NULL;
									fldStr_lab_true = NULL;
									fldStr_lab_false = NULL;
								}
								else
								{
									assert(0);
								}
							}
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_DECSCS, &entriesStr);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL, &entriesStrLab);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_TRUE, &entriesStrLabTrue);
							HjvxConfigProcessor_writeEntry_assignmentStringList(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_LABEL_FALSE, &entriesStrLabFalse);

							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TYPES,entriesInt16);
							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMPONENTS, entriesInt16Comp);
							HjvxConfigProcessor_writeEntry_1dInt16List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_COMMAND_IDS, entriesInt16Comm);
							HjvxConfigProcessor_writeEntry_1dInt64List(processor, datTmp_steps, JVX_CONFIG_ONE_SEQUENCE_STEP_TIMEOUTS, entriesInt64Timeouts);

							// Add subsection
							processor->addSubsectionToSection(datTmp, datTmp_steps);
						}//processor->createEmptySection(&datTmp_steps, ( JVX_CONFIG_ONE_SEQUENCE_STEPS_PREFIX + (std::string)"run_" + jvx_int2String((int)i)).c_str());
						datTmp_steps = NULL;
					} // res = this->_description_sequence(i, &fldStr, &numStepsRun, &numStepsLeave, &markedForProcessing);
					processor->addSubsectionToSection(sectionAll, datTmp);
				}//res = processor->createEmptySection(&datTmp, (JVX_CONFIG_SEQUENCER_SEQ_SUBSECTION_PREFIX + jvx_int2String((int)i)).c_str());
				datTmp = NULL;

			} // for(i = 0; i < num; i++)
			processor->addSubsectionToSection(sectionWhereToAddAllSubsections, sectionAll);
		} // res = processor->createEmptySection(&sectionAll, JVX_CONFIG_SEQUENCER_SEQ_SECTION);
		sectionAll = NULL;
		return(JVX_NO_ERROR);
	};


	// ==========================================================================================
	// Helpers
	// ==========================================================================================

	std::vector<oneSequence>::iterator findSequenceNameInList(std::vector<oneSequence>& all_sequences, std::string description)
	{
		std::vector<oneSequence>::iterator elm = all_sequences.begin();
		for(; elm != all_sequences.end(); elm++)
		{
			if(elm->description == description)
			{
				break;
			}
		}
		return(elm);
	};

	std::vector<oneSequence>::iterator findSequenceIdInList(std::vector<oneSequence>& all_sequences, jvxSize idx)
	{
		std::vector<oneSequence>::iterator elm = all_sequences.begin();
		jvxSize cnt = 0;
		for(; elm != all_sequences.end(); elm++, cnt++)
		{
			if(cnt == idx)
			{
				break;
			}
		}
		return(elm);
	};

	std::vector<oneSequenceStep>::iterator findSequenceStepIdInList(std::vector<oneSequenceStep>& all_steps, jvxSize idx)
	{
		std::vector<oneSequenceStep>::iterator elm = all_steps.begin();
		jvxSize cnt = 0;
		for(; elm != all_steps.end(); elm++, cnt++)
		{
			if(cnt == idx)
			{
				break;
			}
		}
		return(elm);
	};

	bool findNextValidStep(int offset)
	{
		jvxSize i;
		bool foundIt = false;
		for(i = offset; i < _common_set_sequencer.theSequences.all_sequences.size(); i++)
		{
			if(_common_set_sequencer.theSequences.all_sequences[i].markedForProcess)
			{
				if(
					(_common_set_sequencer.theSequences.all_sequences[i].lstOfRunningSteps.size() > 0) ||
					(_common_set_sequencer.theSequences.all_sequences[i].lstOfLeaveSteps.size() > 0))
				{
					_common_set_sequencer.inOperation.idxSequence = (jvxInt16)i;
					if(_common_set_sequencer.theSequences.all_sequences[i].lstOfRunningSteps.size() > 0)
					{
						_common_set_sequencer.inOperation.idxStep = 0;
						_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_RUN;
					}
					else
					{
						_common_set_sequencer.inOperation.idxStep = 0;
						_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_LEAVE;
					}
					foundIt = true;
					break;
				}
			}
		}
		return(foundIt);
	};
};

#endif
