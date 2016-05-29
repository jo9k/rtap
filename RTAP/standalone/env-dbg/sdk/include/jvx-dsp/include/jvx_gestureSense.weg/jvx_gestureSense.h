#ifndef __JVX_GESTURESENSE__H__
#define __JVX_GESTURESENSE__H__

#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN

#define JVX_GESTURESENSE_DESCRIPTION "Javox gesture sensing"
#define JVX_GESTURESENSE_VERSION "1.0"

// Configuration parameters which are constant during runtime.
typedef struct {
  int frameSize;
  int samplingRate;
  int nChannelsIn;
} jvx_gestureSense_cfg_const;

// Configuration parameters which can be changed anytime
typedef struct  {
  jvxInt16 dummy;
  //jvxInt16 useOutputEQ;
  //jvxInt16 useInputEQ;
} jvx_gestureSense_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {
  jvxInt16 dummy;
  //jvxInt16 useKalmanFilter;
} jvx_gestureSense_prmSync;


// debug struct
typedef struct {

  int frameCounter;
  int id;

  //jvxData steeringAngle;

  char *description;
  char *version;

} jvx_gestureSense_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_gestureSense_cfg_const cnst;
  jvx_gestureSense_prmSync prmSync;
  jvx_gestureSense_prm prm;
  jvx_gestureSense_debug debug;

} jvx_gestureSense;



jvxDspBaseErrorType jvx_gestureSense_configInit(jvx_gestureSense* hdl,
                                             int id);

jvxDspBaseErrorType jvx_gestureSense_init(jvx_gestureSense* hdl,
                                       int frameSize,
                                       int nChannelsIn,
                                       int samplingRate);

jvxDspBaseErrorType jvx_gestureSense_process(jvx_gestureSense* hdl,
                                          jvxData** in,
                                          jvxData* out);

jvxDspBaseErrorType jvx_gestureSense_terminate(jvx_gestureSense* hdl);


jvxDspBaseErrorType jvx_gestureSense_update(jvx_gestureSense* hdl,
                                         jvxBool syncUpdate);

JVX_DSP_LIB_END

#endif
