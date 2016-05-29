/*
 *****************************************************
 * Filename: jvx_polynWN_noiseEstimator_config.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
*/

#ifndef __JVX_POLYNWN_NOISE_ESTIMATOR_CONFIG_H__
#define __JVX_POLYNWN_NOISE_ESTIMATOR_CONFIG_H__

#include "jvx_dsp_base.h"

// Configuration parameters, which are constant during runtime.
typedef struct {

	jvxInt32 sampleRate;
	jvxInt32 fftSize;

	// jvxData freq_hp;


} jvx_polynWN_noiseEstimator_cfg_const;


// Configuration parameters, which can be changed anytime.
typedef struct  {

	jvxData freq_highpass;
	jvxData freq_peak_search_min;

	jvxData freq_centroid_low;
	jvxData freq_centroid_hi;

	// transition range from wind only -> speech only
	jvxData freq_centroid_wind;
	jvxData freq_centroid_speech;

	jvxData freq_bin_low;
	jvxData freq_bin_mid;
	jvxData freq_bin_hi;

	jvxData ER_min;
	jvxData ER_max;
	jvxData alpha_ER;

	jvxData alpha_centroid;

	jvxData alpha_noisePSD_wind_ER;
	jvxData alpha_noisePSD_wind;
	jvxData alpha_noisePSD_speech;
	jvxData alpha_noisePSD_mixed;


	// maximum alpha for polynomial fit
	jvxData alpha_pol_max;

	// treshold for local minimum search in dB
	jvxData peak_thres_dB;

	jvxData freq_transrange_sp_act_low;
	jvxData freq_transrange_sp_act_hi;

	jvxData freq_transrange_sp_nonact_low;
	jvxData freq_transrange_sp_nonact_hi;

} jvx_polynWN_noiseEstimator_cfg_async;


// Parameters, which are derived from configuration but not set directly.
typedef struct  {

	jvxInt32 nBands;

	jvxInt32 bin_low;
	jvxInt32 bin_mid;
	jvxInt32 bin_hi;

	jvxInt32 highpass;

	// FFT bins which are relevant for centroid computation.
	jvxInt32 centroid_range_low;
	jvxInt32 centroid_range_hi;
	jvxInt32 centroid_range_size;

	jvxData centroid_wind;
	jvxData centroid_speech;

	jvxInt32 peak_search_min;
	jvxInt32 peak_search_size;

	jvxInt32 transrange_sp_act_low;
	jvxInt32 transrange_sp_act_hi;

	jvxInt32 transrange_sp_nonact_low;
	jvxInt32 transrange_sp_nonact_hi;

} jvx_polynWN_noiseEstimator_cfg_derived;




//! Configuration struct for initialization.
typedef struct
{
	jvx_polynWN_noiseEstimator_cfg_const cnst;
	jvx_polynWN_noiseEstimator_cfg_async async;

	jvx_polynWN_noiseEstimator_cfg_derived derived;

} jvx_polynWN_noiseEstimator_config;



static jvxBool jvx_polynWN_noiseEstimator_config_set_defaults(jvx_polynWN_noiseEstimator_config* cfg)
{

	// Noise Reduction Parameters: Alpha 0.995


	if(!cfg) return false;

	cfg->async.freq_centroid_low = JVX_DBL_2_DATA(50);
	cfg->async.freq_centroid_hi = JVX_DBL_2_DATA(1000);


	cfg->async.freq_centroid_wind = JVX_DBL_2_DATA(80);
	cfg->async.freq_centroid_speech = JVX_DBL_2_DATA(200);

	cfg->async.alpha_centroid = JVX_DBL_2_DATA(0.4);

	cfg->async.freq_bin_low = JVX_DBL_2_DATA(500);
	cfg->async.freq_bin_mid = JVX_DBL_2_DATA(1000);
	cfg->async.freq_bin_hi = JVX_DBL_2_DATA(8000);

	cfg->async.ER_min = JVX_DBL_2_DATA(0.07);
	cfg->async.ER_max = JVX_DBL_2_DATA(10);

	cfg->async.alpha_ER = JVX_DBL_2_DATA(0.4);

	cfg->async.alpha_noisePSD_wind_ER = JVX_DBL_2_DATA(0.3);
	cfg->async.alpha_noisePSD_wind = JVX_DBL_2_DATA(0.3);
	cfg->async.alpha_noisePSD_speech = JVX_DBL_2_DATA(0.3);
	cfg->async.alpha_noisePSD_mixed = JVX_DBL_2_DATA(0.3);

	cfg->async.freq_highpass = JVX_DBL_2_DATA(150);

	// maximum alpha for polynomial fit
	cfg->async.alpha_pol_max = JVX_DBL_2_DATA(-1);


	cfg->async.freq_peak_search_min = JVX_DBL_2_DATA(150);

	// treshold for local minimum search in dB
	cfg->async.peak_thres_dB = JVX_DBL_2_DATA(5);

	cfg->async.freq_transrange_sp_act_low = JVX_DBL_2_DATA(500);
	cfg->async.freq_transrange_sp_act_hi = JVX_DBL_2_DATA(1000);

	cfg->async.freq_transrange_sp_nonact_low = JVX_DBL_2_DATA(2000);
	cfg->async.freq_transrange_sp_nonact_hi = JVX_DBL_2_DATA(2500);



	return true;
}


#endif
