/******************************************************
 * Filename: initSRU.c								  *
 * Description: Functionality to setup the signal     *
 *				routing unit (SRU) for audio I/O via  *
 *				serial ports and sampling rate 		  *
 *				converters (SRC)					  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #include "userDef.h"

// The following definition allows the SRU macro to check for errors. Once the routings have
// been verified, this definition can be removed to save some program memory space.
// The preprocessor will issue a warning stating this when using the SRU macro without this
// definition
#define SRUDEBUG  // Check SRU Routings for errors.
#include <SRU.h>

//------------------------------------------------------------------------

/*==================================================
 * Initialize the SRU
 *///===============================================
void InitSRU()
{

//-----------------------------------------------------------------------------
//
//  MCLK: The output of the 12.288 MHz xtal is either directly connected to the
//        codec, but also connected to DAI_P06, or just to DAI_P17. This is
//        determined by switch SW3 For this example we route the MCLK into
//        DAI_P17 and supply the clock to the ADC via DAI_P06  by routing the
//        signal through the SRU.

//  Tie the pin buffer input LOW. *1*
    SRU(LOW,DAI_PB17_I);

//  Tie the pin buffer enable input LOW *2*
    SRU(LOW,PBEN17_I);

//-----------------------------------------------------------------------------
//
//  Connect the ADC: The codec drives a BCLK output to DAI pin 7, a LRCLK
//          (a.k.a. frame sync) to DAI pin 8 and data to DAI pin 5.
//
//          Connect the ADC to SPORT0, using data input A
//
//          All three lines are always inputs to the SHARC so tie the pin
//          buffer inputs and pin buffer enable inputs all low.


//------------------------------------------------------------------------
//  Connect the ADC to SPORT0, using data input A

    // Put SRC0 into the data chain
    SRU(SPORT0_FS_O, SRC0_FS_OP_I); 	/*3*/
    SRU(DAI_PB08_O, SRC0_FS_IP_I); 		/*4*/
    SRU(SPORT0_CLK_O, SRC0_CLK_OP_I); 	/*5*/
    SRU(DAI_PB07_O, SRC0_CLK_IP_I); 	/*6*/
    SRU(SRC0_DAT_OP_O, SPORT0_DA_I);	/*7*/
    SRU(DAI_PB05_O, SRC0_DAT_IP_I);	/*8*/

    //  Clock in on pin 7
    //SRU(DAI_PB07_O,SPORT0_CLK_I);

    //  Frame sync in on pin 8
    //SRU(DAI_PB08_O,SPORT0_FS_I);

    //  Data in on pin 5
    //SRU(DAI_PB05_O,SPORT0_DA_I);

//------------------------------------------------------------------------
//    Tie the pin buffer inputs LOW for DAI pins 5, 7 and 8.  Even though
//    these pins are inputs to the SHARC, tying unused pin buffer inputs
//    LOW is "good coding style" to eliminate the possibility of
//    termination artifacts internal to the IC.  Note that signal
//    integrity is degraded only with a few specific SRU combinations.
//    In practice, this occurs VERY rarely, and these connections are
//    typically unnecessary.


    SRU(LOW,DAI_PB05_I); 	/*9*/
    SRU(LOW,DAI_PB07_I);	/*10*/
    SRU(LOW,DAI_PB08_I);	/*11*/

//------------------------------------------------------------------------
//  Tie the pin buffer enable inputs LOW for DAI pins 5, 6, 7 and 8 so
//  that they are always input pins.

    SRU(LOW,PBEN05_I);		/*12*/
    SRU(LOW,PBEN07_I);		/*13*/
    SRU(LOW,PBEN08_I);		/*14*/

//-----------------------------------------------------------------------------
//
//  Connect the DACs: The codec accepts a BCLK input from DAI pin 13 and
//          a LRCLK (a.k.a. frame sync) from DAI pin 14 and has four
//          serial data outputs to DAI pins 12, 11, 10 and 9
//
//          Connect DAC1 to SPORT1, using data output A
//          Connect DAC2 to SPORT1, using data output B
//          Connect DAC3 to SPORT2, using data output A
//          Connect DAC4 to SPORT2, using data output B
//
//          Connect the clock and frame sync inputs to SPORT1 and SPORT2
//          should come from the ADC on DAI pins 7 and 8, respectively
//
//          Connect the ADC BCLK and LRCLK back out to the DAC on DAI
//          pins 13 and 14, respectively.
//
//          All six DAC connections are always outputs from the SHARC
//          so tie the pin buffer enable inputs all high.
//

//------------------------------------------------------------------------
//  Connect the pin buffers to the SPORT data lines and ADC BCLK & LRCLK

#ifdef CHANNELSET3
    SRU(SPORT1_DB_O, SRC1_DAT_IP_I);	/*17a*/
    SRU(SRC1_DAT_OP_O, DAI_PB11_I);		/*18a*/
#endif
    SRU(SPORT2_DA_O, SRC2_DAT_IP_I); 	/*15*/
    SRU(SRC2_DAT_OP_O, DAI_PB10_I);	 	/*16*/
    SRU(SPORT2_DB_O, SRC3_DAT_IP_I);	/*17*/
    SRU(SRC3_DAT_OP_O, DAI_PB09_I);		/*18*/

//	SRU(SPORT2_DB_O,DAI_PB09_I);
//  SRU(SPORT2_DA_O,DAI_PB10_I);
//  SRU(SPORT1_DB_O,DAI_PB11_I);
//  SRU(SPORT1_DA_O,DAI_PB12_I);

//------------------------------------------------------------------------
//  Connect the clock and frame sync input from the ADC directly
//    to the output pins driving the DACs.

    SRU(DAI_PB07_O,DAI_PB13_I);		/*19*/
    SRU(DAI_PB08_O,DAI_PB14_I);		/*20*/
    SRU(DAI_PB17_O,DAI_PB06_I);		/*21*/

//------------------------------------------------------------------------
//  Connect the SPORT clocks and frame syncs to the clock and
//  frame sync from the SPDIF receiver

#ifdef CHANNELSET3
    SRU(DAI_PB07_O, SRC1_CLK_OP_I);		/*26a*/
    SRU(SPORT1_CLK_O, SRC1_CLK_IP_I);	/*27a*/
    SRU(DAI_PB08_O, SRC1_FS_OP_I);		/*28a*/
    SRU(SPORT1_FS_O, SRC1_FS_IP_I);		/*29a*/
#endif
    SRU(DAI_PB07_O, SRC2_CLK_OP_I);		/*26*/
    SRU(SPORT2_CLK_O, SRC2_CLK_IP_I);	/*27*/
    SRU(DAI_PB08_O, SRC2_FS_OP_I);		/*28*/
    SRU(SPORT2_FS_O, SRC2_FS_IP_I);		/*29*/
   SRU(DAI_PB07_O, SRC3_CLK_OP_I);		/*22*/
    SRU(SPORT2_CLK_O, SRC3_CLK_IP_I);	/*23*/
    SRU(DAI_PB08_O, SRC3_FS_OP_I);		/*24*/
    SRU(SPORT2_FS_O, SRC3_FS_IP_I);		/*25*/

//    SRU(DAI_PB07_O,SPORT1_CLK_I);
//    SRU(DAI_PB07_O,SPORT2_CLK_I);
//    SRU(DAI_PB08_O,SPORT1_FS_I);
//    SRU(DAI_PB08_O,SPORT2_FS_I);

//------------------------------------------------------------------------
//  Tie the pin buffer enable inputs HIGH to make DAI pins 9-14 outputs.
    SRU(HIGH,PBEN06_I);		/*30*/
    SRU(HIGH,PBEN09_I);		/*31*/
    SRU(HIGH,PBEN10_I);		/*32*/
    SRU(HIGH,PBEN11_I);		/*33*/
    SRU(HIGH,PBEN12_I);		/*34*/
    SRU(HIGH,PBEN13_I);		/*35*/
    SRU(HIGH,PBEN14_I);		/*36*/
//--------------------------------------------------------------------------
// Route SPI signals to AD1835.

    SRU(SPI_MOSI_O,DPI_PB01_I)      //Connect MOSI to DPI PB1.
    SRU(DPI_PB02_O, SPI_MISO_I)     //Connect DPI PB2 to MISO.
    SRU(SPI_CLK_O, DPI_PB03_I)      //Connect SPI CLK to DPI PB3.
    SRU(SPI_FLG3_O, DPI_PB04_I)     //Connect SPI FLAG3 to DPI PB4.
//---------------------------------------------------------------------------
// Tie pin buffer enable from SPI peipherals to determine whether they are
// inputs or outputs

    SRU(SPI_MOSI_PBEN_O, DPI_PBEN01_I);
    SRU(SPI_MISO_PBEN_O, DPI_PBEN02_I);
    SRU(SPI_CLK_PBEN_O, DPI_PBEN03_I);
    SRU(SPI_FLG3_PBEN_O, DPI_PBEN04_I);

//-----------------------------------------------------------------------------



}





