#ifndef __JVX_MVDRELKO__H__
#define __JVX_MVDRELKO__H__

#include "jvx_dsp_base_types.h"
#include "jvx_diffArray/jvx_diffArray.h"
#include "jvx_fft_tools/jvx_fftFramework.h"
#include "jvx_powerAlignment/jvx_powerAlignment.h"

JVX_DSP_LIB_BEGIN

#define JVX_MVDRELKO_DESCRIPTION "Javox MVDR Elko array"
#define JVX_MVDRELKO_VERSION "0.1"

// FIXME: make these parameters configurable ...
#define JVX_MVDRELKO_FFT_FRAMEWORK_BUFFERSIZE_FACTOR 2 // only relevant for SW_OLA and DW_OLA
#define JVX_MVDRELKO_FFT_FRAMEWORK_TYPE JVX_FFT_FRAMEWORK_OLS

// Configuration parameters which are constant during runtime.
typedef struct {
  int frameSize;
  int samplingRate;
  int nChannelsIn;
} jvx_mvdrElko_cfg_const;

// Configuration parameters which can be changed anytime
typedef struct  {
  jvxInt16 useManualAngleControlUndesired;
  jvxInt16 useAutomaticSteeringDesired;
  jvxInt16 useAutomaticSteeringUndesired;
  jvxData manualSteeringAngleDesired; // in degrees
  jvxData manualSteeringAngleUndesired; // in degrees
  jvxInt16 useOutputHP;
  jvxInt16 useInputHP;
  jvxData maxGainGSC_dB;
  jvxData nlms_ax;
  jvxData nlms_ae;
  jvxData nlms_floor;
  jvxData nlms_xi;
  jvxData nlms_kappa;
} jvx_mvdrElko_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {
  int dummy;
} jvx_mvdrElko_prmSync;


// debug struct
typedef struct {

  unsigned int frameCounter;
  unsigned int id;

  jvxData steeringAngleDesired;

  char *description;
  char *version;

  jvxBool isShadowArray;

} jvx_mvdrElko_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_mvdrElko_cfg_const cnst;
  jvx_mvdrElko_prmSync prmSync;
  jvx_mvdrElko_prm prm;
  jvx_mvdrElko_debug debug;

  // public submodules
  jvx_diffArray diffArray;
  jvx_fftFramework fftFramework;
  jvx_powerAlign powerAlign;

} jvx_mvdrElko;



jvxDspBaseErrorType jvx_mvdrElko_configInit(jvx_mvdrElko* hdl);

jvxDspBaseErrorType jvx_mvdrElko_init(jvx_mvdrElko* hdl,
                                      int frameSize,
                                      int nChannelsIn,
                                      int samplingRate);

jvxDspBaseErrorType jvx_mvdrElko_process(jvx_mvdrElko* hdl,
                                         jvxData** in,
                                         jvxData* out);

// callback for producing the beampattern from
// the functions provided by jvx_beampattern.c
jvxDspBaseErrorType jvx_mvdrElko_processOneFrameShadow(jvxHandle* opaque,
                                                       int frameSize,
                                                       int nChannelsIn,
                                                       int samplingRate,
                                                       jvxData** in,
                                                       jvxData* out);

jvxDspBaseErrorType jvx_mvdrElko_terminate(jvx_mvdrElko* hdl);


jvxDspBaseErrorType jvx_mvdrElko_update(jvx_mvdrElko* hdl,
                                        jvxBool syncUpdate);

JVX_DSP_LIB_END

#endif
