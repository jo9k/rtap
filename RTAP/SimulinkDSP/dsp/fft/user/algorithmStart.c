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

#ifdef DEBUG_MATLAB

#include "debugCode.h"
extern debugCode theDebugCodeClass;
extern CalgorithmStart theClass;

// Configure module to expose the process callback via RS232 debugging
char name_process_extern[STRING_LENGTH-1] = "process-extern";
WORD32 process_extern(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);

// Configure module to allow modification of filter parameters
char name_fft[STRING_LENGTH-1] = "processFFT";
WORD32 computeFFT(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);
	
// Configure module to allow modification of filter parameters
char name_ifft[STRING_LENGTH-1] = "processFFT";
WORD32 computeIFFT(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);

char name_weights[STRING_LENGTH-1] = "Set frq wghts";
WORD32 setWeights(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut);

#endif

// Prepare everything for FFT/IFFT filter
#define COPY_BEFORE_FFT
#define N_FFT BUFFERSIZE
#ifdef COPY_BEFORE_FFT
float dm inputFFT[N_FFT];
#endif
complex_float pm twiddle[N_FFT/2]; 
complex_float dm outputFFT[N_FFT];
complex_float dm outputIFFT[N_FFT];
float weights[N_FFT];

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
	int i;
	CrtpAlgorithm::activateAlgorithm(hostRef);

	// Initiaize the tiddle facs for FFT
	twidfft(twiddle, N_FFT);
	memset(outputFFT, 0, sizeof(complex_float)*N_FFT);
	memset(outputIFFT, 0, sizeof(complex_float)*N_FFT);
	for(i = 0; i < N_FFT; i++)
	{
		weights[i] = 1.0;
	}

#ifdef DEBUG_MATLAB
	theDebugCodeClass.registerFunction(process_extern, 1,1, name_process_extern);
	theDebugCodeClass.registerFunction(computeFFT, 1,2, name_fft);
	theDebugCodeClass.registerFunction(computeIFFT, 1,2, name_ifft);
	theDebugCodeClass.registerFunction(setWeights, 1,0, name_weights);
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

	// The temp output from the FFT is outputFFT, the twiddle factors are twiddle

	// = = = = = = = = = = = = = = = = = = = = = = = = = = =
	// = = = = = = = = Left channel = = = = = = = = = = = =
	// = = = = = = = = = = = = = = = = = = = = = = = = = = =
	this->executeFFT(twiddle, ptrLeftIn, outputFFT, buffersize);
	
	// Weight the frequency bins
	for(i = 0; i < buffersize; i++)
	{
		outputFFT[i].re *= weights[i];
		outputFFT[i].im *= weights[i];
	}
	
	// Compute the IFFT
	this->executeIFFT(twiddle, outputFFT, outputIFFT, buffersize);
	
	// Store audio samples back to audio buffers for playback
	for(i = 0; i < buffersize; i++)
	{
		ptrLeftOut[i] = outputIFFT[i].re;
	}
	
	// = = = = = = = = = = = = = = = = = = = = = = = = = = =
	// = = = = = = = = Right channel = = = = = = = = = = = =
	// = = = = = = = = = = = = = = = = = = = = = = = = = = =
	this->executeFFT(twiddle, ptrRightIn, outputFFT, buffersize);
	
	// Weight the frequency bins
	for(i = 0; i < buffersize; i++)
	{
		outputFFT[i].re *= weights[i];
		outputFFT[i].im *= weights[i];
	}
	
	// Compute the IFFT 
	this->executeIFFT(twiddle, outputFFT, outputIFFT, buffersize);
	
	// Store audio data back to output buffer
	for(i = 0; i < buffersize; i++)
	{
		ptrRightOut[i] = outputIFFT[i].re;
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
 * Execute the FFT function. We use an extra functioncall since we
 * want to use this function in the context of the Matlab-Debugger!
 *///===============================================================
void
CalgorithmStart::executeFFT(const complex_float pm* twiddle, float* input, 
							complex_float* output, WORD32 nFFT)
{
	
	// We need to copy the content before executing the FFT
	// to avoid that FFT is buggy for wrong memory addresses (use dm fields!)
#ifdef COPY_BEFORE_FFT
	memcpy(inputFFT, input, nFFT);

	// This call modifies the input buffer
   	rfft(inputFFT, NULL, output, twiddle, 1, nFFT); 
#else
   	rfft(input, NULL, output, twiddle, 1, nFFT); 
#endif

   	// Copy second half of complex output buffer
   	for(int i = 1; i < N_FFT/2; i++)
   	{
   		output[N_FFT/2 + i].re = output[N_FFT/2 - i].re;
   		output[N_FFT/2 + i].im = -output[N_FFT/2 - i].im;
   	} 	
}

/**
 * Execute the IFFT function. We use an extra functioncall since we
 * want to use this function in the context of the Matlab-Debugger!
 *///===============================================================
void
CalgorithmStart::executeIFFT(const complex_float pm* twiddle, 
							complex_float* input, complex_float* output, 
							WORD32 nIFFT)
{
	// This call modifies the input buffer
   	ifft(input, NULL, output, twiddle, 1, nIFFT); 
}

//============================================================
// Debug Matlab entry function
//============================================================

#ifdef DEBUG_MATLAB

/**
 * External access to main processing function
 *///==========================================================
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

/**
 * External access to FFT function
 *///==========================================================
WORD32
computeFFT(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	if(!((numArgsIn == 1)&&(numArgsOut == 2)))
	{		
		return(-1);
	}
	if(fLengthsIn[0] != N_FFT)
	{
		return(-2);
	}
	if(fLengthsOut[0] != N_FFT)
	{
		return(-3);
	}
	if(fLengthsOut[1] != N_FFT)
	{
		return(-4);
	}
	
	theClass.executeFFT(twiddle, inFields[0], outputFFT, fLengthsIn[0]); 
	
	// Copy first half of complex output buffer
	for(int i = 0; i < N_FFT; i++)
	{
   		outFields[0][i] = outputFFT[i].re;
   		outFields[1][i] = outputFFT[i].im;
   	}
   	
   	// Done processing	
   	return(0);
}

/**
 * External access to IFFT function
 *///==========================================================
WORD32
computeIFFT(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	if(!((numArgsIn == 2)&&(numArgsOut == 1)))
	{		
		return(-1);
	}
	if(fLengthsIn[0] != N_FFT)
	{
		return(-2);
	}
	if(fLengthsIn[1] != N_FFT)
	{
		return(-3);
	}
	if(fLengthsOut[0] != N_FFT)
	{
		return(-4);
	}
	
	for(int i = 0; i < N_FFT; i++)
	{
		outputFFT[i].re = inFields[0][i];
		outputFFT[i].im = inFields[1][i];
	}
	
	theClass.executeIFFT(twiddle, outputFFT, outputIFFT, fLengthsIn[0]); 
	
	// Copy first half of complex output buffer, only use the real part
	for(int i = 0; i < N_FFT; i++)
	{
   		outFields[0][i] = outputIFFT[i].re;
   	}
   	
   	// Done processing	
   	return(0);
}

/**
 * Set weights from Matlab Debug interface
 *///==========================================================
WORD32
setWeights(float** inFields, WORD32 numArgsIn, WORD32* fLengthsIn,
	float** outFields, WORD32 numArgsOut, WORD32* fLengthsOut)
{
	if(!((numArgsIn == 1)&&(numArgsOut == 0)))
	{		
		return(-1);
	}
	if(fLengthsIn[0] != N_FFT)
	{
		return(-2);
	}
	memcpy(weights, inFields[0], N_FFT);
	return(0);
}
			
#endif
