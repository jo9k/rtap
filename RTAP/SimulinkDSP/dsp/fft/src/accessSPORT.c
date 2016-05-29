/******************************************************
 * Filename: accessSPORT.c 							  *
 * Description: Component to handle all SPORT related *
 * 				aspects required for audio I/O.		  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include <cdef21369.h>
#include <def21369.h>
#include <signal.h>

#include "userDef.h"
#include "IrtpAlgorithm.h"
#include <string.h>
#include "tools.h"
#include "accessSPORT.h"

#define SRUDEBUG  // Check SRU Routings for errors.
#include <SRU.h>

#define M_PI 3.1415926
#include <math.h>

#define NUM_SAMPLES NUMBER_CHANNELS_DAC_ADC*BUFFERSIZE
#define PCI 0x00080000
#define OFFSET 0x00080000

// Interrupt service routines
void TalkThroughISR0(int);
void TalkThroughISR2(int);
void Delay (int i) ;

extern IrtpAlgorithm* ptrTheClass;

#ifdef DEBUG_MATLAB
#include "debugCode.h"
extern debugCode theDebugCodeClass;
#endif

//===========================================
// Macro to use minimal (memcpy) talkthrough instead of all
// RTProc DSP overhead
//#define MINIMAL_TALKTHROUGH

//===========================================
// Macro to activate the flags to show overload, 50% and 99.5% load
// This option is only valid if PROFILE_AUDIO is defined
#define USE_FLAGS_AUDIO

// declare ISR for software interrupt
extern void interruptProcessSamples(int sigInt);

// Buffers for user processing, global static
DATATYPE SECTION_USER_BUFFERS inputBuffer0[BUFFERSIZE];
DATATYPE SECTION_USER_BUFFERS inputBuffer1[BUFFERSIZE];

#ifdef CHANNELSET2
#ifdef CHANNELSET3
DATATYPE SECTION_USER_BUFFERS outputBuffer5[BUFFERSIZE];
DATATYPE SECTION_USER_BUFFERS outputBuffer6[BUFFERSIZE];
#endif
DATATYPE SECTION_USER_BUFFERS outputBuffer3[BUFFERSIZE];
DATATYPE SECTION_USER_BUFFERS outputBuffer4[BUFFERSIZE];
#endif
DATATYPE SECTION_USER_BUFFERS outputBuffer0[BUFFERSIZE];
DATATYPE SECTION_USER_BUFFERS outputBuffer1[BUFFERSIZE];

DATATYPE SECTION_USER_BUFFERS *copyDataInput[NUMBER_CHANNELS_INPUT] =
{
	inputBuffer0,
	inputBuffer1
};

DATATYPE SECTION_USER_BUFFERS *copyDataOutput[NUMBER_CHANNELS_OUTPUT] =
{
	outputBuffer0,outputBuffer1
#ifdef CHANNELSET2
	,outputBuffer3,outputBuffer4
#ifdef CHANNELSET3
	,outputBuffer5, outputBuffer6
#endif
#endif
};

// Double buffers for copy of input samples. i do not know wether this is useful or not
#ifdef SAFE_INPUT_BUFFERS
WORD32 SECTION_INPUT_COPY inputCopy[2*NUM_SAMPLES];
#endif

//================================================================
// LOCATE IN INTERNAL MEMORY
//================================================================

// Some variables, access required in a fast way
#ifdef SAFE_INPUT_BUFFERS
static int toggleInt = 0;
static int toggleIntInv = 0;
#endif
static int flagProcessing = 0;

//=============================================
// Shortcuts to DMA buffer fragments, input and output
WORD32* ptrChanInADC;
#ifdef CHANNELSET3
WORD32* ptrChanOutDAC2;
#endif
#ifdef CHANNELSET2
WORD32* ptrChanOutDAC3;
#endif
WORD32* ptrChanOutDAC4;

//=============================================
// DMA control buffers
// All these buffers must be located in dmda memory
int TCB_Block_DMA_0A0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_0A1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
#ifdef CHANNELSET3
int TCB_Block_DMA_1B0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_1B1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
#endif
#ifdef CHANNELSET2
int TCB_Block_DMA_2A0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_2A1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
#endif

int TCB_Block_DMA_2B0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_2B1[5] = { 0, NUM_SAMPLES, 1, 0, 0};

//=============================================
// Hardware buffers for DMA
unsigned int Block_In_DMA0A[NUM_SAMPLES*2] ;
#ifdef CHANNELSET3
unsigned int Block_Out_DMA1B[NUM_SAMPLES*2] ;
#endif

#ifdef CHANNELSET2
unsigned int Block_Out_DMA2A[NUM_SAMPLES*2] ;
#endif

unsigned int Block_Out_DMA2B[NUM_SAMPLES*2] ;
//================================================================
//================================================================

// Link to class pointer for global ISRs
accessSPORT RAM_DEFAULT_SECTION* theSPORTClass;

/*==================================================
 * Initialize SPORTs for audio I/O
 *///===============================================
 void 
