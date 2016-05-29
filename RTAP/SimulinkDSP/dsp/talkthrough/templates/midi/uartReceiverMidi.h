/******************************************************
 * Filename: uartMidiReceiver.h						  *
 * Description: Default implementation of Midi 		  *
 *				receiver. Only count note on/off 	  *
 *				events.							  	  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#ifdef USE_UART_FOR_MIDI

#ifndef __UARTMIDIRECEIVER_H__
#define __UARTMIDIRECEIVER_H__

#include "IuartApplicationMidi.h"

/*==============================================
 * Class to implement the application side of the
 * RS232 communication. All base class (uartApplication)
 * callbacks have been implemented. The current
 * realization reacts on one message only be returning
 * a specific data field.
 *///==============================================
class CuartReceiverMidi: public IuartApplicationMidi
{
	// Count note on events
    int cntNotes;

   	// Count errors
    int cntErrors;

public:

    // Constructor
    CuartReceiverMidi();

	//! Report note on event
    virtual void reportNoteOn(char valueKey, char valueVelocity, char channel);

	//! Report note on event
    virtual void reportNoteOff(char valueKey, char valueVelocity, char channel);

	//! Report controller modification event
    virtual void reportModulation(char valueModulation);

	//! Report controller modification event
    virtual void reportVolume(char valueVolume);

	//! Report controller modification event
    virtual void reportSoundControl5(char value);

	//! Report controller modification event
    virtual void reportSoundControl10(char value);

	//! Report controller modification event
    virtual void reportPitchBend(int valuePitch);

	//! Report an error
    virtual void reportError(short errorCode);
};

#endif
#endif
