/* 
 *****************************************************
 * Filename: IjvxAlgorithmCore.h
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

#ifndef __IJVXALGORITHMCORE__H__
#define __IJVXALGORITHMCORE__H__

#include "jvx.h"

JVX_INTERFACE IjvxAlgorithmCore
{
 public:
  virtual JVX_CALLINGCONVENTION ~IjvxAlgorithmCore(){};

	//! Callback to provide the actual setup for procesing from host
	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(jvxInt32 sampleRate, 
		jvxBool* successSamplerate, jvxInt32 buffersize, jvxBool* successBuffersize,
		jvxInt32 channelsInput, jvxBool* successChansInput, jvxInt32 channelsOutput, jvxBool* successChansOutput,
		jvxDataFormat format, jvxBool* successFormat, jvxInt32 selectionID = -1, jvxBool* successSelectionID = NULL) = 0;

	//==================================================================
	// PROCESSING PROCESSING PROCESSING PROCESSING PROCESSING PROCESSING
	//==================================================================

	//! Process will be called for each audio-buffer inbetween start/stopProcesing
	virtual jvxErrorType JVX_CALLINGCONVENTION process(void** fieldOutput, const void** fieldInput, 
		jvxInt32 numberChannelsOut, jvxInt32 numberChannelsIn, jvxInt32 buffersize, 
		jvxDataFormat processingFormat, jvxInt64 timestamp_us) = 0;

		//! Function as opposite of the prepare function to have a clean switch of states
	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess() = 0;
};

#endif