accessSPORT::InitSPORT()
{
	theSPORTClass = this;
	
	// Start software interrupt for audio processing
    interrupt(SIG_USR1, interruptProcessSamples);
    
#ifdef USE_FLAGS_AUDIO
    SRU(FLAG6_O,DPI_PB08_I);    //Drive DPI Pin 8 with Flag 6 (LED 3)
    SRU(FLAG7_O,DPI_PB13_I);    //Drive DPI Pin 13 with Flag 7 (LED 4)
    SRU(FLAG8_O,DPI_PB14_I);    //Drive DPI Pin 14 with Flag 8 (LED 5)
    SRU(HIGH,DPI_PBEN08_I);
    SRU(HIGH,DPI_PBEN13_I);
    SRU(HIGH,DPI_PBEN14_I);
    
        // Switch all (5) LEDs off.
    asm("bit set flags FLG6O|FLG7O|FLG8O;");
	asm("bit clr flags FLG6|FLG7|FLG8;");

#endif

	ptrChanInADC = NULL;
#ifdef CHANNELSET3
	ptrChanOutDAC2 = NULL;
#endif

#ifdef CHANNELSET2
	ptrChanOutDAC3 = NULL;
#endif

	ptrChanOutDAC4 = NULL;

	// Set all buffers to zero
    memset(inputBuffer0, 0, sizeof(inputBuffer0));
    memset(inputBuffer1, 0, sizeof(inputBuffer1));
    
    memset(Block_In_DMA0A, 0, sizeof(Block_In_DMA0A));
#ifdef CHANNELSET3
    memset(Block_Out_DMA1B, 0, sizeof(Block_Out_DMA1B));
#endif

#ifdef CHANNELSET2
    memset(Block_Out_DMA2A, 0, sizeof(Block_Out_DMA2A));
#endif

    memset(Block_Out_DMA2B, 0, sizeof(Block_Out_DMA2B));

    TCB_Block_DMA_0A0[0] = (int) TCB_Block_DMA_0A1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_0A0[3] = (unsigned int) Block_In_DMA0A - OFFSET ;
    TCB_Block_DMA_0A0[4] = (unsigned int) Block_In_DMA0A;
    TCB_Block_DMA_0A1[0] = (int) TCB_Block_DMA_0A0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_0A1[3] = (unsigned int) Block_In_DMA0A - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_0A1[4] = (unsigned int) Block_In_DMA0A + NUM_SAMPLES;

#ifdef CHANNELSET3
    TCB_Block_DMA_1B0[0] = (int) TCB_Block_DMA_1B1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_1B0[3] = (unsigned int) Block_Out_DMA1B - OFFSET ;
    TCB_Block_DMA_1B0[4] = (unsigned int) Block_Out_DMA1B;
    TCB_Block_DMA_1B1[0] = (int) TCB_Block_DMA_1B0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_1B1[3] = (unsigned int) Block_Out_DMA1B - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_1B1[4] = (unsigned int) Block_Out_DMA1B + NUM_SAMPLES;
#endif

#ifdef CHANNELSET2
    TCB_Block_DMA_2A0[0] = (int) TCB_Block_DMA_2A1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2A0[3] = (unsigned int) Block_Out_DMA2A - OFFSET ;
    TCB_Block_DMA_2A0[4] = (unsigned int) Block_Out_DMA2A;
    TCB_Block_DMA_2A1[0] = (int) TCB_Block_DMA_2A0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2A1[3] = (unsigned int) Block_Out_DMA2A - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_2A1[4] = (unsigned int) Block_Out_DMA2A + NUM_SAMPLES;
#endif

    TCB_Block_DMA_2B0[0] = (int) TCB_Block_DMA_2B1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2B0[3] = (unsigned int) Block_Out_DMA2B - OFFSET ;
    TCB_Block_DMA_2B0[4] = (unsigned int) Block_Out_DMA2B;
    TCB_Block_DMA_2B1[0] = (int) TCB_Block_DMA_2B0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2B1[3] = (unsigned int) Block_Out_DMA2B - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_2B1[4] = (unsigned int) Block_Out_DMA2B + NUM_SAMPLES;

    //Clear the Mutlichannel control registers
    *pSPMCTL0 = 0;
    *pSPMCTL1 = 0;
    *pSPMCTL2 = 0;
    *pSPCTL0 = 0 ;
    *pSPCTL1 = 0 ;
    *pSPCTL2 = 0 ;

    *pDIV0 = 0x00180000;
    *pDIV0 |= 24756000/(8*SAMPLERATE*2);// 8 kHz: 192; 48 kHz: 32
#ifdef CHANNELSET3
    *pDIV1 = 0x00180000;
    *pDIV1 |= 24756000/(8*SAMPLERATE*2);
#endif
    *pDIV2 = 0x00180000;
    *pDIV2 |= 24756000/(8*SAMPLERATE*2);

    *pSRCCTL0 = SRC0_SMODEIN0 | SRC0_SMODEOUT0 | SRC0_ENABLE;
#ifdef CHANNELSET3
    *pSRCCTL0 |= SRC1_SMODEIN0 | SRC1_SMODEOUT0 | SRC1_ENABLE;
#endif
#ifdef CHANNELSET2
    *pSRCCTL1 = SRC2_SMODEIN0 | SRC2_SMODEOUT0 | SRC2_ENABLE;
#endif
    *pSRCCTL1 |= SRC3_SMODEIN0 | SRC3_SMODEOUT0 | SRC3_ENABLE;

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL0 = (OPMODE | ICLK | SLEN16 | SPEN_A | SCHEN_A | SDEN_A);
#else
    *pSPCTL0 = (OPMODE | ICLK | SLEN24 | SPEN_A | SCHEN_A | SDEN_A);
#endif
#ifdef CHANNELSET3
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL1 = (SPTRAN | ICLK | OPMODE | SLEN16 | SPEN_B | SCHEN_B | SDEN_B)  ;
#else
    *pSPCTL1 = (SPTRAN | ICLK | OPMODE | SLEN24 | SPEN_B | SCHEN_B | SDEN_B)  ;
#endif
#endif

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL2 = (SPTRAN | ICLK |OPMODE | SLEN16)  ;
#else
    *pSPCTL2 = (SPTRAN | ICLK |OPMODE | SLEN24)  ;
#endif

#ifdef CHANNELSET2
    *pSPCTL2 |= (SPEN_A | SCHEN_A | SDEN_A )  ;
#endif
    *pSPCTL2 |= (SPEN_B | SCHEN_B | SDEN_B)  ;

    // Here we start the input
    *pCPSP0A = (unsigned int) TCB_Block_DMA_0A0 - OFFSET + 3;

    Delay(START_OFFSET);

	// Here we start the output
#ifdef CHANNELSET3
    *pCPSP1B = (unsigned int) TCB_Block_DMA_1B0 - OFFSET + 3;
#endif
#ifdef CHANNELSET2
    *pCPSP2A = (unsigned int) TCB_Block_DMA_2A0 - OFFSET + 3;
#endif
    *pCPSP2B = (unsigned int) TCB_Block_DMA_2B0 - OFFSET + 3;

    // Install the interrupt handler for buffer switches
    interrupt (SIG_SP0,TalkThroughISR0);
    interrupt (SIG_SP2,TalkThroughISR2);
}

