/******************************************************
 * Filename: algorithmIND_DSP.h						  *
 * Description: Base class to realize algorithm 	  *
 *              components. The most easy access to   *
 * 				new algorithms is to derive from this *
 * 				class.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#ifdef USE_UART_FOR_MIDI

#ifndef __UARTMIDIRECEIVER_H__
#define __UARTMIDIRECEIVER_H__
#include "IuartMidiApplication.h"

/*****************************************
 * Class to implement the application side of the
 * RS232 communication. All base class (uartApplication)
 * callbacks have been implemented. The current
 * realization reacts on one message only be returning
 * a specific data field.
 *///==============================================
class CuartMidiReceiver: public IuartMidiApplication
{
    int cntNotes;
    int cntErrors;

public:

    // Constructor
    CuartMidiReceiver();

    virtual void reportNoteOn(char valueKey, char valueVelocity, char channel);

    virtual void reportNoteOff(char valueKey, char valueVelocity, char channel);

    virtual void reportModulation(char valueModulation);

    virtual void reportVolume(char valueVolume);

    virtual void reportSoundControl5(char value);

    virtual void reportSoundControl10(char value);

    virtual void reportPitchBend(int valuePitch);

    virtual void reportError(short errorCode);
};

#endif
#endif
