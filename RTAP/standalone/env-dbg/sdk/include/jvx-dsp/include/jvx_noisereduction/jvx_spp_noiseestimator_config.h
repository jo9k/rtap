#ifndef __JVX_SPP_CONFIG_H__
#define __JVX_SPP_CONFIG_H__

/** What to do to guarantee that Matlab yields the exact same results:
 * HK, August 2011
 *///========================================================
#include <float.h>
#include "jvx_helpers.h"
#include "jvx_allocate.h"

typedef struct
{
	jvxData alphaPH1mean;
	jvxData alphaPSD;
	jvxData q;
	jvxData xiOptDb;
} jvx_spp_noiseEstimator_runtime;

//! MS runtime + init parameters (runtime: to be modified while in full operation; init: can be specified for processing only BEFORE processing begins)
typedef struct
{
	jvx_spp_noiseEstimator_runtime RUNTIME;

	struct
	{
		int fftSize;
		jvxData PH1mean;
	} INIT;

	struct
	{
		int cntInitMax;
	} STARTUP;

} jvx_spp_noiseEstimator_config;

static jvxBool jvx_spp_noiseEstimator_allocate_config(jvx_spp_noiseEstimator_config** cfgFld, int samplerate, int fftLength, int hopSize)
{
	jvxBool res = false;
	if(cfgFld)
	{
               jvx_spp_noiseEstimator_config* hdl;
               JVX_DSP_SAFE_ALLOCATE_OBJECT(hdl, jvx_spp_noiseEstimator_config);

		if(fftLength%2)
		{
			return(false);
		}

		hdl->INIT.fftSize = fftLength;

		// INIT parameters
		hdl->INIT.PH1mean = 0.5;

		// RUNTIME parameters
		hdl->RUNTIME.alphaPH1mean = JVX_DBL_2_DATA(0.9);
		hdl->RUNTIME.alphaPSD = JVX_DBL_2_DATA(0.8);
		hdl->RUNTIME.q = 0.5;
		hdl->RUNTIME.xiOptDb = 15;

		// STARTUP parameters
		hdl->STARTUP.cntInitMax = 5;
		*cfgFld = hdl;
		res = true;
	}
	return(res);
}

static jvxBool jvx_spp_noiseEstimator_deallocate_config(jvx_spp_noiseEstimator_config* cfgFld)
{
	jvxBool res = false;
	if(cfgFld)
	{
                JVX_DSP_SAFE_DELETE_OBJECT(cfgFld);
		res = true;
	}
	return(res);
}


#endif
