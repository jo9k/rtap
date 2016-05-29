/******************************************************
 * Filename: algorithmINDMidi.c						  *
 * Description: Algorithm component with Midi support.*
 *				It is a good idea to add midi 		  *
 *				functionality at the very end of the  *
 * 				development and to work with generic  *
 *				RS232 messages so far.				  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #include "userDef.h"
#include "configureUart.h"

#ifdef USE_UART_FOR_MIDI

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "tt.h"
#include "algorithmIND_DSP.h"
#include "algorithmDSPMidi.h"
#include "tools.h"

/*==================================================
 * Constructor: TODO!
 *///===============================================
 CalgorithmDSPMidi::CalgorithmDSPMidi(): CalgorithmIND_DSP(), CuartMidiReceiver()
{
    currentValueKey = 69;
    currentValueVelocity = 0;
    currentChannel = 0;

    loudness = 1.0;
    velocity = 1.0;
    frequency = 440.0;
    pitchBend = 1.0;
    period = 512;
    iterator = 0;

    oq = 0.8; // open quotient
    sq = 0.7; // speed quotient
    nq = 0.0; // noise quotient

    recalcSignal = false;
    addNoise = false;
    depleteBuffer = false;
    noteOn = true;
}

/*==================================================
 * Callback to indicate that a new note has been triggered.
 *///===============================================
void CalgorithmDSPMidi::reportNoteOn(char valueKey, char valueVelocity, char channel)
{
    CuartMidiReceiver::reportNoteOn(valueKey, valueVelocity, channel);

    currentValueKey = valueKey;
    currentValueVelocity = valueVelocity;
    currentChannel = channel;

    noteOn = true;
    if (nq) addNoise = true;

    //calculateSignal(valueKey, valueVelocity, channel);
}

/*==================================================
 * Callback to indicate that a note was released.
 *///===============================================
void CalgorithmDSPMidi::reportNoteOff(char valueKey, char valueVelocity, char channel)
{
    CuartMidiReceiver::reportNoteOff(valueKey, valueVelocity, channel);

    if (valueKey == currentValueKey)
    {
        depleteBuffer = true;
        addNoise = false;
    }
}

/*==================================================
 * Callback to indicate that a new value of the modulation wheel was
 * set.
 *///===============================================
void CalgorithmDSPMidi::reportModulation(char valueModulation)
{
    CuartMidiReceiver::reportModulation(valueModulation);
    // valueModulation is out of [0, 127]
    // hence nq is out of [0, 1]
    nq = valueModulation*valueModulation/(127.0*127.0);
    if (nq) addNoise = true;
    if (noteOn) recalcSignal = true;
}

/*==================================================
 * Callback to indicate that a new volume was set.
 *///===============================================
void CalgorithmDSPMidi::reportVolume(char valueVolume)
{
    CuartMidiReceiver::reportVolume(valueVolume);
    loudness = (float)valueVolume*(1/128.0);
    if (noteOn) recalcSignal = true;
}


/*==================================================
 * TODO?
 *///===============================================
void CalgorithmDSPMidi::reportSoundControl5(char value)
{
    CuartMidiReceiver::reportSoundControl5(value);

    // calculate new open quotient
    oq = 0.1 + 0.8*(float)value*(1/128.0);

    if (noteOn) recalcSignal = true;
}

/*==================================================
 * TODO?
 *///===============================================
void CalgorithmDSPMidi::reportSoundControl10(char value)
{
    CuartMidiReceiver::reportSoundControl10(value);

    // calculate new speed quotient
    sq = 0.1 + 0.8*(float)value*(1/128.0);

    if (noteOn) recalcSignal = true;
}

/*==================================================
 * Function to indicate that a pitch bend event was initiated.
 *///===============================================
void CalgorithmDSPMidi::reportPitchBend(int valuePitch)
{
    CuartMidiReceiver::reportPitchBend(valuePitch);
    pitchBend = 1.0 + (valuePitch - 8192.0)/16383.0;
    if (noteOn) recalcSignal = true;
}

/*==================================================
 * Function where the uart dispatcher report an RS232 error.
 *///===============================================
void CalgorithmDSPMidi::reportError(short errorMessage)
{
    CuartMidiReceiver::reportError(errorMessage);
}

/*==================================================
 * Function to process input samples to output samples.
 * Here , pass control to the base class.
 *///===============================================
bool CalgorithmDSPMidi::process(DATATYPE outputBuffers[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE], DATATYPE inputBuffers[NUMBER_CHANNELS_INPUT][BUFFERSIZE])
{
    return(CalgorithmIND_DSP::process(outputBuffers, inputBuffers));
}

#endif
