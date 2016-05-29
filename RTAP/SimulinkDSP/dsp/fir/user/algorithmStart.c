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

// =========================================
// Debug with Matlab stuff
#ifdef DEBUG_MATLAB
// =========================================

#include "debugCode.h"
extern debugCode theDebugCodeClass;
extern CalgorithmStart theClass;

// Configure module to expose the process callback via RS232 debugging
char name_process_extern[STRING_LENGTH-1] = "process-extern";
WORD32 process_extern(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);

// Configure module to allow modification of filter parameters
char name_setCoeffs[STRING_LENGTH-1] = "setCoeffs";
WORD32 setCoeffs(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);
	
#endif
// =========================================

// Prepare everything for FIR filter
#define TAPS 31

// Filter coefficients left channel
float pm coeffs_0[TAPS];
float dm states_0[TAPS+1];

// Filter coefficients right channel
float pm coeffs_1[TAPS];
float dm states_1[TAPS+1];


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

	// Allocate states and Dirac coefficients
	memset(states_0, 0, TAPS+1);
	memset(coeffs_0, 0, TAPS);
	coeffs_0[TAPS-1] = 1.0;
	
	// Allocate states and Dirac coefficients
	memset(states_1, 0, TAPS+1);
	memset(coeffs_1, 0, TAPS);
	coeffs_1[TAPS-1] = 1.0;

#ifdef DEBUG_MATLAB
	theDebugCodeClass.registerFunction(process_extern, 1,1, name_process_extern);
	theDebugCodeClass.registerFunction(setCoeffs, 1,0, name_setCoeffs);
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

	// Get the input buffers
	float* ptrLeftIn = inputBuffers[0];
	float* ptrRightIn = inputBuffers[1];

	// Get the output buffers
	float* ptrLeftOut = outputBuffers[0];
	float* ptrRightOut = outputBuffers[1];

	//this->filterFIR(coeffs_0, states_0, ptrLeftIn, ptrLeftOut, buffersize, TAPS);		
	this->filterFIR(coeffs_0, states_0, ptrLeftIn, ptrLeftOut, buffersize, TAPS);
	this->filterFIR(coeffs_1, states_1, ptrRightIn, ptrRightOut, buffersize, TAPS);
	
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

void
CalgorithmStart::filterFIR(const pm float* coeffs, float* states, float* input, float* output, 
							WORD32 fieldL, WORD32 taps)
{
	//fir(input, output, coeffs, states, fieldL, taps);
	float* ret = fir(input, output, coeffs, states, fieldL, taps);
}

WORD32
process_extern(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	bool formatOK = true;
	int bSize = BUFFERSIZE;
	
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
			
	if(theClass.process(outFields, inFields, numArgsOut, numArgsIn, BUFFERSIZE))
	{
		return(0);
	}
	return(-5);
}

WORD32
setCoeffs(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	int i;
	
	if(numArgsIn != 1)
	{
		return(-1);
	}
	if(numArgsOut != 0)
	{
		return(-2);
	}
	if(fLengthsIn[0] != TAPS)
	{
			return(-3);
	}
	for(i = 0; i < TAPS; i++)
	{
		coeffs_0[TAPS-1-i] = inFields[0][i];
		coeffs_1[TAPS-1-i] = inFields[0][i];
	}
	return(0);
}
	

