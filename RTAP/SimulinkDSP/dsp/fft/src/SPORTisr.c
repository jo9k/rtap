///////////////////////////////////////////////////////////////////////////////////////
//NAME:     SPORTisr.c (Block-based Talkthrough)
//DATE:     7/29/05
//PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1835.
//
//USAGE:    This file contains SPORT0 Interrupt Service Routine. Three buffers are used
//          for this example. One is filled by the ADC, another is sent to the DAC, and
//          the final buffer is processed. Each buffer rotates between these functions
//          upon each SP0 interrupt received.
///////////////////////////////////////////////////////////////////////////////////////
/*
   Here is the mapping between the SPORTS and the DACS
   ADC -> DSP  : SPORT0A : I2S
   DSP -> DAC1 : SPORT1A : I2S
   DSP -> DAC2 : SPORT1B : I2S
   DSP -> DAC3 : SPORT2A : I2S
   DSP -> DAC4 : SPORT2B : I2S
*/

#include <stdio.h>
#include <string.h>
#include "tt.h"
#include "tools.h"

//#define DAC12

extern DATATYPE copyDataInput[NUMBER_CHANNELS_INPUT][BUFFERSIZE];
extern DATATYPE copyDataOutput[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE];

struct
{
#ifdef PROFILE_DMA
    unsigned int dbgCntIRQ0;
    unsigned int dbgCntIRQ2;
    unsigned int delayDMA_Output;
    unsigned int loadCnt;
    float load;
    float ratio0;
    float ratio2;
    float ratio3;
#endif
} runtimeCtrlRT;

unsigned int* ptrChanInADC = NULL;
/**
 * Interrupt for DMA Channel 0A: Input. The pointer to the
 * input datafield that is NOT processed by DMA is stored here.
 * We can copy this field until the next buffer is available.
 * ====================================================
 * DMA handling is rather complex due to the async behavior
 * of input and the output DMAs.
 *///=====================================================

void TalkThroughISR0(int sig_int)
{
    // Store the pointer to the next data buffer that is available for processing
    unsigned int* addr0A = (unsigned int*)*pCPSP0A;
    ptrChanInADC = (unsigned int*)addr0A[1];

#ifdef PROFILE_DMA
    runtimeCtrlRT.dbgCntIRQ0++;

    runtimeCtrlRT.delayDMA_Output = *pCSP2B;

#endif
}

/**
 * Interrupt for DMA channel 2A and 2B.
 * At first, the input buffer is copied. This is necessary to
 * avoid that the asynch input overrides the input buffer while
 * in use. Note that all output buffers are managed in this IRQ since both
 * at least both output units are synchronous.
 * =======================================================
 * Next step is the callback for the algorithm
 *///====================================================
