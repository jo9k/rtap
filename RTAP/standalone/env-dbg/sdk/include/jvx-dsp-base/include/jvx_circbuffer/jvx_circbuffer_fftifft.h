#ifndef __JVX_CIRCBUFFERFFT_H__
#define __JVX_CIRCBUFFERFFT_H__

#include "jvx_dsp_base.h"
#include "jvx_circbuffer/jvx_circbuffer.h"
#include "jvx_fft_tools/jvx_fft_core.h"
#include "jvx_fft_tools/jvx_fft_core_typedefs.h"

typedef struct
{
	jvx_circbuffer circBuffer;

	jvxFftTools_coreFftType fftType;
	jvxFFTSize fftSize;

	jvxHandle* coreHdl;
	jvxDataCplx* cplxBuf;
	jvxInt32 lengthCplxBuffer;
} jvx_circbuffer_fft;

jvxDspBaseErrorType
jvx_circbuffer_allocate_global_fft_ifft(jvxHandle** global_fft,
					jvxFFTSize fftType_max);

jvxDspBaseErrorType
jvx_circbuffer_destroy_global_fft_ifft(jvx_fft_ifft_core_global_common* global_fft);

jvxDspBaseErrorType
jvx_circbuffer_allocate_fft_ifft(jvx_circbuffer_fft** hdlOnReturn,
				 jvx_fft_ifft_core_global_common* hdl_global_fft,
				 jvxFftTools_coreFftType fftType,
				 jvxFFTSize fftSize,
				 jvxBool preserveInput);

jvxDspBaseErrorType
jvx_circbuffer_deallocate_fft_ifft(jvx_circbuffer_fft* hdlReturn);

jvxDspBaseErrorType
jvx_circbuffer_access_cplx_fft_ifft(jvx_circbuffer_fft* hdl, jvxDataCplx** outPtr);

jvxDspBaseErrorType
jvx_circbuffer_get_read_phase_fft_ifft(jvx_circbuffer_fft* hdl, jvxInt32* phase);

jvxDspBaseErrorType
jvx_circbuffer_set_read_phase_fft_ifft(jvx_circbuffer_fft* hdl, jvxInt32 phase);

jvxDspBaseErrorType
jvx_circbuffer_get_write_phase_fft_ifft(jvx_circbuffer_fft* hdl, jvxInt32* phase);

jvxDspBaseErrorType
jvx_circbuffer_process_fft_ifft(jvx_circbuffer_fft* hdl);

#endif
