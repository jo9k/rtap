/*============================================================
 * FILE: IalgorithmIND.h
 *============================================================
 * PURPOSE: Interface for the main audio processing component 
 *			in RTProc.
 *============================================================
 * COPYRIGHT/AUTHOR:	IND, RWTH-Aachen, Hauke Krueger, hauke@ind.rwth-aachen.de 
 *============================================================
 * For bugs, write an email to audiodev@ind.rwth-aachen.de
 *============================================================*/ 

/**
 * Abstract interface class for algorithm component for realtime audio processing.
 * The interface has functions for audio processing setup and for runtime
 * communication. 
 *
 * For runtime configuration we have added IgenericruntimeIND interface.
 * Runtime messaging is supported by the IruntimemessagsIND interface.
 * Error message reporting is realized in the context of the IerrormesagesIND interface.
 *
 * For a detailled description, refer to the rtprocdoc.pdf document.
 *
 *////===================================================================================
  
#ifndef _IRTPALGORITHM_H__
#define _IRTPALGORITHM_H__

 
#define __DSP_MODE__
#ifdef __DSP_MODE__
 
 //================================================
 // DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP
 //================================================
 
#include "userDef.h"
#include "IrtpExtensions.h"
#include "IrtpGenericRuntime.h"
#include "IrtpObject.h"


// Base class for algorithm components
class IrtpAlgorithm: public IrtpObject, public IrtpExtensions
{
public:
	
    // Return the current processing state
    virtual void getStateAlgorithm(RTP_STATE* stat)  = 0;

    // Select the algorithm
    virtual bool selectAlgorithm(IrtpObject* hostRef, void* privData = 0, int szField = 0) = 0;
    
    // Activate the algorithm
    virtual bool activateAlgorithm(IrtpObject* hostRef)  = 0;
    
	//! Return the number of setups that the plugin can handle, -1 means: try!
	virtual bool getNumberSetupsAlgorithm(int* numSetups) = 0;

	//! Return the setup for index, -1 means: Any value excepted
	virtual bool getSetupAlgorithm(int id, int* samplerate, 
		int* buffersize, int* numberChannelsInput, int* numberChannelsOutput, char nmSetup[STRING_LENGTH]) = 0;

	//! Return the number of processing formats that are supported
	virtual bool getNumberFormatProcessingAlgorithm(int* form) = 0;

	//! Return the format for an index
	virtual bool getFormatProcessingAlgorithm(int id, RTP_DATAFORMAT* format) = 0;

    // Check whether algorithm is ready for processing
    virtual bool isReadyForProcessingAlgorithm()  = 0;

    // Prepare for processing
    virtual bool prepareProcessingAlgorithm()  = 0;

    // Start processing
    virtual bool startProcessingAlgorithm()  = 0;

    // Stop processing, additional timeout to complete interrupts
    virtual bool stopProcessingAlgorithm(unsigned int timeOutCycles) = 0;

    // Main processing routine
    //virtual bool process(DATATYPE outputBuffers[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE], DATATYPE inputBuffers[NUMBER_CHANNELS_INPUT][BUFFERSIZE]) = 0;
	virtual bool process(DATATYPE** outputBuffers, DATATYPE** inputBuffers, 
		unsigned numberChannelsIn, unsigned numberChannelsOut, unsigned buffersize) = 0;
	
    // Post processing
    virtual bool postProcessingAlgorithm() = 0;

    // Deactivate algorithm
    virtual bool deactivateAlgorithm() = 0;

	// Opposite of select
	virtual bool unselectAlgorithm(void* fldSpecific = 0, int szFld = 0) = 0;
	
	// Function to return the latencies for a specific id, see plugin for details
	virtual bool queryLatencyAlgorithm(int* lat) = 0;

};

#else // __DSP_MODE__ is not defined

//==========================================
// PC PC PC PC PC PC PC PC PC PC PC PC PC PC
//==========================================

#include <windows.h>
#include "rtproc.h"

interface IrtpAlgorithm: public IrtpObject, public IrtpExtensions 
{

