/******************************************************
 * Filename: main.c									  *
 * Description: Main program to execute RTProc DSP,   *
 *				one session audio processing.		  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "uartIND.h"
#include "uartDispatcherMidi.h"
#include "uartDispatcherGeneric.h"
#include "uartReceiverMidi.h"
//#include "uartReceiverGeneric.h"
#include "IrtpAlgorithm.h"
#include "configureProcessingComponent.h"
#include "memory.h"
#include "accessSPORT.h"
#include "debugCode.h"

// Storage space to hold the name of the 
// algorithm component. Not used to 
// safe memory.
/*
struct
{
    char nameFX[STRING_LENGTH];
} RAM_DEFAULT_SECTION hostInfoControl;
*/

extern void InitPLL_SDRAM(void);
extern void InitSRU(void);
extern void Init1835viaSPI(void);

// Link to the algorithm class
IrtpAlgorithm* ptrTheClass = 0;

// Static variables to control the program flow in the main program
volatile bool RAM_DEFAULT_SECTION inProcessing = false;
volatile bool RAM_DEFAULT_SECTION hostOnline = true;
volatile bool RAM_DEFAULT_SECTION restartCommand = false;

// Instance of the alorithm. If this variable does not exist
// talkthrough is active
#ifdef USE_EFFECT
FX_TYPE_ALGORITHM RAM_DEFAULT_SECTION theClass;
#endif

// Object to access the SPORT for audio I/O,
// no specific member variables, only namespace reasons
accessSPORT RAM_DEFAULT_SECTION theAccessSportC;

#ifdef DEBUG_MATLAB

debugCode RAM_DEFAULT_SECTION theDebugCodeClass;
#endif

#ifdef USE_RS232

// UART object as the low level driver for RS232 communication
uartIND RAM_DEFAULT_SECTION theRS232ClassC;

// UART dispatcher to handle incoming and outgoing messages
#ifdef USE_UART_FOR_MIDI
	CuartDispatcherMidi RAM_DEFAULT_SECTION theDispatcherC;
#else
	CuartDispatcherGeneric RAM_DEFAULT_SECTION theDispatcherC;
#endif
#endif

/*==================================================
 * Main routine for audio processing, start audio
 * and set main thread to idle.
 *///===============================================
void main(void)
{
    //Initialize PLL to run at CCLK= 331.776 MHz & SDCLK= 165.888 MHz.
    //SDRAM is setup for use, but cannot be accessed until MSEN bit is enabled
    InitPLL_SDRAM();

	// Initialize the secondary heap
   initializeExtHeap();
	
#ifdef USE_EFFECT
    ptrTheClass = &theClass;
#endif

#ifdef USE_RS232
    theRS232ClassC.initializeRS232Link(uartIND::RS_232_MODE_DEFAULT, &theDispatcherC);
    theDispatcherC.initialize(&theRS232ClassC, &theClass);
	theClass.setBackwardReference(&theDispatcherC);
#endif

    if (ptrTheClass)
    {
    	//ptrTheClass->getNameAlgorithm(hostInfoControl.nameFX);
        ptrTheClass->activateAlgorithm(0);
    }

    // Need to initialize DAI because the sport signals need to be routed
    // before the AD1835 chip is initialized
    InitSRU();

    // This function will configure the codec on the kit
    Init1835viaSPI();

    // Endless procesing loop
    while (hostOnline)
    {
        restartCommand = false;

        // Wait until the fx object is ready
        if (ptrTheClass)
        {
            while (!ptrTheClass->isReadyForProcessingAlgorithm())
            {
                asm("nop;");
            }
        }

        // Start processing
        if (ptrTheClass)
        {
            ptrTheClass->prepareProcessingAlgorithm();
            ptrTheClass->startProcessingAlgorithm();
        }

        inProcessing = true;

        // Setup the sport to receive / transmit the audio I/O data 
        theAccessSportC.InitSPORT();

        // Be in infinite loop and do nothing until done.
        while (inProcessing)
        {
            asm("idle;");
        }
	
        // Set the SPORT into not running sate (no interrupts)
        theAccessSportC.termSPORT();

        if (ptrTheClass)
        {
            ptrTheClass->stopProcessingAlgorithm(1000000);
            ptrTheClass->prepareProcessingAlgorithm();
        }

        // Wait until restart is triggered
        while (!restartCommand)
            asm("nop;");
    }

}
