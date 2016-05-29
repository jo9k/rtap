/* 
 *****************************************************
 * Filename: IjvxAlgorithm.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: Interface for the main audio 
 *				processing component in JVXroc.
 *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __IALGORITHM__H__
#define __IALGORITHM__H__

#include "jvx.h"

/**
 * Abstract JVX_INTERFACE class for algorithm component for realtime audio processing.
 * The JVX_INTERFACE has functions for audio processing setup and for runtime
 * communication. 
 *
 * For runtime configuration we have added IgenericruntimeIND interface.
 * Runtime messaging is supported by the IruntimemessagsIND interface.
 * Error message reporting is realized in the context of the IerrormesagesIND interface.
 *
 * For a detailled description, refer to the jvxrocdoc.pdf document.
 *
 *////===================================================================================
JVX_INTERFACE IjvxAlgorithm: public IjvxAlgorithmCore, public IjvxObject, public IjvxHiddenInterface
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxAlgorithm(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION initialize(IjvxHost* hostRef) = 0;

	//===================================================================
	// INIT INIT INIT INIT INIT INIT INIT INIT INIT INIT INIT INIT INIT
	//===================================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION select() = 0;

	//! Initialize-function will be directly called after constructor
	virtual jvxErrorType JVX_CALLINGCONVENTION activate() = 0;

	//===================================================================
	// SETUPS SETUPS SETUPS SETUPS SETUPS SETUPS SETUPS SETUPS SETUPS SETUPS
	//===================================================================

	//! Return the number of setups that the plugin can handle, -1 means: try!
	virtual jvxErrorType JVX_CALLINGCONVENTION number_setups(jvxInt32* numSetups) = 0;

	//! Return the setup for index, -1 means: Any value excepted
	virtual jvxErrorType JVX_CALLINGCONVENTION setup(jvxSize id, jvxInt32* samplerate, 
		jvxInt32* buffersize, jvxInt32* numberChannelsInput, jvxInt32* numberChannelsOutput, jvxString** nmSetup) = 0;

	//! Return the number of processing formats that are supported
	virtual jvxErrorType JVX_CALLINGCONVENTION number_formats(jvxInt32* form) = 0;

	//! Return the format for an index
	virtual jvxErrorType JVX_CALLINGCONVENTION format(jvxSize id, jvxDataFormat* format) = 0;

	//! Return the format for an index
	virtual jvxErrorType JVX_CALLINGCONVENTION preferred_setup(jvxInt32* samplerate, jvxInt32* buffersize, 
		jvxInt32* numberChannelsInput, jvxInt32* numberChannelsOutput, jvxDataFormat* dataFormat, jvxInt32* processingID) = 0;

	//! Function to set and test the processing parameters
	virtual jvxErrorType JVX_CALLINGCONVENTION set_test_parameters(jvxInt32 sampleRate, jvxBool* successSamplerate, 
		jvxInt32 buffersize, jvxBool* successBuffersize,
		jvxInt32 channelsInput, jvxBool* successChansInput, 
		jvxInt32 channelsOutput, jvxBool* successChansOutput,
		jvxDataFormat format, jvxBool* successFormat,
		jvxInt32 selectionID, jvxBool* resSelectionID) = 0;

	//==================================================================
	// PREPARING PROCESSING PREPARING PROCESSING PREPARING PROCESSING PREPARING PROCESSING
	//==================================================================

	//! Function being called by the host before trying to start audioprocessing
	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready() = 0;

	//! After all, call this function to indicate that processing will very soon begin
	virtual jvxErrorType JVX_CALLINGCONVENTION start() = 0;

	//! Indicate that procesing has stopped
	virtual jvxErrorType JVX_CALLINGCONVENTION stop() = 0;


	//====================================================================
	// TERMINATE TERMINATE TERMINATE TERMINATE TERMINATE TERMINATE TERMINATE TERMINATE
	//====================================================================

	//! Deactivate to set the state back to selected
	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate() = 0;

	//! Opposite of select
	virtual jvxErrorType JVX_CALLINGCONVENTION unselect() = 0;

	//==================================================================
	// MISC MISC MISC MISC MISC MISC MISC MISC MISC MISC MISC MISC MISC MISC
	//==================================================================

	//! Function to return the latencies for a specific id, see plugin for details
	virtual jvxErrorType JVX_CALLINGCONVENTION query_latency(jvxInt32* lat) = 0;

	//============================================================================
	// RUNTIME PARAMETERS RUNTIME PARAMETERS RUNTIME PARAMETERS RUNTIME PARAMETERS
	//============================================================================

};

#endif 
