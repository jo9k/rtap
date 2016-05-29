/******************************************************
 * Filename: accessSPORT.h	  						  *
 * Description: Class to access the SPORT for audio   *
 *				I/O. The class has only two functions,*
 *  			the class wrapper is used for name    *
 *				spacing.							  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/
 
#ifndef _ACCESSSPORT_H__
#define _ACCESSSPORT_H__

/*==================================================
 * Class to access the serial port for audio I/O.
 *///===============================================
 class accessSPORT
{
public:

	//=============================================
	// Profiling variables
#ifdef PROFILE_AUDIO

	//! Obtain the DMA sample counter on input IRQ, input DMA
	WORD32 cntSamplesIRQInput_in;

	//! Obtain the DMA sample counter on input IRQ, output DMA
	WORD32 cntSamplesIRQInput_out;

	//! Obtain the DMA sample counter on output IRQ, input DMA
	WORD32 cntSamplesIRQOutput_in;

	//! Obtain the DMA sample counter on output IRQ, output DMA
	WORD32 cntSamplesIRQOutput_out;
	
	//! Obtain the DMA sample counter on SFT IRQ, input DMA, on function start
	WORD32 cntSamplesSFTI_in_enter;
	
	//! Obtain the DMA sample counter on SFT IRQ, output DMA, on function start
	WORD32 cntSamplesSFTI_out_enter;
	
	//! Obtain the DMA sample counter on SFT IRQ, input DMA, on function end
	WORD32 cntSamplesSFTI_in_leave;
	
	//! Obtain the DMA sample counter on SFT IRQ, output DMA, on function stop
	WORD32 cntSamplesSFTI_out_leave;
	
	//! Calculate the current load, (cntSamplesSFTI_out_enter-cntSamplesSFTI_out_leave)/(2*BUFFERSIZE)
	float load;
#endif
	//=============================================

private:

public:
	void InitSPORT();
	void termSPORT();
	
};

#endif