/*==================================================
 * Termination function to deactivate ISRs
 *///===============================================
void 
accessSPORT::termSPORT()
{
	interrupt (SIG_SP0,SIG_DFL);
    interrupt (SIG_SP2,SIG_DFL);
}

/*==================================================
 * Termination function to deactivate ISRs
 * Interrupt service routine for input samples. In this routine all 
 * input samples are copied into a temoryry buffer. Sources and 
 * destinations are double buffered.
 *///===============================================
void TalkThroughISR0(int sig_int)
{
    // Store the pointer to the next data buffer that is available for processing
    unsigned int* addr0A = (unsigned int*)*pCPSP0A;
    ptrChanInADC = (WORD32*)addr0A[1];

#ifdef PROFILE_AUDIO
	theSPORTClass->cntSamplesIRQInput_in = *pCSP0A;
	theSPORTClass->cntSamplesIRQInput_out = *pCSP2A;
#endif
	
#ifdef SAFE_INPUT_BUFFERS
    memcpy(&inputCopy[NUM_SAMPLES*toggleInt], 
    	    ptrChanInADC, NUM_SAMPLES);
    toggleIntInv = toggleInt;
	toggleInt = (toggleInt+1)%2;
#endif

}

/*==================================================
 * Interrupt service routine for output samples. In this routine all 
 * pointers are set to the output buffers and the software interrupt 
 * is triggered. This routine must be very quick in order to not 
 * interfere with the RS232 stuff (ISR with very high priority).
 *///===============================================
