#ifndef __JVX_SPECTRUM_ESTIMATION_H__
#define __JVX_SPECTRUM_ESTIMATION_H__

#include "jvx_dsp_base_types.h"
#include "jvx_windows/jvx_windows.h"

JVX_DSP_LIB_BEGIN

// Configuration parameters which are constant during runtime.
typedef struct {

  int frameSize;
  int samplingRate;
  int nChannelsIn;

} jvx_spectrumEstimation_cfg_const;


// additional types for parameters
enum {
  jvx_spectrumEstimation_welchRecursive,
  jvx_spectrumEstimation_instantaneous,
  //other ideas:
  // jvx_spectrumEstimation_welchMovingAverage
  // jvx_spectrumEstimation_lpc
};
typedef jvxInt16 jvx_spectrumEstimationMethod;

// Configuration parameters which can be changed anytime
typedef struct  {
	// Required for compilation in Windows
	int dummy;
} jvx_spectrumEstimation_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {

  jvx_spectrumEstimationMethod method;
  jvxInt16 logarithmic;
  jvxInt16 fftLengthLog2;
  jvx_windowType windowType;
  jvxData winArgA;
  jvxData winArgB;

  // parameters for welchRecursive method
  jvxData alpha;

  // other spectrum estimation methods tbd.

} jvx_spectrumEstimation_prmSync;


// debug struct
typedef struct {

  int frameCounter;
  int id;

  jvxInt16 spectrumSize;

} jvx_spectrumEstimation_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_spectrumEstimation_cfg_const cnst;
  jvx_spectrumEstimation_prmSync prmSync;
  jvx_spectrumEstimation_prm prm;
  jvx_spectrumEstimation_debug debug;

} jvx_spectrumEstimation;


////
// API
////

jvxDspBaseErrorType jvx_spectrumEstimation_configInit(jvx_spectrumEstimation *hdl);

jvxDspBaseErrorType jvx_spectrumEstimation_init(jvx_spectrumEstimation* hdl,
                                                int frameSize,
                                                int nChannelsIn,
                                                int samplingRate);

// input: frameSize
// output: pointer to (fftLength/2+1)
jvxDspBaseErrorType jvx_spectrumEstimation_process(jvx_spectrumEstimation* hdl,
                                                   jvxData* in,
                                                   jvxData** out,
                                                   jvxSize channel);

jvxDspBaseErrorType jvx_spectrumEstimation_terminate(jvx_spectrumEstimation* hdl);


jvxDspBaseErrorType jvx_spectrumEstimation_update(jvx_spectrumEstimation* hdl,
                                                  jvxBool syncUpdate);

// data access
jvxData* jvx_spectrumEstimation_getOutputPointer(jvx_spectrumEstimation *hdl);

JVX_DSP_LIB_END

#endif