void TalkThroughISR2(int sig_int)
{
	return;
    int i;
    DATATYPE* ptrChanIn1;
    DATATYPE* ptrChanIn2;

#ifdef DAC12

    DATATYPE* ptrChanOut3;
    DATATYPE* ptrChanOut4;
    unsigned int* ptrChanOutDAC2;
#endif
    DATATYPE* ptrChanOut5;
    DATATYPE* ptrChanOut6;
    DATATYPE* ptrChanOut7;
    DATATYPE* ptrChanOut8;

    unsigned int* ptrChanOutDAC3;
    unsigned int* ptrChanOutDAC4;

    // Obtain data for algorithm profiling
#ifdef PROFILE_DMA
    runtimeCtrlRT.dbgCntIRQ2++;
    int counterDMA_2A_Enter = *pCSP2A;
    int counterDMA_2B_Enter = *pCSP2B;
    runtimeCtrlRT.ratio0 = (float)(*pSRCRAT0 & 0x7FFF);
    runtimeCtrlRT.ratio0 = runtimeCtrlRT.ratio0 *4.8828125e-4;
    runtimeCtrlRT.ratio2 = (float)(*pSRCRAT1 & 0x7FFF);
    runtimeCtrlRT.ratio2 = runtimeCtrlRT.ratio2 *4.8828125e-4;
    runtimeCtrlRT.ratio3 = (float)((*pSRCRAT1>>16) & 0x7FFF);
    runtimeCtrlRT.ratio3 = runtimeCtrlRT.ratio3 *4.8828125e-4;
#endif


    // Copy the data buffer, be sure that input buffer is not switched
    // in the meantime
    if (ptrChanInADC)
    {
        // Critical section, non-interruptible
        asm("#include <def21369.h>");
        asm("bit clr MODE1 IRPTEN;");
        //memcpy(copyDataInput, runtimeCtrlRT.bufIn0A, sizeof(copyDataInput));
        ptrChanIn1 = copyDataInput[0];
        ptrChanIn2 = copyDataInput[1];

        i = BUFFERSIZE;
        for (; i != 0; i--)
        {
#ifdef RTP_CHANNELFORMAT_FLOAT
            *ptrChanIn1++ = Int24ToFloat(*ptrChanInADC++);
            *ptrChanIn2++ = Int24ToFloat(*ptrChanInADC++);
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
            *ptrChanIn1++ = *ptrChanInADC++;
            *ptrChanIn2++ = *ptrChanInADC++;
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
            *ptrChanIn1++ = *ptrChanInADC++;
            *ptrChanIn2++ = *ptrChanInADC++;
#endif
        }

        // End of critical section
        asm("bit set MODE1 IRPTEN;");
    }

    if (ptrTheClass)
    {
#ifdef DEBUG_PROJECT
        asm("#include <def21369.h>");
        asm("bit clr MODE1 IRPTEN;");
#endif
        ptrTheClass->process(copyDataOutput, copyDataInput);
#ifdef DEBUG_PROJECT
        asm("bit set MODE1 IRPTEN;");
#endif
    }
    else
    {
#ifdef DAC12
        memcpy(copyDataOutput[0], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[1], copyDataInput[1], BUFFERSIZE);
        memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
        memcpy(copyDataOutput[4], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[5], copyDataInput[1], BUFFERSIZE);
#else
        memcpy(copyDataOutput[0], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[1], copyDataInput[1], BUFFERSIZE);
        memcpy(copyDataOutput[2], copyDataInput[0], BUFFERSIZE);
        memcpy(copyDataOutput[3], copyDataInput[1], BUFFERSIZE);
#endif
    }

    // Get the currently available data fields for output, the
    // other field in in use by DMA
#ifdef DAC12
    unsigned int* addr1B = (unsigned int*)*pCPSP1B;
    ptrChanOutDAC2 = (unsigned int*)addr1B[1];
    ptrChanOut3 = copyDataOutput[0];
    ptrChanOut4 = copyDataOutput[1];
    ptrChanOut5 = copyDataOutput[2];
    ptrChanOut6 = copyDataOutput[3];
    ptrChanOut7 = copyDataOutput[4];
    ptrChanOut8 = copyDataOutput[5];

#else
    ptrChanOut5 = copyDataOutput[0];
    ptrChanOut6 = copyDataOutput[1];
    ptrChanOut7 = copyDataOutput[2];
    ptrChanOut8 = copyDataOutput[3];
#endif

    unsigned int* addr2A = (unsigned int*)*pCPSP2A;
    unsigned int* addr2B = (unsigned int*)*pCPSP2B;
    ptrChanOutDAC3 = (unsigned int*)addr2A[1];
    ptrChanOutDAC4 = (unsigned int*)addr2B[1];

    i = BUFFERSIZE;
    for (; i != 0; i--)
    {
#ifdef RTP_CHANNELFORMAT_FLOAT
#ifdef DAC12
        *ptrChanOutDAC2++ = floatToInt24(*ptrChanOut3++);
        *ptrChanOutDAC2++ = floatToInt24(*ptrChanOut4++);
#endif
        *ptrChanOutDAC3++ = floatToInt24(*ptrChanOut5++);
        *ptrChanOutDAC3++ = floatToInt24(*ptrChanOut6++);
        *ptrChanOutDAC4++ = floatToInt24(*ptrChanOut7++);
        *ptrChanOutDAC4++ = floatToInt24(*ptrChanOut8++);
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
#ifdef DAC12
        *ptrChanOutDAC2++ = *ptrChanOut3++;
        *ptrChanOutDAC2++ = *ptrChanOut4++;
#endif
        *ptrChanOutDAC3++ = *ptrChanOut5++;
        *ptrChanOutDAC3++ = *ptrChanOut6++;
        *ptrChanOutDAC4++ = *ptrChanOut7++;
        *ptrChanOutDAC4++ = *ptrChanOut8++;
#endif
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
#ifdef DAC12
        *ptrChanOutDAC2++ = *ptrChanOut3++;
        *ptrChanOutDAC2++ = *ptrChanOut4++;
#endif
        *ptrChanOutDAC3++ = *ptrChanOut5++;
        *ptrChanOutDAC3++ = *ptrChanOut6++;
        *ptrChanOutDAC4++ = *ptrChanOut7++;
        *ptrChanOutDAC4++ = *ptrChanOut8++;
#endif
    }

#ifdef CREATE_MAXLOAD
    int ii;
    for (i = 0; i < NUM_SAMPLES; i++)
    {
        for (ii=380; ii > 0; --ii)
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

#ifdef PROFILE_DMA
    int counterDMA_2A_Leave = *pCSP2A;
    int counterDMA_2B_Leave = *pCSP2B;
    runtimeCtrlRT.loadCnt = counterDMA_2B_Enter - counterDMA_2B_Leave;
runtimeCtrlRT.load = runtimeCtrlRT.loadCnt*1.0/(2*BUFFERSIZE)*100.0;
#endif
}

/**
 * Prepare the interrupt service routines
 *///=================================================
void initInterruptServices()
{
#ifdef PROFILE_DMA
    runtimeCtrlRT.dbgCntIRQ0 = 0;
    runtimeCtrlRT.dbgCntIRQ2 = 0;
#endif
}


