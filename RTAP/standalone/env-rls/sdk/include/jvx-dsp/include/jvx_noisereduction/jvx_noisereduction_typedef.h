#ifndef __JVX_NOISE_RECUCTION_TYPEDEFS_H__
#define __JVX_NOISE_RECUCTION_TYPEDEFS_H__

#include "jvx_noiseestimator.h"
#include "jvx_fft_tools/jvx_fft_core.h"
#include "jvx_fft_tools/jvx_fft_tools.h"
#include "jvx_dataformats.h"

typedef enum
	{
		JVX_NR_NOISE_WEIGHTING_RULE_TALKTHROUGH = 0,
		JVX_NR_NOISE_WEIGHTING_RULE_WIENER,
		JVX_NR_NOISE_WEIGHTING_RULE_MMSE_STSA,
		JVX_NR_NOISE_WEIGHTING_RULE_MMSE_LSA,
		JVX_NR_NOISE_WEIGHTING_RULE_MMSE_LAPLACE,
		JVX_NR_NOISE_WEIGHTING_RULE_SUPER_GAUSS_MAP,
		JVX_NR_NOISE_WEIGHTING_RULE_SUPER_GAUSS_JMAP
	} jvxNoiseReductionGainCalculationRule;

typedef enum
	{
		JVX_NR_PARAMS_AS_ADDRESS_NR_PARAMETERS = 0x1,
		JVX_NR_PARAMS_AS_ADDRESS_MS_NE = 0x2,
		JVX_NR_PARAMS_AS_ADDRESS_MMSE_NE = 0x4,
		JVX_NR_PARAMS_AS_ADDRESS_SPP_NE = 0x8,
		JVX_NR_PARAMS_AS_ADDRESS_POLYNWN_NE = 0x10,
		JVX_NR_PARAMS_AS_ADDRESS_MUSIC_TONE_BLOCK = 0x20,
		JVX_NR_PARAMS_AS_ADDRESS_WEIGHTING_RULE = 0x40,
	} jvxNoiseReduction_address_as;

typedef enum
	{
		JVX_NR_PARAMS_S_ADDRESS_ANALYSIS_FFT = 0x1,
	} jvxNoiseReduction_address_sy;

typedef struct
{
	jvxData alpha_c_min;
	jvxData alpha_c_const_0;
	jvxData alpha_c_const_1;
	jvxData alpha_min;
	jvxData alpha_max;
} alpha_smooth_nestimator;

typedef struct
{
	jvxInt32 framesize;
	jvxInt32 buffersize;
	jvxInt32 fftsize;
	jvxInt32 samplerate;
	jvxDataFormat format;
	struct
	{
		jvxFftTools_fwkType fftFrameworkType; // Reference 1
		jvxInt32 desiredNumFilterCoeffs; // No reference
		jvx_windowType desiredWindowType; // No reference
	} fft_processing;

	struct
	{
		jvxData lookback_window_sec; // Reference 16
		jvxData startup_smooth; // Reference 17
		jvxData stop_smooth; // Reference 18
		jvxData update_at_once; // Reference 19
	} minStatControl_init;

	struct
	{
		jvxData lookback_window_sec; // Reference 20
		jvxData startup_alpha; // Reference 23
		jvxData stop_alpha; // Reference 24
		jvxData update_at_once; // Reference 25
	} mmseControl_init;

	struct
	{
		jvxData ph1_mean; // Reference 26
	} sppControl_init;

	jvx_polynWN_noiseEstimator_cfg_const polynWNControl_init;

} jvxNoiseReductionParameters_init;

typedef struct
{
	jvx_noiseEstimator_types fftNoiseEstimatorType; // Reference 2
	jvxBool use_second_fft_analysis; // Reference 3
	jvxData alpha; // Reference 4
	jvxData min_apriori_snr; // Reference 5
	jvxData min_gain; // Reference 6

	struct
	{
		jvxData smoothing_factor; // Reference 7
		jvxData corridor_plus; // Reference 8
		jvxData corridor_minus; // Reference 9
		jvxBool is_active; // Reference 10
	} block_musical_tones;

	jvxNoiseReductionGainCalculationRule weighting_rule; // Reference 11

	jvxBool employ_antialias_weight_processing; // Reference 12
	jvxBool auto_aliasing; // Reference 13

	//jvxData smoothingFactor_energyInput;
	//jvxData smoothingFactor_energyOutput;
	//jvxBool gainControlActive;
	//jvxData gainControl_maxGain;

	struct
	{
		jvxData over_estimation_factor; // Reference 29
		jvxData beta_min; // Reference 35
		jvxData beta_max; // Reference 36
		jvxData av; // Reference 37
		alpha_smooth_nestimator alpha_smooth; // Reference 30- Reference 34
	} minStatControl_runtime;

	struct
	{
		jvxData alpha; // Reference 38
		jvxData gmin; // Reference 39
		//alpha_smooth_nestimator alpha_smooth; // Reference 40 - Reference 44
	} mmseControl_runtime;

	struct
	{
		jvxData alpha_ph1_mean; // Reference 45
		jvxData alpha_psd; // Reference 46
		jvxData q; // Reference 47
		jvxData xopt_db; // Reference 48
		//alpha_smooth_nestimator alpha_smooth; // Reference 49 - Reference 53
	} sppControl_runtime;

	jvx_polynWN_noiseEstimator_cfg_async polynWNControl_runtime;


} jvxNoiseReductionParameters_runtime_async;

typedef struct
{
	struct
	{
		jvxInt32 percentAnalysisWinFftSize; // Reference 15
		jvx_windowType wintype; // Reference 14
	} fft_analysis;

	struct
	{
		jvxInt32 numGains;
	} readOnly;

	//	jvxInt32 neAnalysisSize;
} jvxNoiseReductionParameters_runtime_sync;

#endif
