/******************************************************
 * Filename: IuartApplicationMidi.h						  *
 * Description: uart receiver class to accept incoming*
 *			 	midi messages.						  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef __UARTMIDIAPPLICATION_H__
#define __UARTMIDIAPPLICATION_H__

#include "userDef.h"

#define ERR_ID_STREAMING_MODE 0

/*==================================================
 * Handle to provide callbacks for the dispatcher to
 * report incoming midi messages.
 * The application inherets these functions and can
 * process all incoming data.
 *///================================================
class IuartApplicationMidi
{
public:
    
	// Callbacks for incoming default messages (already parsed)
	
	// New note played
    virtual void reportNoteOn(char valueKey, char valueVelocity, char channel) = 0;

    // Stop note previously played
    virtual void reportNoteOff(char valueKey, char valueVelocity, char channel) = 0;

    // New value for Midi controller
    virtual void reportModulation(char valueModulation) = 0;

    // New value for Midi controller
    virtual void reportVolume(char valueVolume) = 0;

    // New value for Midi controller
    virtual void reportSoundControl5(char value) = 0;

    // New value for Midi controller
    virtual void reportSoundControl10(char value) = 0;

    // New value for Midi controller
    virtual void reportPitchBend(int valuePitch) = 0;

    // New value for Midi controller
    virtual void reportError(short ERROR_ID) = 0;
};

#endif