void TalkThroughISR2(int sig_int)
{
	WORD32 i;
	
#ifdef CHANNELSET3
    unsigned int* addr1B = (unsigned int*)*pCPSP1B;
    ptrChanOutDAC2 = (WORD32*)addr1B[1];
#endif
#ifdef CHANNELSET2
    unsigned int* addr2A = (unsigned int*)*pCPSP2A;
    ptrChanOutDAC3 = (WORD32*)addr2A[1];
#endif

    unsigned int* addr2B = (unsigned int*)*pCPSP2B;
    ptrChanOutDAC4 = (WORD32*)addr2B[1];

#ifdef PROFILE_AUDIO
	theSPORTClass->cntSamplesIRQOutput_in = *pCSP0A;
	theSPORTClass->cntSamplesIRQOutput_out = *pCSP2B;
#endif

	if(flagProcessing)
	{
		// Blink if overload
#ifdef USE_FLAGS_AUDIO
		asm("#include <def21369.h>");
		asm("bit set flags FLG6;");
#endif
		
	}
#ifdef USE_FLAGS_AUDIO
	else
	{
		asm("bit clr flags FLG6;");
	}
#endif     
	// Start the software interrupt right here
	asm("#include <def21369.h>");
	asm("bit set IRPTL SFT1I;");
}

/*==================================================
 * Interrupt service routine for audio processing. This routine is 
 * triggered as soon as the output buffers have been used. From now on,
 * the processing must complete until the next output buffer is processed.
 *///===============================================
