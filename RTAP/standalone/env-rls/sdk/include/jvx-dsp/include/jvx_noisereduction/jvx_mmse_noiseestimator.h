/* 
 *****************************************************
 * Filename: jvx_mmse_noiseEstimator.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: File to realize the MMSE noise estimation for nrMotor.
 *				The set of functions enables initialize, terminate and a compute function.
 *				The code is derived from the code of the original authors of the paper
 *				"MMSE BASED NOISE PSD TRACKING WITH LOW COMPLEXITY", by R. C.Hendriks, 
 *				R. Heusdens and J. Jensen published at the IEEE International Conference 
 *				on Acoustics, Speech and Signal Processing, 03/2010, Dallas, TX, p.4266-4269, (2010).
 *				Note that we started with original Matlab code downloaded from the authors web page
 *				which however, does not have too much in common with the paper and is not very 
 *				efficiently implemented.
 *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __JVX_MMSE_NOISEESTIMATOR_H__
#define __JVX_MMSE_NOISEESTIMATOR_H__

#include "jvx_dsp_base.h"
#include "jvx_mmse_noiseestimator_config.h"

//! Initialize object for MMSE noise estimation. The fft size, the subwindows/windows as well as the alpha can be setup. The returned handle must be used lateron.
jvxDspBaseErrorType jvx_mmse_noiseEstimator_init(jvxHandle** handle, int fftSize, int numSubwindows, int lengthSubwindow, jvxData alpha);

//! Initialize object for MMSE noise estimation. Due to the passed struct, almost all parameters can be setup (see nrMotor_api.h for the definition of the passed struct.
jvxDspBaseErrorType jvx_mmse_noiseEstimator_init_params(jvxHandle** handle, jvx_mmse_noiseEstimator_config* Hc );

//! Update the runtime parameters. Runtime parameters are those which can be modified during operation.
jvxDspBaseErrorType jvx_mmse_noiseEstimator_update_params(jvxHandle* handle, jvx_mmse_noiseEstimator_runtime* params, jvxBool doset);

//! Terminate the noise estimation object. In this function, all used memory is deallocated
jvxDspBaseErrorType jvx_mmse_noiseEstimator_terminate(jvxHandle* handle);

//! Function to compute a noisePSD estimate given a complex spectrum for one frame
#ifdef USE_ALPHA_OPT
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_fft(jvxHandle* handle, jvxData* in_spectrumNoisy_re, jvxData* in_spectrumNoisy_im, jvxData* out_noisePSD, jvxData* alphaopt);
#else
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_fft(jvxHandle* handle, jvxData* in_spectrumNoisy_re, jvxData* in_spectrumNoisy_im, jvxData* out_noisePSD );
#endif

//! Function to compute a noisePSD estimate given a complex spectrum for one frame
#ifdef USE_ALPHA_OPT
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_fft2(jvxHandle* handle, jvxDataCplx* in_spectrumNoisy, jvxData* out_noisePSD, jvxData* alphaopt);
#else
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_fft2(jvxHandle* handle, jvxDataCplx* in_spectrumNoisy, jvxData* out_noisePSD );
#endif

//! Function to compute a noisePSD estimate given a complex spectrum for one frame
#ifdef USE_ALPHA_OPT
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_psd(jvxHandle* handle, jvxData* in_PSDNoisy, jvxData* out_noisePSD, jvxData* alphaopt);
#else
jvxDspBaseErrorType jvx_mmse_noiseEstimator_process_psd(jvxHandle* handle, jvxData* in_PSDNoisy, jvxData* out_noisePSD );
#endif

#endif
