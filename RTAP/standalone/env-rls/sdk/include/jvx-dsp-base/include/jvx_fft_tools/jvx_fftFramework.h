#ifndef __JVX_FFT_FRAMEWORK_H__
#define __JVX_FFT_FRAMEWORK_H__

#include "jvx_dsp_base_types.h"
#include "jvx_windows/jvx_windows.h"
#include "jvx_fft_tools/jvx_fft_core.h"

JVX_DSP_LIB_BEGIN

#define JVX_FFTFRAMEWORK_DESCRIPTION "Javox FFT framework"
#define JVX_FFTFRAMEWORK_VERSION "0.1"

//! Typedefs for FFT framework types
enum
{
	JVX_FFT_FRAMEWORK_SW_OLA = 0,
	JVX_FFT_FRAMEWORK_DW_OLA,
	JVX_FFT_FRAMEWORK_OLA_CF,
	JVX_FFT_FRAMEWORK_OLS,
	JVX_FFT_FRAMEWORK_OLS_CF,
        JVX_FFT_FRAMEWORK_NUMTYPES
};
typedef jvxInt16 jvx_fftFrameworkType;

// Configuration parameters which are constant during runtime.
typedef struct {

  int frameSize;

} jvx_fftFramework_cfg_const;


// Configuration parameters which can be changed anytime
typedef struct  {
	// Required for compilation in Windows
	int dummy;
} jvx_fftFramework_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {
  jvxData bufferSizeFactor; // bufferSize = max(1,bufferSizeFactor) * frameSize
  jvx_fftFrameworkType frameworkType;
  jvxInt16 nFFT;
  jvxInt16 nIFFT;
  jvxInt16 fftLengthLog2;
} jvx_fftFramework_prmSync;


// debug struct
typedef struct {

  int frameCounter;
  int id;

  char *description;
  char *version;

} jvx_fftFramework_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_fftFramework_cfg_const cnst;
  jvx_fftFramework_prmSync prmSync;
  jvx_fftFramework_prm prm;
  jvx_fftFramework_debug debug;

} jvx_fftFramework;

////
// API
////

jvxDspBaseErrorType jvx_fftFramework_configInit(jvx_fftFramework *hdl);

jvxDspBaseErrorType
jvx_fftFramework_init(jvx_fftFramework* hdl,
		      int frameSize);

jvxDspBaseErrorType jvx_fftFramework_processFFT(jvx_fftFramework* hdl,
                                                jvxInt16 fftIdx,
                                                jvxData* in,
                                                jvxDataCplx* out);

jvxDspBaseErrorType jvx_fftFramework_processFFT_ZP(jvx_fftFramework* hdl,
                                                   jvxData* in,
                                                   jvxDataCplx* out);

jvxDspBaseErrorType jvx_fftFramework_processIFFT(jvx_fftFramework* hdl,
                                                 jvxInt16 ifftIdx,
                                                 jvxDataCplx* in,
                                                 jvxDataCplx* weights,
                                                 jvxData* out);

jvxDspBaseErrorType jvx_fftFramework_processAntiAlias(jvx_fftFramework* hdl,
                                                      jvxDataCplx* in,
                                                      jvxDataCplx* out);

jvxDspBaseErrorType jvx_fftFramework_terminate(jvx_fftFramework* hdl);


jvxDspBaseErrorType jvx_fftFramework_update(jvx_fftFramework* hdl,
                                            jvxBool syncUpdate);

jvxInt32 jvx_fftFramework_getDelay(jvx_fftFramework *hdl);

jvxSize jvx_fftFramework_getSpectrumSize(jvx_fftFramework *hdl);

jvxSize jvx_fftFramework_getFFTLength(jvx_fftFramework *hdl);

JVX_DSP_LIB_END


#endif

