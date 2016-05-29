/******************************************************
 * Filename: uartDispatcherMidi.h					  *
 * Description: Base class to realize the dispatcher  *
 * 				for incoming midi messages.			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"

#ifdef USE_UART_FOR_MIDI

#ifndef _UARTDISPATCHER_GENERIC_H__
#define _UARTDISPATCHER_GENERIC_H__

#include "uartIND.h"
#include "IuartDispatcher.h"

#define MAX_BYTES_MIDI_MESSAGE 2

class uartIND;
class IuartApplicationMidi;

/** Format:
 ****/


// Number of elements in buffer is identical to max number of elements minus sync word and data infos
#define MAX_NUMBER_ELEMENTS_DEFAULT_BUFFER (MAX_NUM_ELEMENTS_DISPATCHER-2)
#define MAX_NUMBER_BYTES_DEFAULT_BUFFER (MAX_NUM_BYTES_STREAMING-8)

/*==================================================
 * UART dispatcher for midi messages. The RS-232 link must be configured for
 * a data rate of 31250 BAUD, 8 Bits plus 2 STOPBITS, no parity
 * The hardware adaptor must be built according to the specification is
 *
 * http://www.cinetix.de/interface/tiptrix/midi2rs.htm
 *
 * There seem to be different possibkle types of communication, I will only test
 * the software against my Yamaha keyboard, YS200.
 *///====================================================================
class CuartDispatcherMidi:public IuartDispatcher
{
public:
	
	// Accepted Midi messages
    enum STATUS_BYTES
    {
        STATUS_BYTE_NOTE_OFF          = 0x80, // 8n <note> <velocity>, n channel number
        STATUS_BYTE_NOTE_ON           = 0x90, // 9n <note> <velocity>, n channel number
        STATUS_BYTE_CONTROL_CHANGE    = 0xB0, // Bn <controller> <value>, n channel number
        STATUS_BYTE_PROGRAM_CHANGE    = 0xC0, // Cn <value>, n channel number
        STATUS_BYTE_PITCH_BEND        = 0xE0, // E0 <value (lsb)> <value (msb)>
        STATUS_BYTE_ACTIVE_SENSING    = 0xFE
    };

	// Accepted Midi messages
    typedef enum
    {
        STATUS_MIDI_NONE,
        STATUS_MIDI_NOTE_ON,
        STATUS_MIDI_NOTE_OFF,
        STATUS_MIDI_CONTROL_CHANGE,
        STATUS_MIDI_PROGRAM_CHANGE,
        STATUS_MIDI_PITCH_BEND
    } midiStatusType;

    //! Accepted controllers
    enum CONTROLLERS
    {
        MODULATION                    = 0x01, // (KORG K25 MODULATION WHEEL)
        VOLUME                        = 0x07, // (KORG K25 SLIDER)
        SOUND_CONTROL_5               = 0x4A, // (KORG K25 KNOB1)
        SOUND_CONTROL_10              = 0x4F  // (KORG K25 KNOB2)

        // Annotation for the KORG K25: While pushing the button SW1,
        // the keyboard sends the previously sent program value
        // decremented by one. Similarly while pushing the button
        // SW2, the value is incremented by one.
    };

private:

    // State machine for input
    midiStatusType typeMidiMessage;

    // Counter of bytes to form a complete word
    WORD32 byteCounterWord;

    //! Midi channel that a message addresses
    WORD32 targetChannel;

    //! Backward reference (is this one used?)
    uartIND* refHandleLink;

    //! Link to application
    IuartApplicationMidi* refHandleApplication;

    //! Byte field for input data (characters are not available on the DSP)
    WORD32 byteField[MAX_BYTES_MIDI_MESSAGE];

public:

    // Constructor
    CuartDispatcherMidi();

    // API FUNCTIONS
    //! Initialize by setting the handles for the callbacks
    virtual void initialize(uartIND* handleLink, IuartApplicationMidi* handleApplication);

    //! Callback from uartIND to report another incoming byte
    virtual void byteReceivedDefault(DATATYPE_RS232_CHAR byteMess);

    //! Callback from uartIND to report a complete DMA message
    virtual void messageReceivedStreaming(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING]);

    //! Callback from uartIND to report that a message has been completely sent
    virtual void messageSendComplete();
};

#endif
#endif
