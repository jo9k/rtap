/*
 *****************************************************
 * Filename: jvx_mmse_noiseEstimator_config.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: What to do to achieve that Matlab yields very similar results:
 * 				1) In jvx_mmse_noiseEstimator_typedefs.h: Define Macro MMSE_GAMMA_INC_LOOKUP
 * 				2) In Matlab Code: Activate methods for lookup:
 *					File jvx_mmse_init.m: Line 4: USE_C_REFERENCE = true;
 *					(make sure that approxFunctionLookup = true; in line 15)
 * 				3) Run Matlab version of the algorithm. There will be lookup tables generated
 *					  in the directory tables.
 * 				4) Compile this library involving the lookup tables generated from Matlab.
 * 				!! HINT: Make sure that all parameters are setup identically !!
 * 				The result is not exactly identical, we should see a (max) deviation around
 * 				1.3248e-023 for the exemplary wav-file. The deviation is due to the use of the exp function
 * 				in both versions.                                     *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __JVX_MMSE_CONFIG_H__
#define __JVX_MMSE_CONFIG_H__

#include <float.h>
#include "jvx_helpers.h"
#include "jvx_allocate.h"

//! MMSE runtime parameters (runtime: to be modified while in full operation; init: can be specified for processing only BEFORE processing begins)
typedef struct
{
	jvxData minGain;
	jvxData alpha;
	jvxData alpha_exec;

	// Currently algorithm works only for gamma=1 and nu = 0.6, snrLowLim is set by init function in a useful way
	//int gamma;
	//jvxData nu;
	//jvxData snrLowLim;

#ifdef USE_ALPHA_OPT
	jvxData alphacConst[2];
	jvxData alphacMin;
	jvxData alphaMax;
	jvxData alphaMin;
#endif

} jvx_mmse_noiseEstimator_runtime;

//! MMSE runtime + init parameters (runtime: to be modified while in full operation; init: can be specified for processing only BEFORE processing begins)
typedef struct
{
	jvx_mmse_noiseEstimator_runtime RUNTIME;

	struct
	{
		int fftLength;
		jvxBool switchFreqAtOnce;
		int numSubwindows;
		int lengthSubwindow;
	} INIT;

	struct
	{
		int completeAveraging;
		int startupNoisePSD;
		jvxData alpha_start;
#ifdef USE_ALPHA_OPT
		struct
		{
			jvxInt32 begin;
			jvxData constVals[2];
		} smoothing;
#endif
	} STARTUP;
} jvx_mmse_noiseEstimator_config;

static jvxDspBaseErrorType jvx_mmse_noiseEstimator_allocate_config(jvx_mmse_noiseEstimator_config** hdlOut, int samplerate, int fftLength, int hopSize)
{
	if(hdlOut)
	{
                jvx_mmse_noiseEstimator_config* hdl;

		jvxData mmseNumWindows = JVX_DBL_2_DATA(floor(0.8*samplerate/hopSize));
		int numSubWindows = JVX_DATA2INT32(mmseNumWindows);
		int lengthSubwindow = 1;

                JVX_DSP_SAFE_ALLOCATE_OBJECT(hdl, jvx_mmse_noiseEstimator_config);

		hdl->INIT.fftLength = fftLength;
		hdl->INIT.numSubwindows = numSubWindows;
		hdl->INIT.lengthSubwindow = lengthSubwindow;
		hdl->INIT.switchFreqAtOnce = true;

		hdl->RUNTIME.alpha = JVX_DBL_2_DATA(0.98);
		hdl->RUNTIME.alpha_exec = JVX_DBL_2_DATA(0.8);

		hdl->RUNTIME.minGain = mat_epsMin;
		hdl->STARTUP.alpha_start = JVX_DBL_2_DATA(0.92);
		hdl->STARTUP.completeAveraging = 5;
		hdl->STARTUP.startupNoisePSD = 10;

#ifdef USE_ALPHA_OPT
		hdl->RUNTIME.alphacConst[0] = 0.7;
		hdl->RUNTIME.alphacConst[1] = 0.3;
		hdl->RUNTIME.alphacMin = 0.7;
		hdl->RUNTIME.alphaMax = 0.98;
		hdl->RUNTIME.alphaMin = 0.2;
		hdl->STARTUP.smoothing.begin = 30;
		hdl->STARTUP.smoothing.constVals[0] = 0.5;
		hdl->STARTUP.smoothing.constVals[1] = 0.4;
#endif

		*hdlOut = hdl;
		return(JVX_DSP_NO_ERROR);
	}
	return(JVX_DSP_ERROR_INVALID_ARGUMENT);
};

static jvxDspBaseErrorType jvx_mmse_noiseEstimator_deallocate_config(jvx_mmse_noiseEstimator_config* hdlOut)
{
	if(hdlOut)
	{
                JVX_DSP_SAFE_DELETE_OBJECT(hdlOut);
		return(JVX_DSP_NO_ERROR);
	}
	return(JVX_DSP_ERROR_INVALID_ARGUMENT);
};

#endif
