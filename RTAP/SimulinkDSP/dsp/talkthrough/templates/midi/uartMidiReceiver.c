/******************************************************
 * Filename: uartMidiReceiver.c						  *
 * Description: Default realization of the Midi 	  *
 *				receiver, currently no sign 		  *
 *				functionality except for note counting*
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #include "userDef.h"
#include "configureUart.h"
#include "uartMidiReceiver.h"

#ifdef USE_UART_FOR_MIDI

/*==================================================
 * Constructor: Set note counter to zero
 *///===============================================
CuartMidiReceiver::CuartMidiReceiver()
{
    cntNotes = 0;
    cntErrors = 0;
}

/*==================================================
 * Count notes
 *///===============================================
void
CuartMidiReceiver::reportNoteOn(char valueKey, char valueVelocity, char channel)
{
    cntNotes++;
}

/*==================================================
 * Count notes
 *///===============================================
void
CuartMidiReceiver::reportNoteOff(char valueKey, char valueVelocity, char channel)
{
    cntNotes++;
}

/*==================================================
 * 
 *///===============================================
void
CuartMidiReceiver::reportModulation(char valueModulation)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartMidiReceiver::reportVolume(char valueVolume)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartMidiReceiver::reportSoundControl5(char value)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartMidiReceiver::reportSoundControl10(char value)
{
}

/*==================================================
 * 
 *///===============================================
void
CuartMidiReceiver::reportPitchBend(int valuePitch)
{
}

/*==================================================
 * Count errors.
 *///===============================================
void
CuartMidiReceiver::reportError(short errorMessage)
{
    cntErrors++;
}

#endif
