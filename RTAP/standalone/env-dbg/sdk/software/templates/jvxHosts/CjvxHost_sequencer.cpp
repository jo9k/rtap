#include "CjvxHost.h"

typedef enum
{
	JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP,
	JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_INCREMENT,
	JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_LEAVE_LOOP,
	JVX_SEQUENCER_PROCEDE_JUMP_TO_LABEL,
	JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT,
	JVX_SEQUENCER_PROCEDE_BREAK,
	JVX_SEQUENCER_PROCEDE_EXIT_ERROR,
	JVX_SEQUENCER_PROCEDE_WANTS_TO_SHUTDOWN
} whatToDoNextType;

/*
jvxErrorType
CjvxHostBdx1::prepareOneDevice(jvxComponentType tp, IjvxObject* theObj)
{
	jvxErrorType res = JVX_NO_ERROR;
	IjvxDataProcessorSelector* theSelector = NULL;

	theObj->
	switch(tp)
	{
	case JVX_COMPONENT_SCANNER_TECHNOLOGY:
		res = JVX_ERROR_UNSUPPORTED;
		break;
	case JVX_COMPONENT_SCANNER_DEVICE:
		res = theScanner
		break;
	case JVX_COMPONENT_SPECTRUM_PROCESSOR_TECHNOLOGY:
		res = JVX_ERROR_UNSUPPORTED;
		break;
	case JVX_COMPONENT_SPECTRUM_PROCESSOR_DEVICE:
		res = theSpecProcDev->prepare();
		break;
	case JVX_COMPONENT_LAMP_TECHNOLOGY:
		res = JVX_ERROR_UNSUPPORTED;
		break;
	case JVX_COMPONENT_LAMP_DEVICE:
		res = theLampDev->prepare();
		break;
	case JVX_COMPONENT_CAMERA_TECHNOLOGY:
		res = JVX_ERROR_UNSUPPORTED;
		break;
	case JVX_COMPONENT_CAMERA_DEVICE:
		res = theCameraDev->prepare();
		break;
	case JVX_COMPONENT_ASCAN_ENDPOINT:
		res = theAscanEndpt->prepare();
		break;
	}
	return res;
}
*/
/*
jvxErrorType
CjvxHostBdx1::prepareCompone(IjvxScannerDevice* theDevice)
{
	jvxErrorType res = JVX_NO_ERROR;
	IjvxObject* theObj_sp = NULL;
	IjvxSpectrumProcessorDevice* theDev_sp = NULL;

	IjvxObject* theObj_as = NULL;
	IjvxAscanEndpoint* theDev_as = NULL;

	theDe
	this->request_object_component(JVX_COMPONENT_SPECTRUM_PROCESSOR_DEVICE, &theObj_sp);
	if(theObj_sp)
	{
		theObj_sp->request_specialization(reinterpret_cast<jvxHandle**>(&theDev_sp), NULL, NULL);
	}

	this->request_object_component(JVX_COMPONENT_ASCAN_ENDPOINT, &theObj_sp);
	if(theObj_sp)
	{
		theObj_sp->request_specialization(reinterpret_cast<jvxHandle**>(&theDev_as), NULL, NULL);
	}

	res = theDevice->prepare(theDev_sp, theDev_as);
	return res;
}
*/

jvxErrorType
CjvxHost::cb_startup(jvxHandle* privateData_queue, jvxInt64 timestamp_us)
{
	CjvxHost* this_pointer = (CjvxHost*)privateData_queue;
	if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_PROCESS_STARTUP_COMPLETE)
	{
		this_pointer->_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_IN_OPERATION;
		if(this_pointer->_common_set_sequencer.inOperation.report)
		{
			this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_PROCESS_STARTUP_COMPLETE, "Startup of sequencer thread successfully completed", -1, -1, JVX_SEQUENCER_QUEUE_TYPE_NONE);
		}
		this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;

	}
	return JVX_NO_ERROR;
}

