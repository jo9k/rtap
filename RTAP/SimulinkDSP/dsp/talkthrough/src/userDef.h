/******************************************************
 * Filename: algorithmIND_DSP.h						  *
 * Description: Base class to realize algorithm 	  *
 *              components. The most easy access to   *
 * 				new algorithms is to derive from this *
 * 				class.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 
 #ifndef _USERDEF_H_
#define _USERDEF_H_

/**************************************************************/
// Macro to define the length of all string fields
#define STRING_LENGTH 16 // <---- must be multiple of 4!
/**************************************************************/

#include "typedefs.h"

/*==============================================================
 * MACROS TO SETUP ALL PROCESSING PARAMATERS
 *============================================================*/
 
 
/**************************************************************/
// Macro to setup the buffersize
#define BUFFERSIZE 512
#define START_OFFSET 1500
/**************************************************************/

/**************************************************************/
// Macro to setup the samplerate
#define SAMPLERATE 48000
/**************************************************************/

/**************************************************************/
// Switch on /off channels 2 and 3. 
// Channel Selection: (frontal view)
//  O   O  O  O  O
//  O   O  O  O  O  o O O [--]
//               |-----------CHANNELSET1
//            |--------------CHANNELSET2
//         |-----------------CHANNELSET3
// The maximum of useable channels is 6 because we have only a
// maximum of 8 sample rate converters (SRC)
#define CHANNELSET2
#define CHANNELSET3

// Checking for the CHANNESLSET configuration
#ifndef CHANNELSET2
#ifdef CHANNELSET3
#error "Configuration error: CHANNELSET3 defined but CHANNELSET2 NOT DEFINED!"
#endif
#endif

/**************************************************************/

/**************************************************************/
// Select the processing format for the 
#define RTP_CHANNELFORMAT_FLOAT 					// float datatype
//#define RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT	// 24 Bit signed
//#define RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT	// 16 Bit signed
/**************************************************************/

/**************************************************************/
// If the user wants to debug his project, stop all interrupts during
// the actual algorithm.
//#define DEBUG_PROJECT
#define DEBUG_PROJECT_RS232

#define DEBUG_MATLAB
#define SWITCH_OFF_AUDIO_DEBUG_MODE
/**************************************************************/

/**************************************************************/
// Activate/deactivate the algorithm component in program.
#define USE_EFFECT

/**************************************************************/
// Activate/deactivate RS232 extension.
#define USE_RS232

/********************************************/
// Configure UART for MIDI or for generic
//#define USE_UART_FOR_MIDI
/********************************************/
#include "configureUart.h"

#define TIMER_MAT_DEBUG 990000000
//#define TIMER_MAT_DEBUG 99000
#define TIMER_MIDI 4000000
/********************************************/
// Maximum number of elements of load part in RS232 messages
#define MAX_NUMBER_ELEMENTS_LOAD_RS232 5
/********************************************/

/*==============================================================
 *============================================================*/

/**************************************************************/
// based on the processing format sertup the right datatype
#ifdef RTP_CHANNELFORMAT_FLOAT
#define DATATYPE float
#else

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
#define DATATYPE unsigned int
#else

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
#define DATATYPE unsigned short
#else
#error KEINDATAFORMATSPECIZIFIZIERT
#endif

#endif

#endif
/**************************************************************/

/**************************************************************/
// Determine the number of input and output channels
// Processing is operated with 6 or 4 channels
#define NUMBER_CHANNELS_DAC_ADC 2

#define NUMBER_CHANNELS_INPUT NUMBER_CHANNELS_DAC_ADC

#define NUMBER_CHANNELS_OUTPUT_1CHANNEL 2

#ifdef CHANNELSET2
#define NUMBER_CHANNELS_OUTPUT_2CHANNELS (NUMBER_CHANNELS_OUTPUT_1CHANNEL+2)
#else
#define NUMBER_CHANNELS_OUTPUT_2CHANNELS (NUMBER_CHANNELS_OUTPUT_1CHANNEL)
#endif

#ifdef CHANNELSET3
#define NUMBER_CHANNELS_OUTPUT (NUMBER_CHANNELS_OUTPUT_2CHANNELS+2)
#else
#define NUMBER_CHANNELS_OUTPUT NUMBER_CHANNELS_OUTPUT_2CHANNELS
#endif
/**************************************************************/

/**************************************************************/
// Macro to determine wether all RS232 traffic is stored in internal or
// SDRAM memory. If in SDRAM, the alternative heap is used for allocation.
#define USE_RS232_STATIC
/**************************************************************/

/**************************************************************/
// Macro to use extra buffers for input or the DMA directly. I have
// not notified any difference so far
#define SAFE_INPUT_BUFFERS
/**************************************************************/

/**************************************************************/
// Switch on/off audio (load) profiling
#define PROFILE_AUDIO
/**************************************************************/

#define SECTION_USER_BUFFERS section("seg_sdram")
#define SECTION_INPUT_COPY section("seg_sdram")
#define RAM_DEFAULT_SECTION section("seg_sdram")

#include <cdef21369.h>
#include <def21369.h>
#include <signal.h>

#include "IrtpAlgorithm.h"

#define NUM_SAMPLES NUMBER_CHANNELS_DAC_ADC*BUFFERSIZE

// Produce blind MAXLOAD
//#define CREATE_MAXLOAD
#define NOPSx10_MAXLOAD 150

// Function prototypes for this talkthrough code

extern void InitPLL_SDRAM(void);
extern void processBlock(unsigned int *);

extern void InitSRU(void);
extern void Init1835viaSPI(void);

extern void InitSPORT(void);
extern void initInterruptServices(void);
extern void TalkThroughISR0(int);
extern void TalkThroughISR2(int);
extern void ClearSPORT(void);

extern void SetupSPI1835 () ;
extern void DisableSPI1835 () ;
extern void Configure1835Register (int i) ;
extern unsigned int Get1835Register (int i) ;

extern void Delay (int i) ;

class IrtpAlgorithm;
extern IrtpAlgorithm* ptrTheClass;


#endif
