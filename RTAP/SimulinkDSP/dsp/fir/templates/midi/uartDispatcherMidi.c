/******************************************************
 * Filename: uartDispatcherMidi.c					  *
 * Description: Function to realize the RS232 		  *
 *              dispatcher for Midi. A midi message is*
 *              a concatenation of bytes. The 		  *
 *				dispatcher translates the incoming	  *
 *				bytes into a Midi message.			  *
 * 				Note that only the most important 	  *
 *				messages are currently supported. 	  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#include "configureUart.h"

#ifdef USE_UART_FOR_MIDI

#define USE_FLAGS_MIDI

#include <string.h>
#include <processor_include.h>

#ifdef USE_FLAGS_MIDI
#define SRUDEBUG  // Check SRU Routings for errors.
#include <SRU.h>
#include <signal.h>
#endif

#include "uartIND.h"
#include "uartDispatcherMidi.h"
#include "IuartApplicationMidi.h"
#include "userDef.h"


#ifdef USE_FLAGS_MIDI
void timerZero(int aigInt)
{
	asm("#include <def21369.h>");
	asm("bit clr flags FLG4;");
	timer_off();
}
#endif

/*==================================================
 * Constructor: Initialize all static variables
 *///===============================================
CuartDispatcherMidi::CuartDispatcherMidi()
{
    typeMidiMessage = STATUS_MIDI_NONE;

    targetChannel = -1;

    refHandleLink = NULL;
    refHandleApplication = NULL;

    byteCounterWord = 0;
    
#ifdef USE_FLAGS_MIDI
	asm("#include <def21369.h>");
	
	// Create link to control flags
    SRU(FLAG4_O,DPI_PB06_I);    //Drive DPI Pin 6 with Flag 4 (LED 1)

	//Enabling the Buffer direction for DAI and DPI pins using the following sequence:
	//High -> Output, Low -> Input
    SRU(HIGH,DPI_PBEN06_I);
 
    // Switch all (5) LEDs off.
    asm("bit set flags FLG4O;");
	asm("bit clr flags FLG4;");
 
    interrupt(SIG_TMZ, timerZero);
#endif
}

/*==================================================
 * Set all backward and forward links of the dispatcher
 *///===============================================
void
CuartDispatcherMidi::initialize(uartIND* handleLink,
                                IuartApplicationMidi* handleApplication)
{
	refHandleLink = handleLink;
    refHandleApplication = handleApplication;
}

/*==================================================
 * Callback for a new byte which has arrived. A state machine 
 * transforms the incoming byte into a midi message.
 *///===============================================
