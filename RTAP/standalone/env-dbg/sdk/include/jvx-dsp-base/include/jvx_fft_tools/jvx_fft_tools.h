#ifndef __JVX_FFT_TOOLS_H__
#define __JVX_FFT_TOOLS_H__

#include "jvx_dsp_base_types.h"
#include "jvx_windows/jvx_windows.h"
#include "jvx_fft_tools/jvx_fft_core.h"

// =======================================================================================

typedef jvxHandle jvxFFTfwk;
typedef jvxHandle jvxIFFTfwk;
typedef jvxHandle jvxFFTweightMod;

typedef struct
{
	jvxFFT* hdlFft;
	jvxIFFT* hdlIfft;
	jvxFFTfwk* hdlFftFwk;
	jvxIFFTfwk* hdlIfftFwk;
	jvxData* inputFFT;
	jvxData* outputIFFT;
	jvxDataCplx* outputFFT;
	jvxDataCplx* inputIFFT_newWeight;
	jvxDataCplx* inputIFFT_oldWeight;
	jvxData delay;
} jvxFFTframework;

typedef	struct
{
	jvxFFT* hdlFft;
	jvxFFTfwk* hdlFftFwk;
	jvxData* inputFFT;
	jvxDataCplx* outputFFT;
	jvxData* window_ptr;
	jvxData* window;
	jvxInt32 numSamplesValidWindow;
} jvxFFTframeworkAnalysis;

typedef	struct
{
	jvxFFT* hdlFft;
	jvxIFFT* hdlIfft;
	jvxHandle* hdlProcessWeights;
	jvxDataCplx* input_complex;
	jvxDataCplx* output_complex;
	jvxInt32 number_inputoutputComplex;
} jvxFFTframeworkWeightCorrection;

//! Typedefs for FFT framework types
typedef enum
{
	JVX_FFT_TOOLS_FWK_SW_OLA = 0,
	JVX_FFT_TOOLS_FWK_DW_OLA,
	JVX_FFT_TOOLS_FWK_OLA_CF,
	JVX_FFT_TOOLS_FWK_OLS,
	JVX_FFT_TOOLS_FWK_OLS_CF,
	JVX_FFT_TOOLS_FWK_ANALYSIS
} jvxFftTools_fwkType;


/**
 * Allocate two handles: One for the FFT part, one for the IFFT part. Pass in framesize,
 * buffersize, handles for FFT and IFFT. The returned data are buffers for input and output as well as
 * the buffersizes and the handles for FFT and IFFT.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_init_framework_dw_ola(jvxFFTfwk** hdlOut_fft,
				    jvxIFFTfwk** hdlOut_ifft,
				    jvxInt32 framesize,
				    jvxInt32 buffersize,
				    jvxFFT* coreFft,
				    jvxIFFT* coreIfft,
				    jvxData** inputReal,
				    jvxData** outputReal,
				    jvxDataCplx** inputComplex,
				    jvxDataCplx** outputComplex,
				    jvxInt32* numElements_timeDomain,
				    jvxInt32* numElements_Spectrum,
				    jvxData* delay);

/**
 * Allocate two handles: One for the FFT part, one for the IFFT part. Pass in framesize,
 * buffersize, handles for FFT and IFFT. The returned data are buffers for input and output as well as
 * the buffersizes and the handles for FFT and IFFT.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_init_framework_sw_ola(jvxFFTfwk** hdlOut_fft,
				    jvxIFFTfwk** hdlOut_ifft,
				    jvxInt32 framesize,
				    jvxInt32 buffersize,
				    jvxFFT* coreFft,
				    jvxIFFT* coreIfft,
				    jvxData** inputReal,
				    jvxData** outputReal,
				    jvxDataCplx** inputComplex,
				    jvxDataCplx** outputComplex,
				    jvxInt32* numElements_timeDomain,
				    jvxInt32* numElements_Spectrum,
				    jvxData* delay);

/**
 * Allocate two handles: One for the FFT part, one for the IFFT part. Pass in framesize,
 * buffersize, handles for FFT and IFFT. The returned data are buffers for input and output as well as
 * the buffersizes and the handles for FFT and IFFT.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_init_framework_ola_cf(jvxFFTfwk** hdlOut_fft,
				    jvxIFFTfwk** hdlOut_ifft,
				    jvxInt32 framesize,
				    jvxFFT* coreFft,
				    jvxIFFT* coreIfft,
				    jvxData** inputReal,
				    jvxData** outputReal,
				    jvxDataCplx** inputComplex_newWeights,
				    jvxDataCplx** inputComplex_oldWeights,
				    jvxDataCplx** outputComplex,
				    jvxInt32* numElements_timeDomain,
				    jvxInt32* numElements_Spectrum,
				    jvxData* delay);

/**
 * Allocate two handles: One for the FFT part, one for the IFFT part. Pass in framesize,
 * buffersize, handles for FFT and IFFT. The returned data are buffers for input and output as well as
 * the buffersizes and the handles for FFT and IFFT.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_init_framework_ols(jvxFFTfwk** hdlOut_fft,
				 jvxIFFTfwk** hdlOut_ifft,
				 jvxInt32 framesize,
				 jvxFFT* coreFft,
				 jvxIFFT* coreIfft,
				 jvxData** inputReal,
				 jvxData** outputReal,
				 jvxDataCplx** inputComplex,
				 jvxDataCplx** outputComplex,
				 jvxInt32* numElements_timeDomain,
				 jvxInt32* numElements_Spectrum,
				 jvxData* delay);

/**
 * Allocate two handles: One for the FFT part, one for the IFFT part. Pass in framesize,
 * buffersize, handles for FFT and IFFT. The returned data are buffers for input and output as well as
 * the buffersizes and the handles for FFT and IFFT.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_init_framework_ols_cf(jvxFFTfwk** hdlOut_fft,
				    jvxIFFTfwk** hdlOut_ifft,
				    jvxInt32 framesize,
				    jvxFFT* coreFft,
				    jvxIFFT* coreIfft,
				    jvxData** inputReal,
				    jvxData** outputReal,
				    jvxDataCplx** inputComplex_newWeights,
				    jvxDataCplx** inputComplex_oldWeights,
				    jvxDataCplx** outputComplex,
				    jvxInt32* numElements_timeDomain,
				    jvxInt32* numElements_Spectrum,
				    jvxData* delay);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TERMINATE FRAMEWORKS TERMINATE FRAMEWORKS TERMINATE FRAMEWORKS TERMINATE FRAMEWORKS
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * Deallocate handles for FFT and IFFT for OLS_CF. Note that also the buffers returned
 * on init will no longer be valid. Framework type will be derived from handle.
 *///====================================================================================
