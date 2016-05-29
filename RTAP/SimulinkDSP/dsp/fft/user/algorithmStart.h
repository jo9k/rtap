/******************************************************
 * Filename: algorithmStart.c						  *
 * Description: Algorithm component for the students  *
 *				to start with, conatining both generic*
 *		 		and Midi RS232 support.			      *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef __ALGORITHMSTART_H__
#define __ALGORITHMSTART_H__

#include <filter.h> 
#include <math.h>

#include "userDef.h"
#include "CrtpAlgorithm.h"

#ifdef USE_UART_FOR_MIDI
#include "uartReceiverMidi.h"
#else
#include "uartReceiverGeneric.h"
#endif

/*==================================================
 * Component definition. Depending on the Midi or generic
 * mode, different member functions are available.
 *///===============================================
#ifdef USE_UART_FOR_MIDI
class CalgorithmStart: public CrtpAlgorithm, public CuartReceiverMidi
#else
class CalgorithmStart: public CrtpAlgorithm, public CuartReceiverGeneric
#endif
{
public:
	
	// Multiplication factor for audio output
	float level;
	
	// Constructor
	CalgorithmStart();
	
#ifdef USE_UART_FOR_MIDI

	// = = = = = = = = = = = = = = = = = = = = = = = = = =
	// MIDI PROCESSING MIDI PROCESSING MIDI PROCESSING
	// = = = = = = = = = = = = = = = = = = = = = = = = = =
    
	//! Accept incoming note on
    void reportNoteOn(char valueKey, char valueVelocity, char channel);

    //! Accept incoming note off
    void reportNoteOff(char valueKey, char valueVelocity, char channel);
#else

	// = = = = = = = = = = = = = = = = = = = = = = = = = =
	// GENERIC PROCESSING GENERIC PROCESSING GENERIC PROCESSING
	// = = = = = = = = = = = = = = = = = = = = = = = = = =
    //! Accept incoming generic message
	void incomingDefaultMessage(DATATYPE_RS232_CHAR purpose, 
		DATATYPE_RS232* fldLoad, UWORD16 numElements);
#endif

	bool activateAlgorithm(IrtpObject* hostRef);

	//! Audio processing callback
    //bool process(DATATYPE outputBuffers[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE], DATATYPE inputBuffers[NUMBER_CHANNELS_INPUT][BUFFERSIZE]);
    bool process(DATATYPE** outputBuffers, DATATYPE** inputBuffers, 
    	unsigned numberChannelsOut, unsigned numberChannelsIn, unsigned buffersize);
    
	void executeFFT(const complex_float pm* twiddle, float* input, complex_float * output, WORD32 nFFT);
    
	void executeIFFT(const complex_float pm* twiddle, complex_float* input, 
    				 complex_float* output, WORD32 nIFFT);
};

#endif
