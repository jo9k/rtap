/******************************************************
 * Filename: uartMidiReceiver.c						  *
 * Description: Default realization of the Midi 	  *
 *				receiver, currently no sign 		  *
 *				functionality except for note counting*
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #include "userDef.h"
#include "configureUart.h"
#include "uartReceiverMidi.h"

#ifdef USE_UART_FOR_MIDI

/*==================================================
 * Constructor: Set note counter to zero
 *///===============================================
CuartReceiverMidi::CuartReceiverMidi()
{
    cntNotes = 0;
    cntErrors = 0;
}

/*==================================================
 * Count notes
 *///===============================================
void
CuartReceiverMidi::reportNoteOn(char valueKey, char valueVelocity, char channel)
{
    cntNotes++;
}

/*==================================================
 * Count notes
 *///===============================================
void
CuartReceiverMidi::reportNoteOff(char valueKey, char valueVelocity, char channel)
{
    cntNotes++;
}

/*==================================================
 * 
 *///===============================================
void
CuartReceiverMidi::reportModulation(char valueModulation)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartReceiverMidi::reportVolume(char valueVolume)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartReceiverMidi::reportSoundControl5(char value)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartReceiverMidi::reportSoundControl10(char value)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartReceiverMidi::reportPitchBend(int valuePitch)
{
}

/*==================================================
 * Count errors.
 *///===============================================
void
CuartReceiverMidi::reportError(short errorMessage)
{
    cntErrors++;
}

#endif