void
CuartDispatcherMidi::byteReceivedDefault(DATATYPE_RS232_CHAR byteMessage)
{
	// Ignore active sensing
    if (byteMessage == STATUS_BYTE_ACTIVE_SENSING) 
    	return;

    char messageByte;
    if (byteMessage & 0x80)
    {
        messageByte = (byteMessage & 0xF0);
        switch (messageByte)
        {
        case STATUS_BYTE_NOTE_ON:
            typeMidiMessage = STATUS_MIDI_NOTE_ON;
            targetChannel = byteMessage & 0xF;
            byteCounterWord = 0;
            break;
        case STATUS_BYTE_NOTE_OFF:
            typeMidiMessage = STATUS_MIDI_NOTE_OFF;
            targetChannel = byteMessage & 0xF;
            byteCounterWord = 0;
            break;
        case STATUS_BYTE_CONTROL_CHANGE:
            typeMidiMessage = STATUS_MIDI_CONTROL_CHANGE;
            targetChannel = byteMessage & 0xF;
            byteCounterWord = 0;
            break;
        case STATUS_BYTE_PROGRAM_CHANGE:
            typeMidiMessage = STATUS_MIDI_PROGRAM_CHANGE;
            targetChannel = byteMessage & 0xF;
            byteCounterWord = 0;
            break;
        case STATUS_BYTE_PITCH_BEND:
            typeMidiMessage = STATUS_MIDI_PITCH_BEND;
            targetChannel = byteMessage & 0xF;
            byteCounterWord = 0;
            break;
        default:
            // do nothing at the moment
            break;
        }
    }
    else
    {
    	//**************************************************
    	/* Switch case statement for different midi messages.*/
        switch (typeMidiMessage)
        {
        case STATUS_MIDI_NOTE_ON:
#ifdef USE_FLAGS_MIDI
			asm("bit set flags FLG4;");
	    	timer_set(TIMER_MIDI, TIMER_MIDI);
	    	timer_on();
#endif
            byteField[byteCounterWord++] = byteMessage;
            if (byteCounterWord == 2)
            {
                byteCounterWord = 0;
                if (refHandleApplication)
                {
                    // if a velocity of zero was sent, this
                    // indicates that the key was released
                    if (byteField[1] != 0)
                    {
                        refHandleApplication->reportNoteOn(byteField[0],
                                                           byteField[1], targetChannel);
                    }
                    else
                    {
                        refHandleApplication->reportNoteOff(byteField[0],
                                                            byteField[1], targetChannel);
                    }
                }
            }
            break;
        case STATUS_MIDI_NOTE_OFF:
            byteField[byteCounterWord++] = byteMessage;
            if (byteCounterWord == 2)
            {
                byteCounterWord = 0;
                if (refHandleApplication)
                    refHandleApplication->reportNoteOff(byteField[0],
                                                        byteField[1], targetChannel);
            }
            break;
        case STATUS_MIDI_CONTROL_CHANGE:
 
           byteField[byteCounterWord++] = byteMessage;
            if (byteCounterWord == 2)
            {
                // current value of actuator is in byteField[1]
                switch (byteField[0])
                {
                case MODULATION:
                    if (refHandleApplication)
                        refHandleApplication->reportModulation(byteField[1]);
                    byteCounterWord = 0;
                    break;
                case VOLUME:
                    if (refHandleApplication)
                        refHandleApplication->reportVolume(byteField[1]);
                    byteCounterWord = 0;
                    break;
                case SOUND_CONTROL_5:
                    if (refHandleApplication)
                        refHandleApplication->reportSoundControl5(byteField[1]);
                    byteCounterWord = 0;
                    break;
                case SOUND_CONTROL_10:
                    if (refHandleApplication)
                        refHandleApplication->reportSoundControl10(byteField[1]);
                    byteCounterWord = 0;
                    break;
                default:
                    // do nothing at the moment
                    byteCounterWord = 0;
                    break;
                }
            }
            break;
        case STATUS_MIDI_PROGRAM_CHANGE:
            // do nothing at the moment; byteMessage
            // contains the new program number
            byteCounterWord = 0;
            break;
        case STATUS_MIDI_PITCH_BEND:
            byteField[byteCounterWord++] = byteMessage;
            if (byteCounterWord == 2)
            {
                int valuePitch = byteField[0] + 128*byteField[1];
                byteCounterWord = 0;
                if (refHandleApplication)
                    refHandleApplication->reportPitchBend(valuePitch);
            }
            break;
        default:
            // do nothing at the moment
            break;
        }
    }
}

/*==================================================
 * Callback for a new streaming message. Streaming 
 * does not make sense for midi
 *///===============================================
void
CuartDispatcherMidi::messageReceivedStreaming(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING])
{
    if (refHandleApplication)
        refHandleApplication->reportError(ERR_ID_STREAMING_MODE);
}

/*==================================================
 * Callback for completion of message send. This does not make sense since
 * we accept only incoming messages at the moment.
 *///===============================================
void
CuartDispatcherMidi::messageSendComplete()
{
}

/*
bool
CuartDispatcherMidi::sendMessageNonBlocking(DATATYPE_RS232* ptrField,
        short numElementsToWrite, short& numElementsWritten, char purpose, int slotId)
{
    return(false);
}


// Function to send a message. The input data elements are
// send with a preceding header.
bool
CuartDispatcherMidi::sendMessageNonBlocking(char* ptrField,
        short numElementsToWrite, short& numElementsWritten, char purpose, int slotId)
{
    return(false);
}
*/
#endif