jvxErrorType
CjvxHost::cb_period_expired(jvxHandle* privateData_queue, jvxInt64 timestamp_us)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR, resL = JVX_NO_ERROR;
	CjvxHost* this_pointer = (CjvxHost*)privateData_queue;
	oneSequenceStep* theStep = NULL;
	jvxUInt64 reportFlag = 0;
	jvxBool regularNextStep = true;
	jvxBool run = true;
	jvxBool searchNext = false;
	IjvxObject* theObject = NULL;
	jvxHandle* hdl = NULL;
	jvxComponentType theType = JVX_COMPONENT_UNKNOWN;
	std::string errorDescription;
	jvxSequencerEvent howtosaygoodbye = JVX_SEQUENCER_EVENT_NONE;
	jvxBool reportEndOfProcess = false;

		// ====================================================================
	// Loop for as many steps as possible
	// ====================================================================
	while(run)
	{
		IjvxTechnology* theTech = NULL;
		IjvxDevice* theDev = NULL;
		IjvxNode* theNode = NULL;

		IjvxSequencerControl* theSeqContr = NULL;
		jvxBool success = false;
		std::string label_next_step ;
		whatToDoNextType whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;

		IjvxDataProcessorSelector* theSelector = NULL;
		IjvxDataProcessor* datProc = NULL;
		jvxSize numReferences = 0;
		IjvxDataProcessor** fldReferences = NULL;
		jvxComponentType tpReference;
		jvxInt32 purpose;

		jvxBool reportEndOfSequence = false;
		jvxSize thePcOffset = 0;

		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		// Find the pointer to current sequence step
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		// If we have completed the sequencer before, do nothing else
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		if(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE)
		{
			break;
		}

		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		// Check wether processing was aborted from outside
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		if(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_WANTS_TO_SHUTDOWN)
		{
			whatToDoNext = JVX_SEQUENCER_PROCEDE_WANTS_TO_SHUTDOWN;
			// State is switched to shutdown lateron
		}
		else
		{
			if(this_pointer->_common_set_sequencer.inOperation.idxSequence >= 0)
			{
				if(this_pointer->_common_set_sequencer.inOperation.idxSequence < (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences.size())
				{
					switch(this_pointer->_common_set_sequencer.inOperation.currentQueueTp)
					{
					case JVX_SEQUENCER_QUEUE_TYPE_RUN:
						if(this_pointer->_common_set_sequencer.inOperation.idxStep < (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps.size())
						{
							theStep = &this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps[this_pointer->_common_set_sequencer.inOperation.idxStep];
						}
						break;
					case JVX_SEQUENCER_QUEUE_TYPE_LEAVE:
						if(this_pointer->_common_set_sequencer.inOperation.idxStep < (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps.size())
						{
							theStep = &this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps[this_pointer->_common_set_sequencer.inOperation.idxStep];
						}
						break;
					}
				}
			}

			if(!theStep)
			{
				whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
				errorDescription = "Failed to determine next step";
				res = JVX_ERROR_UNEXPECTED;
			}

			// Store the current step type for external triggering
			this_pointer->_common_set_sequencer.inOperation.thisStep.step_tp = theStep->step_tp;

			// Set object reference to invalid..
			theObject = NULL;

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Get reference to target object
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			if(res == JVX_NO_ERROR)
			{
				res = this_pointer->request_object_component(theStep->step_target, &theObject);
				if(!((res == JVX_NO_ERROR) && theObject))
				{
					switch(theStep->step_tp)
					{
					case JVX_SEQUENCER_TYPE_JUMP:
					case JVX_SEQUENCER_TYPE_WAIT_FOREVER:
					case JVX_SEQUENCER_TYPE_REQUEST_START_IN_PROCESSING_LOOP:
					case JVX_SEQUENCER_TYPE_REQUEST_STOP_IN_PROCESSING_LOOP:
					case JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER:
					case JVX_SEQUENCER_TYPE_CALLBACK:
					case JVX_SEQUENCER_TYPE_BREAK:
						// This is ok, we do not need an object
						res = JVX_NO_ERROR;
						break;
					default:
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Failed to get reference to component " + jvxComponentType_txt(theStep->step_target);
						res = JVX_ERROR_ELEMENT_NOT_FOUND;
					}
				}
			}

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Get object specialization reference
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			if(res == JVX_NO_ERROR)
			{
				if(theObject) // Some valid steps require no object pointer...
				{
					hdl = NULL;
					res = theObject->request_specialization(&hdl, &theType, NULL);
					if(!((res == JVX_NO_ERROR) && (hdl)))
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Failed to get specialization of component " + jvxComponentType_txt(theStep->step_target);
						res = JVX_ERROR_INTERNAL;
						this_pointer->return_object_component(theStep->step_target, theObject);
					}
				}
			}

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Set pointer to correct object reference
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			if(res == JVX_NO_ERROR)
			{
				if(theObject) // Some valid steps require no object pointer...
				{
					bool foundType = false;
					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						theTech = reinterpret_cast<IjvxTechnology*>(hdl);
						foundType = true;
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						theDev = reinterpret_cast<IjvxDevice*>(hdl);
						foundType = true;
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						theNode = reinterpret_cast<IjvxNode*>(hdl);
						foundType = true;
						break;
					}

					if(!foundType)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Component type " + jvxComponentType_txt(theStep->step_tp) + " not supported.";
						res = JVX_ERROR_UNSUPPORTED;
						this_pointer->return_object_component(theStep->step_target, theObject);
					}
				}
			}

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Execute the actual command
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			if(res == JVX_NO_ERROR)
			{
				switch(theStep->step_tp)
				{
				case JVX_SEQUENCER_TYPE_COMMAND_STD_ACTIVATE:

					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->activate();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->activate();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->activate();
						break;
					}
					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <activate> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}

					break;

				case JVX_SEQUENCER_TYPE_COMMAND_STD_PREPARE:




					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR_SELECTOR, reinterpret_cast<jvxHandle**>(&theSelector));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR_SELECTOR, reinterpret_cast<jvxHandle**>(&theSelector));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR_SELECTOR, reinterpret_cast<jvxHandle**>(&theSelector));
						break;
					}

					if(theSelector)
					{
						theSelector->number_processors(&numReferences);
					}

					if(numReferences > 0)
					{
						JVX_SAFE_NEW_FLD(fldReferences, IjvxDataProcessor*, numReferences);
						for(i = 0; i < numReferences; i++)
						{
							tpReference = JVX_COMPONENT_UNKNOWN;

							theSelector->type_processors(i, &tpReference);
							datProc = NULL;

							// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
							// Get reference to target object
							// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
							if(res == JVX_NO_ERROR)
							{
								res = this_pointer->request_object_component(tpReference, &theObject);
								if(!((res == JVX_NO_ERROR) && theObject))
								{
									whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
									errorDescription = "Failed to get reference to component " + jvxComponentType_txt(tpReference);
									res = JVX_ERROR_ELEMENT_NOT_FOUND;
								}
							}

							// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
							// Get object specialization reference
							// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
							if(res == JVX_NO_ERROR)
							{
								hdl = NULL;
								res = theObject->request_specialization(&hdl, NULL, NULL);
								if(!((res == JVX_NO_ERROR) && (hdl)))
								{
									whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
									errorDescription = "Failed to get specialization of component " + jvxComponentType_txt(theStep->step_tp);
									res = JVX_ERROR_INTERNAL;
									this_pointer->return_object_component(tpReference, theObject);
								}
							}
							if(res == JVX_NO_ERROR)
							{
								switch(tpReference)
								{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
									reinterpret_cast<IjvxTechnology*>(hdl)->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR, reinterpret_cast<jvxHandle**>(&datProc));
									break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
									reinterpret_cast<IjvxDevice*>(hdl)->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR, reinterpret_cast<jvxHandle**>(&datProc));
									break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
									reinterpret_cast<IjvxNode*>(hdl)->request_hidden_interface(JVX_INTERFACE_DATAPROCESSOR, reinterpret_cast<jvxHandle**>(&datProc));
									break;
								}
							}
							fldReferences[i] = datProc;
						}
					}
					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->prepare(fldReferences, numReferences);
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->prepare(fldReferences, numReferences);
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->prepare(fldReferences, numReferences);
						break;
					}

					if(numReferences > 0)
					{
						JVX_SAFE_DELETE_FLD(fldReferences, IjvxDataProcessor*);
						fldReferences = NULL;
					}
					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <prepare> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}

					break;

				case JVX_SEQUENCER_TYPE_COMMAND_STD_START:

					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->start();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->start();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->start();
						break;
					}

					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <start> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

				case JVX_SEQUENCER_TYPE_COMMAND_STD_STOP:

					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->stop();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->stop();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->stop();
						break;
					}
					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <stop> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

				case JVX_SEQUENCER_TYPE_COMMAND_STD_POSTPROCESS:

					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->postprocess();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->postprocess();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->postprocess();
						break;
					}
					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <postprocess> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

				case JVX_SEQUENCER_TYPE_COMMAND_STD_DEACTIVATE:

					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->deactivate();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->deactivate();
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->deactivate();
						break;
					}
					if(res == JVX_NO_ERROR)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Operation <deactivate> failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

				case JVX_SEQUENCER_TYPE_CONDITION_WAIT:
				case JVX_SEQUENCER_TYPE_CONDITION_JUMP:
				case JVX_SEQUENCER_TYPE_COMMAND_SPECIFIC:


					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
					}

					if((res == JVX_NO_ERROR) && theSeqContr)
					{
						if(theStep->step_tp == JVX_SEQUENCER_TYPE_CONDITION_WAIT)
						{
							res = theSeqContr->status_condition(theStep->step_function_id, &success);
							if(res == JVX_NO_ERROR)
							{
								if(success == true)
								{
									whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
								}
								else
								{
									whatToDoNext = JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT;
									// Show that we need to continue waiting
								}
							}
							else
							{
							}
						}
						else if(theStep->step_tp == JVX_SEQUENCER_TYPE_CONDITION_JUMP)
						{
							res = theSeqContr->status_condition(theStep->step_function_id, &success);
							if(res == JVX_NO_ERROR)
							{
								if(success == true)
								{
									label_next_step = theStep->step_cond_label_true;
								}
								else
								{
									label_next_step = theStep->step_cond_label_false;
								}
								whatToDoNext = JVX_SEQUENCER_PROCEDE_JUMP_TO_LABEL;
							}
							else
							{
							}
						}
						else if(theStep->step_tp == JVX_SEQUENCER_TYPE_COMMAND_SPECIFIC)
						{
							res = theSeqContr->trigger_command(theStep->step_function_id);
							if(res == JVX_NO_ERROR)
							{
								whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
							}
							else
							{
								if(res == JVX_ERROR_POSTPONE)
								{
									whatToDoNext = JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT;
									res = JVX_NO_ERROR;
								}
							}
						}
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Request to receive sequencer control failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

				case JVX_SEQUENCER_TYPE_JUMP:
					label_next_step = theStep->step_cond_label_true;
					whatToDoNext = JVX_SEQUENCER_PROCEDE_JUMP_TO_LABEL;
					break;

				case JVX_SEQUENCER_TYPE_WAIT_FOREVER:
					// Just do nothing else than return
					whatToDoNext = JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT;
					howtosaygoodbye = JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ABORT;
					break;

				case JVX_SEQUENCER_TYPE_BREAK:

					// Just do nothing else than return
					whatToDoNext = JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT;
					howtosaygoodbye = JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_SEQUENCE;
					break;

				case JVX_SEQUENCER_TYPE_REQUEST_START_IN_PROCESSING_LOOP:
				case JVX_SEQUENCER_TYPE_REQUEST_STOP_IN_PROCESSING_LOOP:

					whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_LEAVE_LOOP;
					howtosaygoodbye = JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_SEQUENCE;
					break;

				case JVX_SEQUENCER_TYPE_WAIT_CONDITION_RELATIVE_JUMP:
					switch(theStep->step_target)
					{
#include "codeFragments/jvxHosts/Hjvx_caseStatement_technologies.h"
						res = theTech->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_devices.h"
						res = theDev->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
#include "codeFragments/jvxHosts/Hjvx_caseStatement_nodes.h"
						res = theNode->request_hidden_interface(JVX_INTERFACE_SEQUENCERCONTROL, reinterpret_cast<jvxHandle**>(&theSeqContr));
						break;
					}

					if((res == JVX_NO_ERROR) && theSeqContr)
					{
						res = theSeqContr->trigger_relative_jump(theStep->step_function_id, &thePcOffset);
						if(res == JVX_NO_ERROR)
						{
							whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_INCREMENT;
						}
						else
						{
							if((res == JVX_ERROR_POSTPONE) || (thePcOffset == 0))
							{
								whatToDoNext = JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT;
								res = JVX_NO_ERROR;
							}
						}
					}
					else
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Request to receive sequencer control failed for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
					break;

					case JVX_SEQUENCER_TYPE_REQUEST_UPDATE_VIEWER:

						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;
						break;
					case JVX_SEQUENCER_TYPE_CALLBACK:
						
						resL = JVX_NO_ERROR;
						if(this_pointer->_common_set_sequencer.inOperation.report)
						{
							resL = this_pointer->_common_set_sequencer.inOperation.report->sequencer_callback(this_pointer->_common_set_sequencer.inOperation.idxSequence, 
								this_pointer->_common_set_sequencer.inOperation.idxStep, this_pointer->_common_set_sequencer.inOperation.currentQueueTp,
								theStep->step_function_id);
						}
						whatToDoNext = JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP;

						if(res != JVX_NO_ERROR)
						{
							whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
							errorDescription = "Sequencer callback failed for function id <" + jvx_int2String(theStep->step_function_id) + "> due to following reason: " + jvxErrorType_descr(res);
						}

						break;

				default:

					whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
					errorDescription = "Unsupported sequencer operation " + jvxSequencerElementType_txt(theStep->step_tp) + " for component type " + jvxComponentType_txt(theStep->step_tp) + " due to following reason: " + jvxErrorType_descr(res);
					res = JVX_ERROR_UNSUPPORTED;
				}//switch(theStep->step_tp)

				// Return the object
				hdl = NULL;
				this_pointer->return_object_component(theStep->step_target, theObject);

			}//if(res == JVX_NO_ERROR)

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Check for timeout
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			if(whatToDoNext == JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT)
			{
				if(theStep->step_timeout_ms > 0)
				{
					jvxInt64 deltaT = (timestamp_us - this_pointer->_common_set_sequencer.inOperation.timestamp_store)/1000;
					if(deltaT > theStep->step_timeout_ms)
					{
						res = JVX_ERROR_TIMEOUT;
						whatToDoNext =  JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Timeout in operation " + jvxSequencerElementType_txt(theStep->step_tp) + " for sequencer step <" + theStep->description +">, component type " + jvxComponentType_txt(theStep->step_tp) + ".";
					}
				}
			}
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			// Next, report what happened if desired
			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

			if(res != JVX_NO_ERROR)
			{
				if(this_pointer->_common_set_sequencer.inOperation.report)
				{
					if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_STEP_ERROR)
					{
						this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_STEP_ERROR,
							errorDescription.c_str(), this_pointer->_common_set_sequencer.inOperation.idxSequence,
							this_pointer->_common_set_sequencer.inOperation.idxStep,
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp,
							theStep->step_tp);
					}
				}
			}
			else
			{
				if(whatToDoNext != JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT)
				{
					if(this_pointer->_common_set_sequencer.inOperation.report)
					{
						if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_STEP)
						{
							this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_STEP,
								theStep->description.c_str(),
								this_pointer->_common_set_sequencer.inOperation.idxSequence,
								this_pointer->_common_set_sequencer.inOperation.idxStep,
								this_pointer->_common_set_sequencer.inOperation.currentQueueTp,
								theStep->step_tp);
						}
					}
				}
			}
		}

		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		// Find the next valid step or terminate
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		bool runInner = true;
		while(runInner)
		{
			switch(whatToDoNext)
			{
			case JVX_SEQUENCER_PROCEDE_NEED_TO_WAIT:

				// If we need to wait, stop this loop
				runInner = false;
				run = false;
				break;

			case JVX_SEQUENCER_PROCEDE_EXIT_ERROR:

				// If we see this, we need to cancel the run queue and switch to leave quque
				// If there is no more element in leave queue, report end of everything
				if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				{
					// Switch to leave queue
					this_pointer->_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_LEAVE;

					// Set the seqeuner into error mode
					this_pointer->_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_ERROR;

					// Start proceding with LEAVE queue
					if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size() > 0)
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						this_pointer->_common_set_sequencer.inOperation.idxStep = 0;
						res = JVX_NO_ERROR;
					}
					else
					{
						reportEndOfSequence = true;
						reportEndOfProcess = true;
						run = false;
					}
					runInner = false;
				}
				else
				{
					// Move sequence counter forward
					this_pointer->_common_set_sequencer.inOperation.idxStep++;

					// If the previous one was not the last one, procede with next step - even in case of error
					if(this_pointer->_common_set_sequencer.inOperation.idxStep < this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size())
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						res = JVX_NO_ERROR;
					}
					else
					{
						reportEndOfSequence = true;
						reportEndOfProcess = true;
						run = false;
					}

					res = JVX_NO_ERROR;
					runInner = false;
				}
				break;

			case JVX_SEQUENCER_PROCEDE_WANTS_TO_SHUTDOWN:

				this_pointer->_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS;
				if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				{
					this_pointer->_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_LEAVE;

					if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size() > 0)
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						this_pointer->_common_set_sequencer.inOperation.idxStep = 0;
						res = JVX_NO_ERROR;
					}
					else
					{
						reportEndOfSequence = true;
						reportEndOfProcess = true;
						run = false;
					}
					runInner = false;
				}
				else
				{
					// Move sequence counter forward
					this_pointer->_common_set_sequencer.inOperation.idxStep++;

					// If the previous one was not the last one, procede with next step - even in case of error
					if(this_pointer->_common_set_sequencer.inOperation.idxStep < this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size())
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						res = JVX_NO_ERROR;
					}
					else
					{
						reportEndOfSequence = true;
						reportEndOfProcess = true;
						run = false;
					}

					res = JVX_NO_ERROR;
					runInner = false;
				}
				break;


			case JVX_SEQUENCER_PROCEDE_JUMP_TO_LABEL:

				if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				{
					bool foundLabel = false;
					jvxSize i;
					for(i = 0; i < this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps.size(); i++)
					{
						if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps[i].step_label == label_next_step)
						{
							this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							this_pointer->_common_set_sequencer.inOperation.idxStep = (jvxInt16)i;
							foundLabel = true;
							break;
						}
					}
					if(!foundLabel)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Unable to jump to label " + label_next_step;
						if(this_pointer->_common_set_sequencer.inOperation.report)
						{
							if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_STEP)
							{
								this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_STEP_ERROR,
									errorDescription.c_str(), this_pointer->_common_set_sequencer.inOperation.idxSequence,
									this_pointer->_common_set_sequencer.inOperation.idxStep,
									this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
							}
						}
					}
					else
					{
						runInner = false;
					}
				}
				else
				{
					bool foundLabel = false;
					jvxSize i;
					for(i = 0; i < this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size(); i++)
					{
						if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps[i].step_label == label_next_step)
						{
							this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							this_pointer->_common_set_sequencer.inOperation.idxStep = (jvxInt16)i;
							foundLabel = true;
							break;
						}
					}
					if(!foundLabel)
					{
						whatToDoNext = JVX_SEQUENCER_PROCEDE_EXIT_ERROR;
						errorDescription = "Unable to jump to label " + label_next_step;
						if(this_pointer->_common_set_sequencer.inOperation.report)
						{
							if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_STEP)
							{
								this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_STEP_ERROR,
									errorDescription.c_str(), this_pointer->_common_set_sequencer.inOperation.idxSequence,
									this_pointer->_common_set_sequencer.inOperation.idxStep,
									this_pointer->_common_set_sequencer.inOperation.currentQueueTp,
									theStep->step_tp);
							}
						}

						// This is a critical situation, sequencer needs to emergency leave
						reportEndOfProcess = true;
						reportEndOfSequence = true;
						run = false;
					}
					else
					{
						runInner = false;
					}
				}

				break;
			case JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP:
			case JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_LEAVE_LOOP:

				if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				{
					this_pointer->_common_set_sequencer.inOperation.idxStep++;
					if(this_pointer->_common_set_sequencer.inOperation.idxStep >= (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps.size())
					{
						if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size() > 0)
						{
							this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							this_pointer->_common_set_sequencer.inOperation.idxStep = 0;
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_LEAVE;
							runInner = false;
						}
						else
						{
							runInner = false;

							if(
								(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS)||
								(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_ERROR))
							{
								reportEndOfSequence = true;
								reportEndOfProcess = true;
								run = false;
							}
							else
							{
								// Try to procede with next sequence
								if(!this_pointer->findNextValidStep(this_pointer->_common_set_sequencer.inOperation.idxSequence+1))
								{
									// There is no further sequence, let us restart if enabled
									if(this_pointer->_common_set_sequencer.inOperation.loop_enabled)
									{
										// Should never fail
										assert(this_pointer->findNextValidStep(0));
									}
									else
									{
										reportEndOfSequence = true;
										reportEndOfProcess = true;
										run = false;
									}
								}
								else
								{
									this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
								}
							}
						}
					}
					else
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						runInner = false;
					}
				}//if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				else
				{
					this_pointer->_common_set_sequencer.inOperation.idxStep++;
					if(this_pointer->_common_set_sequencer.inOperation.idxStep < (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size())
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						runInner = false;
					}
					else
					{
						reportEndOfSequence = true;
						runInner = false;

						if(
							(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS)||
							(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_ERROR))
						{
							reportEndOfProcess = true;
							run = false;
						}
						else
						{
							// Try to procede with next sequence
							if(!this_pointer->findNextValidStep(this_pointer->_common_set_sequencer.inOperation.idxSequence+1))
							{
								// There is no further sequence, let us restart if enabled
								if(this_pointer->_common_set_sequencer.inOperation.loop_enabled)
								{
									// Should never fail
									assert(this_pointer->findNextValidStep(0));
								}
								else
								{
									reportEndOfSequence = true;
									reportEndOfProcess = true;
									run = false;
								}
							}
							else
							{
								this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							}
						}
					}
				}
				if(whatToDoNext == JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_LEAVE_LOOP)
				{
					// We want to leave the loop here
					run = false;
				}
				break;

			case JVX_SEQUENCER_PROCEDE_NEXT_REGULAR_STEP_INCREMENT:

				if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				{
					this_pointer->_common_set_sequencer.inOperation.idxStep += thePcOffset;
					if(this_pointer->_common_set_sequencer.inOperation.idxStep >= (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfRunningSteps.size())
					{
						if(this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size() > 0)
						{
							this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							this_pointer->_common_set_sequencer.inOperation.idxStep = 0;
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp = JVX_SEQUENCER_QUEUE_TYPE_LEAVE;
							runInner = false;
						}
						else
						{
							runInner = false;

							if(
								(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS)||
								(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_ERROR))
							{
								reportEndOfSequence = true;
								reportEndOfProcess = true;
								run = false;
							}
							else
							{
								// Try to procede with next sequence
								if(!this_pointer->findNextValidStep(this_pointer->_common_set_sequencer.inOperation.idxSequence+1))
								{
									// There is no further sequence, let us restart if enabled
									if(this_pointer->_common_set_sequencer.inOperation.loop_enabled)
									{
										// Should never fail
										assert(this_pointer->findNextValidStep(0));
									}
									else
									{
										reportEndOfSequence = true;
										reportEndOfProcess = true;
										run = false;
									}
								}
								else
								{
									this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
								}
							}
						}
					}
					else
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						runInner = false;
					}
				}//if(this_pointer->_common_set_sequencer.inOperation.currentQueueTp == JVX_SEQUENCER_QUEUE_TYPE_RUN)
				else
				{
					this_pointer->_common_set_sequencer.inOperation.idxStep += thePcOffset;
					if(this_pointer->_common_set_sequencer.inOperation.idxStep < (jvxInt16)this_pointer->_common_set_sequencer.theSequences.all_sequences[this_pointer->_common_set_sequencer.inOperation.idxSequence].lstOfLeaveSteps.size())
					{
						this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
						runInner = false;
					}
					else
					{
						reportEndOfSequence = true;
						runInner = false;

						if(
							(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS)||
							(this_pointer->_common_set_sequencer.inOperation.theSeqState == JVX_SEQUENCER_STATUS_ERROR))
						{
							reportEndOfProcess = true;
							run = false;
						}
						else
						{
							// Try to procede with next sequence
							if(!this_pointer->findNextValidStep(this_pointer->_common_set_sequencer.inOperation.idxSequence+1))
							{
								// There is no further sequence, let us restart if enabled
								if(this_pointer->_common_set_sequencer.inOperation.loop_enabled)
								{
									// Should never fail
									assert(this_pointer->findNextValidStep(0));
								}
								else
								{
									reportEndOfSequence = true;
									reportEndOfProcess = true;
									run = false;
								}
							}
							else
							{
								this_pointer->_common_set_sequencer.inOperation.timestamp_store = timestamp_us;
							}
						}
					}
				}
				break;
			}//switch(whatToDoNext)
		}//while(runInner)
		if(reportEndOfSequence)
		{
			switch(this_pointer->_common_set_sequencer.inOperation.theSeqState)
			{
			case JVX_SEQUENCER_STATUS_ERROR:
				if(this_pointer->_common_set_sequencer.inOperation.report)
				{
					if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ERROR)
					{
						this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ERROR,
							"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
							this_pointer->_common_set_sequencer.inOperation.idxStep,
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
					}
				}
				break;
			case JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS:
				if(this_pointer->_common_set_sequencer.inOperation.report)
				{
					if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_SEQUENCE_ABORT)
					{
						this_pointer->_common_set_sequencer.inOperation.report->report_event(
							//JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ABORT,
							howtosaygoodbye,
							"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
							this_pointer->_common_set_sequencer.inOperation.idxStep,
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
					}
				}
				break;
			case JVX_SEQUENCER_STATUS_IN_OPERATION:
				if(this_pointer->_common_set_sequencer.inOperation.report)
				{
					if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_SEQUENCE)
					{
						this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_SEQUENCE,
							"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
							this_pointer->_common_set_sequencer.inOperation.idxStep,
							this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
					}
				}
				break;

			default:
				assert(0);
			}
			reportEndOfSequence = false;
		}
	}//while(run)

	res = JVX_NO_ERROR;

	if(reportEndOfProcess)
	{
		switch(this_pointer->_common_set_sequencer.inOperation.theSeqState)
		{
		case JVX_SEQUENCER_STATUS_ERROR:
			if(this_pointer->_common_set_sequencer.inOperation.report)
			{
				if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ERROR)
				{
					this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ERROR,
						"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
						this_pointer->_common_set_sequencer.inOperation.idxStep,
						this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
				}
			}
			break;
		case JVX_SEQUENCER_STATUS_SHUTDOWN_IN_PROGRESS:
			if(this_pointer->_common_set_sequencer.inOperation.report)
			{
				if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ABORT)
				{
					this_pointer->_common_set_sequencer.inOperation.report->report_event(
						//JVX_SEQUENCER_EVENT_INCOMPLETE_COMPLETION_PROCESS_ABORT,
						howtosaygoodbye,
						"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
						this_pointer->_common_set_sequencer.inOperation.idxStep,
						this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
				}
			}
			break;
		case JVX_SEQUENCER_STATUS_IN_OPERATION:
			if(this_pointer->_common_set_sequencer.inOperation.report)
			{
				if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_PROCESS)
				{
					this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_SUCCESSFUL_COMPLETION_PROCESS,
						"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
						this_pointer->_common_set_sequencer.inOperation.idxStep,
						this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
				}
			}
			break;

		default:
			assert(0);
		}

		this_pointer->_common_set_sequencer.inOperation.theSeqState = JVX_SEQUENCER_STATUS_SHUTDOWN_COMPLETE;
		res = JVX_ERROR_ABORT;
	}
	return res;
}

jvxErrorType
CjvxHost::cb_wokeup(jvxHandle* privateData_queue, jvxInt64 timestamp_us)
{
	return JVX_NO_ERROR;
}

jvxErrorType
CjvxHost::cb_stopped(jvxHandle* privateData_queue, jvxInt64 timestamp_us)
{
	CjvxHost* this_pointer = (CjvxHost*)privateData_queue;
	if(this_pointer->_common_set_sequencer.inOperation.report)
	{
		if(this_pointer->_common_set_sequencer.inOperation.eventMask & JVX_SEQUENCER_EVENT_PROCESS_TERMINATED)
		{
			this_pointer->_common_set_sequencer.inOperation.report->report_event(JVX_SEQUENCER_EVENT_PROCESS_TERMINATED,
				"", this_pointer->_common_set_sequencer.inOperation.idxSequence,
				this_pointer->_common_set_sequencer.inOperation.idxStep,
				this_pointer->_common_set_sequencer.inOperation.currentQueueTp);
		}
	}
	return JVX_NO_ERROR;
}