void interruptProcessSamples(int sigInt)
{
#ifndef MINIMAL_TALKTHROUGH
    int i,ii;
    DATATYPE* ptrChanIn1;
    DATATYPE* ptrChanIn2;
#ifdef CHANNELSET3
    DATATYPE* ptrChanOut3;
    DATATYPE* ptrChanOut4;
#endif
#ifdef CHANNELSET2
    DATATYPE* ptrChanOut5;
    DATATYPE* ptrChanOut6;
#endif
    DATATYPE* ptrChanOut7;
    DATATYPE* ptrChanOut8;
    
#ifdef SAFE_INPUT_BUFFERS
    WORD32* ptrChanInADC = &inputCopy[NUM_SAMPLES*toggleIntInv];
#else
    WORD32* ptrChanInADC = theSPORTClass->ptrChanInADC;
#endif

	// Set global flag to indicate processing is active
    flagProcessing = 1;

#ifdef PROFILE_AUDIO
	theSPORTClass->cntSamplesSFTI_in_enter = *pCSP0A;
	theSPORTClass->cntSamplesSFTI_out_enter =  *pCSP2B;
#endif
    
	// The pointers to user input buffers    
   	ptrChanIn1 = copyDataInput[0];
   	ptrChanIn2 = copyDataInput[1];

   	// Prepare input buffers
#ifndef SAFE_INPUT_BUFFERS
	if(ptrChanInADC)
#endif
	{
#ifdef RTP_CHANNELFORMAT_FLOAT
   		for (i = BUFFERSIZE; i != 0; i--)
   		{
			*ptrChanIn1++ = Int24ToFloat(*ptrChanInADC++);
			*ptrChanIn2++ = Int24ToFloat(*ptrChanInADC++);
   		}
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
   		for (i = BUFFERSIZE; i != 0; i--)
   		{
        	*ptrChanIn1++ = *ptrChanInADC++;
        	*ptrChanIn2++ = *ptrChanInADC++;
   		}
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
   		for (i = BUFFERSIZE; i != 0; i--)
   		{
        	*ptrChanIn1++ = *ptrChanInADC++;
        	*ptrChanIn2++ = *ptrChanInADC++;
   		}
#endif
	}

//	ptrTheClass = 0;
#ifdef DEBUG_MATLAB
#ifdef SWITCH_OFF_AUDIO_DEBUG_MODE
	if(theDebugCodeClass.debugActive)
#else
	if(0)
#endif
	{
    	memset(copyDataOutput[0], 0, BUFFERSIZE);
		memset(copyDataOutput[1], 0, BUFFERSIZE);

#ifdef CHANNELSET2
        memset(copyDataOutput[2], 0, BUFFERSIZE);
        memset(copyDataOutput[3], 0, BUFFERSIZE);
#ifdef CHANNELSET3
        memset(copyDataOutput[4], 0, BUFFERSIZE);
        memset(copyDataOutput[5], 0, BUFFERSIZE);
#endif        
#endif
	}
	else
	{
		if(ptrTheClass)
		{
#ifdef DEBUG_PROJECT
        	asm("#include <def21369.h>");
        	asm("bit clr MODE1 IRPTEN;");
#endif

#ifdef CHANNELSET2
#ifdef CHANNELSET3
        	ptrTheClass->process(copyDataOutput, copyDataInput, 6, 2, BUFFERSIZE);
#else
        	ptrTheClass->process(copyDataOutput, copyDataInput, 4, 2, BUFFERSIZE);
#endif
#else
        	ptrTheClass->process(copyDataOutput, copyDataInput, 2, 2, BUFFERSIZE);
#endif


#ifdef DEBUG_PROJECT
        	asm("bit set MODE1 IRPTEN;");
#endif
    	}
    	else
    	{
	        memcpy(copyDataOutput[0], copyDataInput[0], BUFFERSIZE);
        	memcpy(copyDataOutput[1], copyDataInput[1], BUFFERSIZE);

#ifdef CHANNELSET2
        	memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        	memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
#endif
#ifdef CHANNELSET3
#ifdef CHANNELSET2
        	memcpy(copyDataOutput[4], copyDataInput[0], BUFFERSIZE);
        	memcpy(copyDataOutput[5], copyDataInput[1], BUFFERSIZE);
#else
        	memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        	memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
#endif        
#endif
    	}
    }

#else
	// Call process function in algorithm class here
	if (ptrTheClass)
	{
#ifdef DEBUG_PROJECT
        asm("#include <def21369.h>");
        asm("bit clr MODE1 IRPTEN;");
#endif
#ifdef CHANNELSET2
#ifdef CHANNELSET3
        	ptrTheClass->process(copyDataOutput, copyDataInput, 6, 2, BUFFERSIZE);
#else
        	ptrTheClass->process(copyDataOutput, copyDataInput, 4, 2, BUFFERSIZE);
#endif
#else
        	ptrTheClass->process(copyDataOutput, copyDataInput, 2, 2, BUFFERSIZE);
#endif
#ifdef DEBUG_PROJECT
        asm("bit set MODE1 IRPTEN;");
#endif
    }
    else
    {
        memcpy(copyDataOutput[0], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[1], copyDataInput[1], BUFFERSIZE);

#ifdef CHANNELSET2
        memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
#endif
#ifdef CHANNELSET3
#ifdef CHANNELSET2
        memcpy(copyDataOutput[4], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[5], copyDataInput[1], BUFFERSIZE);
#else
        memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
#endif        
#endif
    }
#endif // DEBUG_MATLAB
    // Get the currently available data fields for output, the
    // other field in in use by DMA
    ptrChanOut7 = copyDataOutput[0];
    ptrChanOut8 = copyDataOutput[1];
#ifdef CHANNELSET2
    ptrChanOut5 = copyDataOutput[2];
    ptrChanOut6 = copyDataOutput[3];
#endif
#ifdef CHANNELSET3
#ifdef CHANNELSET2
    ptrChanOut3 = copyDataOutput[4];
    ptrChanOut4 = copyDataOutput[5];
#else
    ptrChanOut3 = copyDataOutput[2];
    ptrChanOut4 = copyDataOutput[3];
#endif
#endif
	// Copy the output to the hardware DMA buffers
	if(ptrChanOutDAC4)
    {
	    i = BUFFERSIZE;
	    for (; i != 0; i--)
	    {
#ifdef RTP_CHANNELFORMAT_FLOAT
        	*ptrChanOutDAC4++ = floatToInt24(*ptrChanOut7++);
        	*ptrChanOutDAC4++ = floatToInt24(*ptrChanOut8++);
#ifdef CHANNELSET2
        	*ptrChanOutDAC3++ = floatToInt24(*ptrChanOut5++);
        	*ptrChanOutDAC3++ = floatToInt24(*ptrChanOut6++);
#endif
#ifdef CHANNELSET3
        	*ptrChanOutDAC2++ = floatToInt24(*ptrChanOut3++);
        	*ptrChanOutDAC2++ = floatToInt24(*ptrChanOut4++);
#endif
#endif

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
        	*ptrChanOutDAC4++ = *ptrChanOut7++;
        	*ptrChanOutDAC4++ = *ptrChanOut8++;
#ifdef CHANNELSET2
        	*ptrChanOutDAC3++ = *ptrChanOut5++;
        	*ptrChanOutDAC3++ = *ptrChanOut6++;
#endif
#ifdef CHANNELSET3
        	*ptrChanOutDAC2++ = *ptrChanOut3++;
        	*ptrChanOutDAC2++ = *ptrChanOut4++;
#endif
#endif

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
        	*ptrChanOutDAC4++ = *ptrChanOut7++;
        	*ptrChanOutDAC4++ = *ptrChanOut8++;
#ifdef CHANNELSET2
        	*ptrChanOutDAC3++ = *ptrChanOut5++;
        	*ptrChanOutDAC3++ = *ptrChanOut6++;
#endif
#ifdef CHANNELSET3
        	*ptrChanOutDAC2++ = *ptrChanOut3++;
        	*ptrChanOutDAC2++ = *ptrChanOut4++;
#endif
#endif
    	}
    }


    // TEst performance bounds
#ifdef CREATE_MAXLOAD
    for (i = 0; i < NUM_SAMPLES; i++)
    {
        for (ii = NOPSx10_MAXLOAD; ii > 0; --ii)
        {
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
        }
    }
#endif

#else //MINIMAL_TALKTHROUGH
	
#ifdef PROFILE_AUDIO
	theSPORTClass->cntSamplesSFTI_in_enter = *pCSP0A;
	theSPORTClass->cntSamplesSFTI_out_enter =  *pCSP2B;
#endif
   
/*
	int i,ii;
	for (i = 0; i < NUM_SAMPLES; i++)
    {
        for (ii=150; ii > 0; --ii)
        {
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
            asm("nop;");
        }
    }
*/    
#ifdef SAFE_INPUT_BUFFERS
    WORD32* ptrChanInADC = &inputCopy[NUM_SAMPLES*toggleIntInv];
#else
    WORD32* ptrChanInADC = theSPORTClass->ptrChanInADC;
#endif
	if (ptrChanOutDAC4)
    {
#ifdef CHANNELSET3
	   memcpy(ptrChanOutDAC2, ptrChanInADC, NUM_SAMPLES);
#endif
#ifdef CHANNELSET2
	   memcpy(ptrChanOutDAC3, ptrChanInADC, NUM_SAMPLES);
#endif
	   memcpy(ptrChanOutDAC4, ptrChanInADC, NUM_SAMPLES);
    }
    #endif
  	
#ifdef PROFILE_AUDIO
    theSPORTClass->cntSamplesSFTI_in_leave = *pCSP0A;
	theSPORTClass->cntSamplesSFTI_out_leave = *pCSP2B;
	theSPORTClass->load = (theSPORTClass->cntSamplesSFTI_out_enter-theSPORTClass->cntSamplesSFTI_out_leave)/(2.0*BUFFERSIZE);
#ifdef USE_FLAGS_AUDIO
	asm("#include <def21369.h>");
	if(theSPORTClass->load > 0.5)
		asm("bit set flags FLG8;");
	else
		asm("bit clr flags FLG8;");
	if(theSPORTClass->load > 0.9)
		asm("bit set flags FLG7;");
	else
		asm("bit clr flags FLG7;");
#endif
		
#endif	
    // Set flag to indicate that processing is over
    flagProcessing = 0;
}

