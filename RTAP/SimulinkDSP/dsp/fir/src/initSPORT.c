///////////////////////////////////////////////////////////////////////////////////////
//NAME:     initSPORT.c (Block-based Talkthrough)
//DATE:     7/29/05
//PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1835.
//
//USAGE:    This file uses SPORT0 to receive data from the ADC and transmits the
//			data to the DAC's via SPORT1A, SPORT1B, SPORT2A and SPORT2B.
//			DMA Chaining is enabled
//
////////////////////////////////////////////////////////////////////////////////////////

#include "tt.h"
#include <string.h>

#define M_PI 3.1415926
#include <math.h>

/*
   Here is the mapping between the SPORTS and the DACS
   ADC -> DSP  : SPORT0A : I2S
   DSP -> DAC1 : SPORT1A : I2S
   DSP -> DAC2 : SPORT1B : I2S
   DSP -> DAC3 : SPORT2A : I2S
   DSP -> DAC4 : SPORT2B : I2S
*/

#define PCI 0x00080000
#define OFFSET 0x00080000

unsigned int Block_In_DMA0A[NUM_SAMPLES*2] ;
#ifdef DAC12
unsigned int Block_Out_DMA1B[NUM_SAMPLES*2] ;
#endif

unsigned int Block_Out_DMA2A[NUM_SAMPLES*2] ;
unsigned int Block_Out_DMA2B[NUM_SAMPLES*2] ;


DATATYPE copyDataInput[NUMBER_CHANNELS_INPUT][BUFFERSIZE];
DATATYPE copyDataOutput[NUMBER_CHANNELS_OUTPUT][BUFFERSIZE];

int TCB_Block_DMA_0A0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_0A1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
#ifdef DAC12
int TCB_Block_DMA_1B0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_1B1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
#endif

int TCB_Block_DMA_2A0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_2A1[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_2B0[5] = { 0, NUM_SAMPLES, 1, 0, 0};
int TCB_Block_DMA_2B1[5] = { 0, NUM_SAMPLES, 1, 0, 0};

void InitSPORT()
{
    // Set all buffers to zero
    memset(copyDataInput, 0, sizeof(copyDataInput));
    memset(Block_In_DMA0A, 0, sizeof(Block_In_DMA0A));
#ifdef DAC12
    memset(Block_Out_DMA1B, 0, sizeof(Block_Out_DMA1B));
#endif

    memset(Block_Out_DMA2A, 0, sizeof(Block_Out_DMA2A));
    memset(Block_Out_DMA2B, 0, sizeof(Block_Out_DMA2B));

    TCB_Block_DMA_0A0[0] = (int) TCB_Block_DMA_0A1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_0A0[3] = (unsigned int) Block_In_DMA0A - OFFSET ;
    TCB_Block_DMA_0A0[4] = (unsigned int) Block_In_DMA0A;
    TCB_Block_DMA_0A1[0] = (int) TCB_Block_DMA_0A0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_0A1[3] = (unsigned int) Block_In_DMA0A - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_0A1[4] = (unsigned int) Block_In_DMA0A + NUM_SAMPLES;

#ifdef DAC12
    TCB_Block_DMA_1B0[0] = (int) TCB_Block_DMA_1B1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_1B0[3] = (unsigned int) Block_Out_DMA1B - OFFSET ;
    TCB_Block_DMA_1B0[4] = (unsigned int) Block_Out_DMA1B;
    TCB_Block_DMA_1B1[0] = (int) TCB_Block_DMA_1B0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_1B1[3] = (unsigned int) Block_Out_DMA1B - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_1B1[4] = (unsigned int) Block_Out_DMA1B + NUM_SAMPLES;
#endif

    TCB_Block_DMA_2A0[0] = (int) TCB_Block_DMA_2A1 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2A0[3] = (unsigned int) Block_Out_DMA2A - OFFSET ;
    TCB_Block_DMA_2A0[4] = (unsigned int) Block_Out_DMA2A;
    TCB_Block_DMA_2A1[0] = (int) TCB_Block_DMA_2A0 + 3 - OFFSET + PCI ;
    TCB_Block_DMA_2A1[3] = (unsigned int) Block_Out_DMA2A - OFFSET + NUM_SAMPLES;
    TCB_Block_DMA_2A1[4] = (unsigned int) Block_Out_DMA2A + NUM_SAMPLES;

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
#ifdef DAC12
    *pDIV1 = 0x00180000;
    *pDIV1 |= 24756000/(8*SAMPLERATE*2);
#endif
    *pDIV2 = 0x00180000;
    *pDIV2 |= 24756000/(8*SAMPLERATE*2);
    *pSRCCTL0 = SRC0_SMODEIN0 | SRC0_SMODEOUT0 | SRC0_ENABLE;
#ifdef DAC12
    *pSRCCTL0 |= SRC1_SMODEIN0 | SRC1_SMODEOUT0 | SRC1_ENABLE;
#endif
    *pSRCCTL1 = SRC2_SMODEIN0 | SRC2_SMODEOUT0 | SRC2_ENABLE;
    *pSRCCTL1 |= SRC3_SMODEIN0 | SRC3_SMODEOUT0 | SRC3_ENABLE;

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL0 = (OPMODE | ICLK | SLEN16 | SPEN_A | SCHEN_A | SDEN_A);
#else
    *pSPCTL0 = (OPMODE | ICLK | SLEN24 | SPEN_A | SCHEN_A | SDEN_A);
#endif
#ifdef DAC12
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL1 = (SPTRAN | ICLK | OPMODE | SLEN16 | SPEN_B | SCHEN_B | SDEN_B)  ;
#else
    *pSPCTL1 = (SPTRAN | ICLK | OPMODE | SLEN24 | SPEN_B | SCHEN_B | SDEN_B)  ;
#endif
#endif

#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_16BIT
    *pSPCTL2 = (SPTRAN | ICLK |OPMODE | SLEN16 | SPEN_A | SCHEN_A | SDEN_A | SPEN_B | SCHEN_B | SDEN_B)  ;
#else
    *pSPCTL2 = (SPTRAN | ICLK |OPMODE | SLEN24 | SPEN_A | SCHEN_A | SDEN_A | SPEN_B | SCHEN_B | SDEN_B)  ;
#endif
    *pCPSP0A = (unsigned int) TCB_Block_DMA_0A0 - OFFSET + 3;

    Delay(10000);
#ifdef DAC12
    *pCPSP1B = (unsigned int) TCB_Block_DMA_1B0 - OFFSET + 3;
#endif
    *pCPSP2A = (unsigned int) TCB_Block_DMA_2A0 - OFFSET + 3;
    *pCPSP2B = (unsigned int) TCB_Block_DMA_2B0 - OFFSET + 3;
}
