#ifndef __JVXL_NOISEESTIMATOR_H__
#define __JVXL_NOISEESTIMATOR_H__

#include "jvx_dsp_base.h"

#include "jvx_noisereduction/jvx_pwn_noiseestimator.h"
#include "jvx_noisereduction/jvx_spp_noiseestimator.h"
#include "jvx_noisereduction/jvx_ms_noiseestimator.h"
#include "jvx_noisereduction/jvx_mmse_noiseestimator.h"

// public typedefs
typedef enum
{
  JVX_NOISE_ESTIMATOR_MS,
  JVX_NOISE_ESTIMATOR_MMSE,
  JVX_NOISE_ESTIMATOR_SPP,
  JVX_NOISE_ESTIMATOR_POLYNWN
} jvx_noiseEstimator_types;


// helper structs
typedef struct
{
  jvxInt32 R;
  jvxInt32 M;
  jvxInt32 sRate;

  jvx_ms_noiseEstimator_config *msCfg;
  jvx_mmse_noiseEstimator_config *mmseCfg;
  jvx_spp_noiseEstimator_config *sppCfg;

} jvx_noiseEstimator_config;

typedef struct
{
  jvx_noiseEstimator_types type;

  /* jvx_ms_noiseEstimator_runtime* nr_hdlMSRuntime; // to intern? */
  /* jvx_mmse_noiseEstimator_runtime* nr_hdlMMSERuntime; */
  /* jvx_spp_noiseEstimator_runtime* nr_hdlSPPRuntime; */
  /* jvx_polynWN_noiseEstimator_runtime* nr_hdlPOLYNWNuntime; */
} jvx_noiseEstimator_async;

typedef struct
{
  void *dummy;
} jvx_noiseEstimator_sync;

typedef struct
{
  void *dummy;
} jvx_noiseEstimator_debug;

// main struct for interfacing with this module
typedef struct
{
  jvxHandle *intern;
  jvx_noiseEstimator_config config;
  jvx_noiseEstimator_async async;
  jvx_noiseEstimator_sync sync;
  jvx_noiseEstimator_debug debug;
} jvx_noiseEstimator;

// fill an jvx_noiseEstimator struct with default values
static jvxDspBaseErrorType jvx_noiseEstimator_default_config(jvx_noiseEstimator *hdl)
{
  if(!hdl)
    return JVX_DSP_ERROR_INVALID_ARGUMENT;

  hdl->async.type = JVX_NOISE_ESTIMATOR_MS;

  return JVX_DSP_NO_ERROR;
}

// public interface functions
jvxDspBaseErrorType jvx_noiseEstimator_config_init(jvx_noiseEstimator *hdl,
                                                          jvxInt32 R,
                                                          jvxInt32 M,
                                                          jvxInt32 sRate);
jvxDspBaseErrorType jvx_noiseEstimator_init(jvx_noiseEstimator *hdl);
jvxDspBaseErrorType jvx_noiseEstimator_terminate(jvx_noiseEstimator *hdl);
jvxDspBaseErrorType jvx_noiseEstimator_process(jvx_noiseEstimator *hdl,
                                                      jvxData* input,
                                                      jvxData* output);
jvxDspBaseErrorType jvx_noiseEstimator_update_sync(jvx_noiseEstimator *hdl,
                                                   jvxBool do_set);
jvxDspBaseErrorType jvx_noiseEstimator_update_async(jvx_noiseEstimator *hdl,
                                                    jvxBool do_set);


#endif
