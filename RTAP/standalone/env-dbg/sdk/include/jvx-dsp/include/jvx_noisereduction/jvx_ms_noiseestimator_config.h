/*
 *****************************************************
 * Filename: jvx_ms_noiseEstimator_config.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: What to do to guarantee that Matlab yields the exact same results:
 * 				1) In jvx_ms_noiseEstimator_typedefs.h: Define Macro MS_GAMMA_LOOKUP
 * 				2) In Matlab Code: Activate methods for lookup:
 *					File jvx_ms_init.m: Line 4: gammaLookup = false;
 * 				3) Run Matlab version of the algorithm. There will be lookup tables generated
 *				  in the directory tables.
 * 				4) Compile this library involving the lookup tables generated from Matlab.
 * 				!! HINT: Make sure that all parameters are setup identically !!                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __JVX_MS_CONFIG_H__
#define __JVX_MS_CONFIG_H__

#include <float.h>
#include <math.h>

#include "jvx_helpers.h"
#include "jvx_allocate.h"
#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN

typedef struct
{
	jvxData overEstFactor;
	jvxData alphacConst[2];
	jvxData alphacMin;
	jvxData alphaMax;
	jvxData alphaMin;

	jvxData betaMin;
	jvxData betaMax;
	jvxData av;

} jvx_ms_noiseEstimator_runtime;

//! MS runtime + init parameters (runtime: to be modified while in full operation; init: can be specified for processing only BEFORE processing begins)
typedef struct
{
	jvx_ms_noiseEstimator_runtime RUNTIME;

	struct
	{
		jvxInt32 fftLength;
		jvxInt32 num_noiseSlopesM1 ;
		jvxData* Qm1M_thresholds;
		jvxData* noise_slopes;
		jvxBool switchFreqAtOnce;
		jvxInt32 numSubWindows;
		jvxInt32 numFramesSubwindow;
	} INIT;

	struct
	{
		struct
		{
			jvxInt32 minimumUpdate;
			jvxInt32 minimumSearch;
		}search;

		struct
		{
			jvxData constVals[2];
			jvxInt32 begin;
		} smoothing;
	} STARTUP;

} jvx_ms_noiseEstimator_config;

static jvxDspBaseErrorType jvx_ms_noiseEstimator_allocate_config(jvx_ms_noiseEstimator_config** cfgFld, int samplerate, int fftLength, int hopSize)
{
	jvxDspBaseErrorType res = JVX_DSP_NO_ERROR;
	if(cfgFld)
	{
                jvx_ms_noiseEstimator_config* hdl;

		// Window length in time: 1.5
		jvxData tMinSearch_s = 1.5;

		jvxData numWindows = tMinSearch_s * samplerate/hopSize;
		int numSubWindows = JVX_DATA2INT32(floor(sqrt(numWindows)));
		int sizeSubWindows = JVX_DATA2INT32(floor(numWindows/(jvxData)numSubWindows));

                JVX_DSP_SAFE_ALLOCATE_OBJECT(hdl, jvx_ms_noiseEstimator_config);

		if(fftLength%2)
		{
			return(JVX_DSP_ERROR_INVALID_SETTING);
		}

		// INIT parameters
		hdl->INIT.fftLength = fftLength;
		hdl->INIT.numSubWindows = numSubWindows;
		hdl->INIT.numFramesSubwindow = sizeSubWindows;
		hdl->INIT.num_noiseSlopesM1 = 3;
		JVX_DSP_SAFE_ALLOCATE_FIELD(hdl->INIT.Qm1M_thresholds, jvxData, hdl->INIT.num_noiseSlopesM1);
		hdl->INIT.Qm1M_thresholds[0] = JVX_DBL_2_DATA(0.02);
		hdl->INIT.Qm1M_thresholds[1] = JVX_DBL_2_DATA(0.05);
		hdl->INIT.Qm1M_thresholds[2] = JVX_DBL_2_DATA(0.08);
                JVX_DSP_SAFE_ALLOCATE_FIELD(hdl->INIT.noise_slopes, jvxData, hdl->INIT.num_noiseSlopesM1+1);
		hdl->INIT.noise_slopes[0] = 8;
		hdl->INIT.noise_slopes[1] = 4;
		hdl->INIT.noise_slopes[2] = 2;
		hdl->INIT.noise_slopes[3] = JVX_DBL_2_DATA(1.2);
		hdl->INIT.switchFreqAtOnce = true;

		hdl->STARTUP.search.minimumUpdate = 20;
		hdl->STARTUP.search.minimumSearch = 40;
		hdl->STARTUP.smoothing.begin = 30;
		hdl->STARTUP.smoothing.constVals[0] = JVX_DBL_2_DATA(0.5);
		hdl->STARTUP.smoothing.constVals[1] = JVX_DBL_2_DATA(0.4);

		hdl->RUNTIME.alphacConst[0] = JVX_DBL_2_DATA(0.7);
		hdl->RUNTIME.alphacConst[1] = JVX_DBL_2_DATA(0.3);
		hdl->RUNTIME.alphacMin = JVX_DBL_2_DATA(0.7);
		hdl->RUNTIME.alphaMax = JVX_DBL_2_DATA(0.98);
		hdl->RUNTIME.alphaMin = JVX_DBL_2_DATA(0.2);
		hdl->RUNTIME.overEstFactor = JVX_DBL_2_DATA(1.1);
		hdl->RUNTIME.betaMin = JVX_DBL_2_DATA(0.02);
		hdl->RUNTIME.betaMax = JVX_DBL_2_DATA(0.8);
		hdl->RUNTIME.av = JVX_DBL_2_DATA(2.12);
		*cfgFld = hdl;
	}
	else
	{
		res = JVX_DSP_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

static jvxDspBaseErrorType jvx_ms_noiseEstimator_deallocate_config(jvx_ms_noiseEstimator_config* cfgFld)
{
	jvxDspBaseErrorType res = JVX_DSP_NO_ERROR;
	if(cfgFld)
	{
		if(cfgFld->INIT.noise_slopes)
		{
                        JVX_DSP_SAFE_DELETE_FIELD(cfgFld->INIT.noise_slopes);
			cfgFld->INIT.noise_slopes = NULL;
		}
		if(cfgFld->INIT.Qm1M_thresholds)
		{
                        JVX_DSP_SAFE_DELETE_FIELD(cfgFld->INIT.Qm1M_thresholds);
			cfgFld->INIT.Qm1M_thresholds = NULL;
		}
                JVX_DSP_SAFE_DELETE_FIELD(cfgFld);
	}
	else
	{
		res = JVX_DSP_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

JVX_DSP_LIB_END

#endif