jvxDspBaseErrorType
jvx_fft_tools_terminate_framework(jvxFFTfwk* hdl_fft,
				  jvxIFFTfwk* hdl_ifft);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ALLOCATION OF FFT OR IFFT PART
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Allocate FFT framework handle given a specific framework type
jvxDspBaseErrorType
jvx_fft_tools_init_framework_fft(jvxFFTfwk** hdlOut,
				 jvxFftTools_fwkType,
				 jvxInt32 framesize,
				 jvxInt32 buffersize,
				 jvxFFT* coreFft,
				 jvxData** inputReal,
				 jvxDataCplx** outputComplex,
				 jvxInt32* numElements_input,
				 jvxInt32* numElements_output);

//! Allocate IFFT framework handle given a specific framework type
jvxDspBaseErrorType
jvx_fft_tools_init_framework_ifft(jvxIFFTfwk** hdlOut,
				  jvxFftTools_fwkType,
				  jvxInt32 framesize,
				  jvxInt32 buffersize,
				  jvxIFFT* coreIfft,
				  jvxDataCplx** inputComplex_oldWeights,
				  jvxDataCplx** inputComplex_newWeights,
				  jvxData** outputReal,
				  jvxInt32* numElements_input,
				  jvxInt32* numElements_output,
				  jvxData* delay);

//! Allocate the FFT analysis handle. It is like a SW_OLA FFT but no window function allocated
jvxDspBaseErrorType
jvx_fft_tools_init_framework_fft_analysis(jvxFFTfwk** hdlOut,
					  jvxInt32 framesize,
					  jvxFFT* hdlCoreFft,
					  jvxData** inputReal,
					  jvxDataCplx** outputComplex,
					  jvxInt32* numElements_input,
					  jvxInt32* numElements_output);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DEALLOCATION OF FFT OR IFFT PART
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Deallocate FFT handle object, framework type is derived from handles..
jvxDspBaseErrorType
jvx_fft_tools_terminate_framework_fft(jvxFFTfwk* hdl);

//! Deallocate IFFT handle object, framework type is derived from handles..
jvxDspBaseErrorType
jvx_fft_tools_terminate_framework_ifft(jvxIFFTfwk* hdl);

//! Deallocate FFT handle object, framework type is derived from handles..
jvxDspBaseErrorType
jvx_fft_tools_terminate_framework_fft_analysis(jvxFFTfwk* hdl);

// =======================================================================================

//! Process the FFT part of a framework. Data is expected to be filled to buffers by application in advance
jvxDspBaseErrorType
jvx_fft_tools_process_framework_fft(jvxFFTfwk* hdl);

//! Process the IFFT part of a framework. Data is expected to be filled to buffers by application in advance
jvxDspBaseErrorType
jvx_fft_tools_process_framework_ifft(jvxIFFTfwk* hdl);

//! Process the FFT part of a framework for analysis. Data is expected to be filled to buffers by application in advance
jvxDspBaseErrorType
jvx_fft_tools_process_framework_fft_analysis(jvxFFTfwk* hdl,
					     jvxData* window,
					     jvxUInt32 numSamplesValidWindow);




// =======================================================================================
// HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS
// =======================================================================================

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ANTI ALIASING ANTI ALIASING ANTI ALIASING ANTI ALIASING ANTI ALIASING ANTI ALIASING
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Prepare a handle that holds FFT and IFFT kernel as well as the input and output buffers.
jvxDspBaseErrorType
jvx_fft_tools_init_modify_weights_no_aliasing(jvxFFTweightMod** hdlOut,
					      jvxFFT* coreFft,
					      jvxIFFT* coreIfft,
					      jvxDataCplx** input,
					      jvxDataCplx** output,
					      jvxInt32* numElements_inputoutput,
					      jvxInt32 desiredNumFilterCoefficients,
					      jvx_windowType winType);

//! Deallocate handle object
jvxDspBaseErrorType
jvx_fft_tools_terminate_modify_weights_no_aliasing(jvxFFTweightMod* hdl);

//! Postprocessing of computed weights in FFT processing applications to avoid aliasing
jvxDspBaseErrorType
jvx_fft_tools_process_modify_weights_no_aliasing(jvxFFTweightMod* hdl);

// =======================================================================================

#endif
