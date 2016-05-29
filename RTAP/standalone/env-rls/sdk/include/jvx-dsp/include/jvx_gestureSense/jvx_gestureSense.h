#ifndef __JVX_GESTURESENSE__H__
#define __JVX_GESTURESENSE__H__

#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN

#define JVX_GESTURESENSE_DESCRIPTION "Javox gesture sensing"
#define JVX_GESTURESENSE_VERSION "1.0"

#define JVX_GESTURE_FRAMELENGTH 720 // ethernet frame length
#define JVX_GESTURE_TX_LENGTH 1320 // tx signal length is ~2 frames
#define JVX_GESTURE_RX_LENGTH (JVX_GESTURE_FRAMELENGTH*3) // rx signal length is 3 frames
#define JVX_GESTURE_BLOCKLENGTH (JVX_GESTURE_FRAMELENGTH*4) // tx: 2 send + 2 silence, rx: 1 dummy + 3 receive
#define JVX_GESTURE_FFTLENGTH 4096
#define JVX_GESTURE_DELAY 50 // approximate 50 samples delay in loopback (DA + AD + CIC + 2x McASP)
#define JVX_GESTURE_MIN_DIST 56 // minimum distance to hand is 5cm = 56 samples

#define JVX_GESTURE_TX_F0 30e3 // sweep from 30kHz ..
#define JVX_GESTURE_TX_F1 50e3 // ..to 50kHz

// TODO move to jvx_gestureSense_defaultConfig.h
#define JVX_GESTURESENSE_SAMPLERATE 192000
#define JVX_GESTURESENSE_N_CHANNELS_IN 6 // 3
#define JVX_GESTURESENSE_N_CHANNELS_OUT 2 // 1

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

#include "jvx_gestureSense_prv.h"

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

jvxDspBaseErrorType jvx_gestureSense_process_in(jvx_gestureSense* hdl,
                                          jvxData** in,
                                          jvxData* externPlotBuffer);

jvxDspBaseErrorType jvx_gestureSense_process_out(jvx_gestureSense* hdl,
                                          jvxData* out);

jvxDspBaseErrorType jvx_gestureSense_terminate(jvx_gestureSense* hdl);


jvxDspBaseErrorType jvx_gestureSense_update(jvx_gestureSense* hdl,
                                         jvxBool syncUpdate);

JVX_DSP_LIB_END

#endif
