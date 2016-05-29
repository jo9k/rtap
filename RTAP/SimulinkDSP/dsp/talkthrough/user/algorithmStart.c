/******************************************************
 * Filename: algorithmStart.c						  *
 * Description: Algorithm component for the students  *
 *				to start with, containing both generic*
 *		 		and Midi RS232 support.			      *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include <string.h>
#include <stdlib.h>

#include "algorithmStart.h"

// Two possible implementation of the talkthrough are available,
// use this macro to use the efficient one. The difference is 
// round about 4 percent of load
#define EFFICIENT_IMPLEMENTATION

#include "accessSPORT.h"
extern accessSPORT* theSPORTClass;

// ===================================
// Matlab debug stuff
// ===================================
#ifdef DEBUG_MATLAB

#include "debugCode.h"
extern debugCode theDebugCodeClass;
extern CalgorithmStart theClass;

// Configure module to expose the process callback via RS232 debugging
char name_process_extern[STRING_LENGTH-1] = "process-extern";
WORD32 process_extern(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);
	
#endif
// ===================================

#ifdef USE_UART_FOR_MIDI
#else
#include "uartDispatcherGeneric.h"
#endif

#ifdef USE_UART_FOR_MIDI
CalgorithmStart::CalgorithmStart(): CrtpAlgorithm(), CuartReceiverMidi()
#else
CalgorithmStart::CalgorithmStart(): CrtpAlgorithm(), CuartReceiverGeneric()
#endif
{
	memcpy(_nameAlgorithm, "USER CLASS: START", STRING_LENGTH);
	memcpy(_descriptionAlgorithm, "TALKTHROUGH FUNCTION", STRING_LENGTH);
	
#ifdef USE_UART_FOR_MIDI
	level = 0.0;
#else
	level = 1;
#endif
}

bool CalgorithmStart::activateAlgorithm(IrtpObject* hostRef)
{
	CrtpAlgorithm::activateAlgorithm(hostRef);

#ifdef DEBUG_MATLAB
	theDebugCodeClass.registerFunction(process_extern, 1,1, name_process_extern);
#endif
    return(true);
}

/*==================================================
 * Function to process input samples to output samples.
 * Here , pass control to the base class.
 *///===============================================
//bool 
//CalgorithmStart::process(DATATYPE outputBuffers[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE], 
//						 DATATYPE inputBuffers[NUMBER_CHANNELS_INPUT][BUFFERSIZE])
bool 
CalgorithmStart::process(DATATYPE** outputBuffers, DATATYPE** inputBuffers, 
	unsigned numberChannelsOut, unsigned numberChannelsIn, unsigned buffersize)
{
	// CrtpAlgorithm::process(outputBuffers, inputBuffers, numberChannelsIn, numberChannelsOut, buffersize);
	int i,j;
	
	// Compute the minmum number of channels from input and output
	int chans = min(numberChannelsIn, numberChannelsOut);

	// ======================================================================
	// Two implementations for talkthrough, one efficient and one inefficient
	// ======================================================================

#ifdef EFFICIENT_IMPLEMENTATION
	DATATYPE* outPtr = NULL;
	DATATYPE* inPtr = NULL;
	for(j = 0; j < chans; j++)
	{
		outPtr = outputBuffers[j];
		inPtr = inputBuffers[j];
		for(i = 0; i < buffersize; i++)
		{
			//outputBuffers[j][i] = inputBuffers[j][i] * level;
			*outPtr++ = *inPtr++ * level;
		}
	}
#else
	for(j = 0; j < chans; j++)
	{
		for(i = 0; i < buffersize; i++)
		{
			outputBuffers[j][i] = inputBuffers[j][i] * level;
		}
	}
#endif
		
	for(;j < numberChannelsOut; j++)
	{
		memset(outputBuffers[j], 0, buffersize);
	}

	return(true);	
}

#ifdef USE_UART_FOR_MIDI

// = = = = = = = = = = = = = = = = = = = = = = = = = =
// MIDI PROCESSING MIDI PROCESSING MIDI PROCESSING
// = = = = = = = = = = = = = = = = = = = = = = = = = =

/*==================================================
 * Callback to indicate that a new note has been triggered.
 *///===============================================
void 
CalgorithmStart::reportNoteOn(char valueKey, char valueVelocity, char channel)
{
	level = valueVelocity/128.0;
}

/*==================================================
 * Callback to indicate that a note was released.
 *///===============================================
void 
CalgorithmStart::reportNoteOff(char valueKey, char valueVelocity, char channel)
{
	level = 0.0;
}
#else

// = = = = = = = = = = = = = = = = = = = = = = = = = =
// GENERIC PROCESSING GENERIC PROCESSING GENERIC PROCESSING
// = = = = = = = = = = = = = = = = = = = = = = = = = =

/*==================================================
 * Callback to receive incoming generic messages.
 *///===============================================
void
CalgorithmStart::incomingDefaultMessage(DATATYPE_RS232_CHAR purpose, 
	DATATYPE_RS232* fldLoad, UWORD16 numElements)
{
	float loadF; 
	switch(purpose)
	{
		case 0:
			
			// send message right back
			if(!_theDispatcher->sendMessageNonBlocking(purpose, fldLoad, numElements, 0));
			{
				// report error
			}
			break;
		case 1:
			// Accept incoming level control here
			if(numElements >= 1)
			{
				level = *((float*)fldLoad);
			}
			break;
		case 2:
			// Opcode 2: Send the curent load
			
			// Get the load
			loadF = theSPORTClass->load;
			
			// Send the value as float (pack as 32 bit integer)
			if(!_theDispatcher->sendMessageNonBlocking(purpose, (DATATYPE_RS232*)&loadF, 
					(DATATYPE_RS232_CHAR)1, 0));
			{
				// report error
			}
			
		default:
			// send message unsopported back
			break;
	}
}
#endif

/**
 * Callback to invoke the main procesing function during operation to
 * control the main processing callback from within Matlab
 *///===================================================
WORD32
process_extern(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	bool formatOK = true;
	int bSize = BUFFERSIZE;
	
	// Input/Output configuration must match the processing parameters
	if(numArgsIn != NUMBER_CHANNELS_INPUT)
	{
		return(-1);
	}
	if(numArgsOut != NUMBER_CHANNELS_OUTPUT)
	{
		return(-2);
	}
	
	formatOK = true;
	for(int i = 0; i < numArgsIn; i++)
	{
		if(fLengthsIn[i] != BUFFERSIZE)
		{
			formatOK = false;
		}
	}
	if(!formatOK)
	{
		return(-3);
	}
	
	formatOK = true;
	for(int i = 0; i < numArgsIn; i++)
	{
		if(fLengthsIn[i] != BUFFERSIZE)
		{
			formatOK = false;
		}
	}
	if(!formatOK)
	{
		return(-4);
	}
	
	// Call the main process function callback		
	if(theClass.process(outFields, inFields, numArgsOut, numArgsIn, BUFFERSIZE))
	{
		return(0);
	}
	return(-5);
}