	//! Return the state of the plugin
	virtual rtpBool RTPROC_CALLINGCONVENTION getStateAlgorithm(rtpState* stat) = 0;
	
	virtual rtpBool RTPROC_CALLINGCONVENTION selectAlgorithm(IrtpHost* hostRef, rtpHandle* privData = 0, rtpSize szField = 0) = 0;

	//! Initialize-function will be directly called after constructor
	virtual rtpBool RTPROC_CALLINGCONVENTION activateAlgorithm(rtpHandle* field = NULL, rtpSize szField = 0) = 0;

	//! Return the number of setups that the plugin can handle, -1 means: try!
	virtual rtpBool RTPROC_CALLINGCONVENTION getNumberSetupsAlgorithm(rtpInt32* numSetups) = 0;

	//! Return the setup for index, -1 means: Any value excepted
	virtual rtpBool RTPROC_CALLINGCONVENTION getSetupAlgorithm(rtpIdx id, rtpInt32* samplerate, 
		rtpInt32* buffersize, rtpInt32* numberChannelsInput, rtpInt32* numberChannelsOutput, rtpString** nmSetup) = 0;

	//! Return the number of processing formats that are supported
	virtual rtpBool RTPROC_CALLINGCONVENTION getNumberFormatProcessingAlgorithm(rtpInt32* form) = 0;

	//! Return the format for an index
	virtual rtpBool RTPROC_CALLINGCONVENTION getFormatProcessingAlgorithm(rtpIdx id, rtpDataFormat* format) = 0;

	//! Function being called by the host before trying to start audioprocessing
	virtual rtpBool RTPROC_CALLINGCONVENTION isReadyForProcessingAlgorithm() = 0;

	//! Callback to provide the actual setup for procesing from host
	virtual rtpBool RTPROC_CALLINGCONVENTION prepareProcessingAlgorithm(rtpInt32 sampleRate, 
		rtpBool* successSamplerate, rtpInt32 buffersize, rtpBool* successBuffersize,
		rtpInt32 channelsInput, rtpBool* successChansInput, rtpInt32 channelsOutput, rtpBool* successChansOutput,
		rtpDataFormat format, rtpBool* successFormat, rtpInt32 selectionID = -1, rtpBool* successSelectionID = NULL) = 0;

	//! After all, call this function to indicate that processing will very soon begin
	virtual rtpBool RTPROC_CALLINGCONVENTION startProcessingAlgorithm() = 0;

	//! Indicate that procesing has stopped
	virtual rtpBool RTPROC_CALLINGCONVENTION stopProcessingAlgorithm(rtpInt32 waitMS = 0) = 0;

	//! Process will be called for each audio-buffer inbetween start/stopProcesing
	virtual rtpBool RTPROC_CALLINGCONVENTION process(void** fieldOutput, const void** fieldInput, 
		rtpInt32 numberChannelsOut, rtpInt32 numberChannelsIn, rtpInt32 buffersize, 
		rtpDataFormat processingFormat) = 0;

	//! Function as opposite of the prepare function to have a clean switch of states
	virtual rtpBool RTPROC_CALLINGCONVENTION postProcessingAlgorithm(rtpIdx id = 0, rtpHandle* field = 0, 
		rtpSize szField = 0) = 0;

	//! Deactivate to set the state back to selected
	virtual rtpBool RTPROC_CALLINGCONVENTION deactivateAlgorithm(rtpHandle* fldSpecific = 0, rtpSize szFld = 0) = 0;

	//! Opposite of select
	virtual rtpBool RTPROC_CALLINGCONVENTION unselectAlgorithm(rtpHandle* fldSpecific = 0, rtpSize szFld = 0) = 0;

	//! Function to return the latencies for a specific id, see plugin for details
	virtual rtpBool RTPROC_CALLINGCONVENTION queryLatencyAlgorithm(rtpInt32* lat) = 0;
};

#endif //__DSP_MODE__

#endif //_IRTPALGORITHM_H__
