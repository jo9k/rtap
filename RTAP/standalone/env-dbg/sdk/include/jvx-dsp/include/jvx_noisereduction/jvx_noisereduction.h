/*
 *****************************************************
 * Filename: jvx_noise_reduction.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description:                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

/**
   @file
   @brief Noise reduction
 */


#ifndef __JVX_NOISE_REDUCTION_H__
#define __JVX_NOISE_REDUCTION_H__


#include "jvx_dsp_base.h"
#include "jvx_noisereduction_typedef.h"

JVX_DSP_LIB_BEGIN

//! Initialize noise reduction memory. This also returns handles to modify parameters
jvxDspBaseErrorType
jvx_noise_reduction_init(jvxHandle** hdlOut, jvxNoiseReductionParameters_init** hdlParams_in, jvxNoiseReductionParameters_runtime_async** hdlParams_rt_as, jvxNoiseReductionParameters_runtime_sync** hdlParams_rt_sy);

//! Terminate noise reduction memory.
jvxDspBaseErrorType
jvx_noise_reduction_terminate(jvxHandle* hdlReturn);

//! Start noise reduction processing
jvxDspBaseErrorType
jvx_noise_reduction_start(jvxHandle* hdl, jvxNoiseReductionParameters_init* hdlParams_in);

//! Stop noise reduction processing
jvxDspBaseErrorType
jvx_noise_reduction_stop(jvxHandle* hdl);

//! Retrieve last error code of noise reduction algorithm
jvxDspBaseErrorType
jvx_noise_reduction_get_last_error(jvxHandle* hdl);

/** Process one buffer by means of noise reduction (step1). This function return only the apriori SNR values.
 * The snrApriori values may be modified before calling the "step2"-function. */
jvxDspBaseErrorType
jvx_noise_reduction_process_step1(jvxHandle* hdl, jvxHandle* bufIn, jvxData** snrAprio);

/** Process one buffer by means of noise reduction (step2). This function return only the weights at first
 * The "weights" may be modified before calling the "step3"-function. */
jvxDspBaseErrorType
jvx_noise_reduction_process_step2(jvxHandle* hdl, jvxDataCplx** weights);

//! Process one buffer by means of noise reduction (step3). Weights may have been manipulated before, function returns the enhanced output
jvxDspBaseErrorType
jvx_noise_reduction_process_step3(jvxHandle* hdl, jvxHandle* bufOut, jvxData* alternativeWeights);

// =========================================================================================

//! Set parameters from application. Init means that this function can be called if not in processing!
jvxDspBaseErrorType
jvx_noise_reduction_update_parameters_init(jvxHandle* hdl, jvxNoiseReductionParameters_init* hdlParams, jvxBitField bitfieldSet, jvxBool set);

//! Set parameters from application. Async means that this function can be called at any time
jvxDspBaseErrorType
jvx_noise_reduction_update_parameters_async(jvxHandle* hdl, jvxNoiseReductionParameters_runtime_async* hdlParams, jvxBitField bitfieldSet, jvxBool set);

//! Set parameters from application. Sync means that this function must not be called while processing is active
jvxDspBaseErrorType
jvx_noise_reduction_update_parameters_sync(jvxHandle* hdl, jvxNoiseReductionParameters_runtime_sync* hdlParams, jvxBitField bitfieldSet, jvxBool set);

void
jvx_noise_reduction_compute_weights(jvxNoiseReductionGainCalculationRule weightingRule,
                                                  jvxDataCplx* gains,
                                                  jvxData* apriosnr,
                                                  jvxData* apostsnr,
                                                  jvxInt32 numValues,
                                                  jvxData min_gain,
                                                  jvxData* noisePsd,
                                                  jvxDataCplx* inSpec);


JVX_DSP_LIB_END

#endif
