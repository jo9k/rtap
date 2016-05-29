/*
 *****************************************************
 * Filename: jvx_polynWN_noiseEstimator.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
*/


#ifndef _JVX_POLYNWN_NOISE_ESTIMATOR_H__
#define _JVX_POLYNWN_NOISE_ESTIMATOR_H__

#include "jvx_dsp_base.h"
#include "jvx_pwn_noiseestimator_config.h"


typedef struct {

	jvxInt32 framecounter;

} jvx_polynWN_noiseEstimator_debug;



typedef struct {

	jvxHandle* intern;

	jvx_polynWN_noiseEstimator_config cfg;
	jvx_polynWN_noiseEstimator_debug debug;

} jvx_polynWN_noiseEstimator;



//! Initialize the polynWN data struct.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_init(jvx_polynWN_noiseEstimator* hdl, jvxInt32 sampleRate, jvxInt32 fftSize);


//! Start polynWN processing. Call after init and before process.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_start(jvx_polynWN_noiseEstimator* hdl);


//! Processing of single buffer. Computation of noise PSD. Complex FFT signal input.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_process_fft(jvx_polynWN_noiseEstimator* hdl,
                                                           jvxDataCplx* oneBufferFFT_in,
                                                           jvxData* noisePSD_out);

//!  Processing of single buffer. Computation of noise PSD. Real-valued signal PSD input.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_process_psd(jvx_polynWN_noiseEstimator* hdl,
                                                           jvxData* oneBufferPSD_in,
                                                           jvxData* noisePSD_out);


//! Stop polynWN processing. Call when processing of buffers is over.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_stop(jvx_polynWN_noiseEstimator* hdl);


//! Terminate the object.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_terminate(jvx_polynWN_noiseEstimator* hdl);


//! Set or get public constant configuration struct to/from jvx_polynWN_noiseEstimator.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_update_config_const(jvx_polynWN_noiseEstimator* hdl,
                                                                   jvxBool do_set);

//! Set or get public async configuration struct to/from jvx_polynWN_noiseEstimator.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_update_config_async(jvx_polynWN_noiseEstimator* hdl,
                                                                   jvxBool do_set);

/*
//! Set or get public sync configuration struct to/from jvx_polynWN_noiseEstimator.
jvxDspBaseErrorType jvx_polynWN_noiseEstimator_update_config_sync(jvx_polynWN_noiseEstimator* hdl,
																		  jvxBool do_set);
*/


#endif
