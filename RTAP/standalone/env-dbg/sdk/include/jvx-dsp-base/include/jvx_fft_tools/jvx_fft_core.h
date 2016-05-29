#ifndef __JVX_FFT_CORE__H__
#define __JVX_FFT_CORE__H__

//#include "jvx_fft_tools/jvx_fft_tools.h"
#include "jvx_dsp_base.h"
#include "jvx_dsp_base_types.h"

// =====================================================================


typedef jvxHandle jvxFFTGlobal;
typedef jvxHandle jvxFFT;
typedef jvxHandle jvxIFFT;

typedef enum
{
	JVX_FFT_IFFT_PRESERVE_INPUT,
	JVX_FFT_IFFT_EFFICIENT
} jvxFftIfftOperate;

//! Typedefs for FFT sizes, only power of 2 allowed.
typedef enum
{
	JVX_FFT_TOOLS_FFT_SIZE_16 = 0,
	JVX_FFT_TOOLS_FFT_SIZE_32,
	JVX_FFT_TOOLS_FFT_SIZE_64,
	JVX_FFT_TOOLS_FFT_SIZE_128,
	JVX_FFT_TOOLS_FFT_SIZE_256,
	JVX_FFT_TOOLS_FFT_SIZE_512,
	JVX_FFT_TOOLS_FFT_SIZE_1024,
	JVX_FFT_TOOLS_FFT_SIZE_2048,
	JVX_FFT_TOOLS_FFT_SIZE_4096,
	JVX_FFT_TOOLS_FFT_SIZE_8192,
	JVX_FFT_TOOLS_FFT_ARBITRARY_SIZE
} jvxFFTSize;

#define JVX_FFT_TOOLS_DEFINE_FFT_SIZES static jvxInt32 jvxFFTSize_sizes[] = \
{ \
	16, \
	32, \
	64, \
	128, \
	256, \
	512, \
	1024, \
	2048, \
	4096, \
	8192, \
	-1 \
} ;


// =====================================================================

jvxDspBaseErrorType jvx_create_fft_ifft_global(jvxFFTGlobal** global_hdl,
					       jvxFFTSize fftType_max);

jvxDspBaseErrorType jvx_create_fft_real_2_complex(jvxFFT** hdl,
						  jvxFFTGlobal* global_hdl,
						  jvxFFTSize fftType,
						  jvxData** in_ptr_fld_N,
						  jvxDataCplx** out_ptr_fld_N2P1,
						  jvxSize* N,
						  jvxFftIfftOperate operate,
						  jvxData* input,
						  jvxDataCplx* output);

jvxDspBaseErrorType jvx_create_fft_complex_2_complex(jvxFFT** hdl,
						     jvxFFTGlobal* global_hdl,
						     jvxFFTSize fftType,
						     jvxDataCplx** in_ptr_fld_N,
						     jvxDataCplx** out_ptr_fld_N,
						     jvxSize* N,
						     jvxFftIfftOperate operate,
						     jvxDataCplx* input,
						     jvxDataCplx* output);

jvxDspBaseErrorType jvx_create_ifft_complex_2_real(jvxIFFT** hdl,
						   jvxFFTGlobal* global_hdl,
						   jvxFFTSize fftType,
						   jvxDataCplx** in_ptr_fld_N2P1,
						   jvxData** out_ptr_fld_N,
						   jvxSize* N,
						   jvxFftIfftOperate operate,
						   jvxDataCplx* input,
						   jvxData* output);

jvxDspBaseErrorType jvx_create_ifft_complex_2_complex(jvxIFFT** hdl,
						      jvxFFTGlobal* global_hdl,
						      jvxFFTSize fftType,
						      jvxDataCplx** in_ptr_fld_N,
						      jvxDataCplx** out_ptr_fld_N,
						      jvxSize* N,
						      jvxFftIfftOperate operate,
						      jvxDataCplx* input,
						      jvxDataCplx* output);

jvxDspBaseErrorType jvx_execute_fft(jvxFFT* hdl);
jvxDspBaseErrorType jvx_execute_ifft(jvxIFFT* hdl);

jvxDspBaseErrorType jvx_destroy_fft(jvxFFT* hdl);
jvxDspBaseErrorType jvx_destroy_ifft(jvxIFFT* hdl);

jvxDspBaseErrorType jvx_destroy_fft_ifft_global(jvxFFTGlobal* global_hdl);


jvxFFTSize
jvx_fft_tools_get_nearest_size(jvxSize n);

#endif
