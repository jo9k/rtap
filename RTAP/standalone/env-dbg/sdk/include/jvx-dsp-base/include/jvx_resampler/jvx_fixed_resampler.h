#ifndef __JVX_FIXED_RESAMPLER_H__
#define __JVX_FIXED_RESAMPLER_H__

#include "jvx_dsp_base.h"
#include "jvx_dataformats.h"

jvxDspBaseErrorType jvx_fixed_resampler_init(jvxHandle** H,
                                             jvxData* delay_samples, int lFilter,
                                             int buffersizeIn, int buffersizeOut, jvxDataFormat format,
                                             jvxData stopbandAtten_dB, jvxData socketVal);

jvxDspBaseErrorType jvx_fixed_resampler_terminate(jvxHandle* H);

jvxDspBaseErrorType jvx_fixed_resampler_process(jvxHandle* H, void* in, void* out);

#endif
