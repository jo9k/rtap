#ifndef __JVX_DIFFARRAY__H__
#define __JVX_DIFFARRAY__H__

#include "jvx_dsp_base_types.h"
#include "jvx_powerAlignment/jvx_powerAlignment.h"

JVX_DSP_LIB_BEGIN

#define JVX_DIFFARRAY_DESCRIPTION "Javox differential microphone array"
#define JVX_DIFFARRAY_VERSION "1.0"

// Configuration parameters which are constant during runtime.
typedef struct {
  int frameSize;
  int samplingRate;
  int nChannelsIn;
} jvx_diffArray_cfg_const;

// Configuration parameters which can be changed anytime
typedef struct  {
  jvxInt16 useInputHP;
  jvxInt16 useOutputHP;
  jvxInt16 useDEQ;
  jvxInt16 deqFadeOverLength; // fadeover in samples for factor in directional equalizer
  jvxData nlmsKappa;
  jvxData nlmsXi;
  jvxData nlms_ax;
  jvxData nlms_ae;
  jvxData nlmsFloor;
  jvxInt16 useAutomaticSteering;
  jvxData maxSteeringAngle; // in degrees
  jvxData minSteeringAngle; // in degrees
  jvxData manualSteeringAngle; // in degrees
  jvxData outputFIRHPcoeff;
  jvxInt16 useOutputEQ;
} jvx_diffArray_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {
  jvxInt16 fracDelayFilterDelay;
} jvx_diffArray_prmSync;


// debug struct
typedef struct {

  int frameCounter;
  int id;

  jvxData steeringAngle;

  char *description;
  char *version;

  jvxBool isShadowArray;

} jvx_diffArray_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_diffArray_cfg_const cnst;
  jvx_diffArray_prmSync prmSync;
  jvx_diffArray_prm prm;
  jvx_diffArray_debug debug;

  // public submodules
  jvx_powerAlign powerAlign;

} jvx_diffArray;



jvxDspBaseErrorType jvx_diffArray_configInit(jvx_diffArray* hdl);

jvxDspBaseErrorType jvx_diffArray_init(jvx_diffArray* hdl,
                                       int frameSize,
                                       int nChannelsIn,
                                       int samplingRate);

jvxDspBaseErrorType jvx_diffArray_process(jvx_diffArray* hdl,
                                          jvxData** in,
                                          jvxData* out);

// callback for producing the beampattern from
// the functions provided by jvx_beampattern.c
jvxDspBaseErrorType jvx_diffArray_processOneFrameShadow(jvxHandle* opaque,
                                                        int frameSize,
                                                        int nChannelsIn,
                                                        int samplingRate,
                                                        jvxData** in,
                                                        jvxData* out);

jvxDspBaseErrorType jvx_diffArray_terminate(jvx_diffArray* hdl);


jvxDspBaseErrorType jvx_diffArray_update(jvx_diffArray* hdl,
                                         jvxBool syncUpdate);


// data access functions
jvxData jvx_diffArray_get_a(jvx_diffArray* hdl);
jvxInt32 jvx_diffArray_getDelay(jvx_diffArray* hdl);

JVX_DSP_LIB_END

#endif
