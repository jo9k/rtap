#ifndef __JVX_BEAMPATTERN__H__
#define __JVX_BEAMPATTERN__H__

#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN


#define JVX_BEAMPATTERN_DESCRIPTION "Javox beampattern simulator"
#define JVX_BEAMPATTERN_VERSION "1.0"

// Configuration parameters which are constant during runtime.
typedef struct {
  int frameSize;
  int samplingRate;

} jvx_beampattern_cfg_const;

// Configuration parameters which can be changed anytime
typedef struct  {
  void *dummy;
} jvx_beampattern_prm;


// Configuration parameters which can be changed between buffer processing
typedef struct  {
  jvxInt16 numAzimuthAngles;
  jvxInt16 numElevationAngles;
  jvxInt16 fftLengthLog2;
  jvxData radiusCircleAround;
  jvxInt16 fracDelayFilterDelay;
  jvxData azimuthMin;
  jvxData azimuthMax;
  jvxData elevationMin;
  jvxData elevationMax;
  jvxBool removePropagationToArray;
  jvxData speedOfSound;
} jvx_beampattern_prmSync;


// debug struct
typedef struct {

  int frameCounter;
  int id;

  jvxInt16 spectrumSize;

} jvx_beampattern_debug;


// main struct
typedef struct {

  jvxHandle* prv;
  jvx_beampattern_cfg_const cnst;
  jvx_beampattern_prmSync prmSync;
  jvx_beampattern_prm prm;
  jvx_beampattern_debug debug;

} jvx_beampattern;



// additional typdefs
typedef jvxDspBaseErrorType (*jvx_beampattern_process_cb)(jvxHandle* hdl,
                                                          int frameSize,
                                                          int nChannelsIn,
                                                          int samplingRate,
                                                          jvxData** in,
                                                          jvxData* out);

jvxDspBaseErrorType jvx_beampattern_configInit(jvx_beampattern* hdl);

jvxDspBaseErrorType jvx_beampattern_init(jvx_beampattern* hdl,
                                         int samplingRate,
                                         jvxHandle *bf_hdl,
                                         jvx_beampattern_process_cb bf_cb);

/*
 * Coordinate system:
 *
 *
 *                         | +z (90°/undefined)
 *                         |
 *                         |     (0°/0°)
 *                         |    / +y
 *                         |   /
 *                         |  /
 *                         | /
 *                         | ---------------- +x (0°/270°)
 */
jvxDspBaseErrorType jvx_beampattern_addMicrophone(jvx_beampattern* hdl,
                                                  jvxData x,
                                                  jvxData y,
                                                  jvxData z,
                                                  jvxSize *id);

jvxDspBaseErrorType jvx_beampattern_removeAllMicrophones(jvx_beampattern* hdl);

jvxDspBaseErrorType jvx_beampattern_removeMicrophone(jvx_beampattern* hdl,
                                                     jvxSize id);

jvxDspBaseErrorType jvx_beampattern_printMicrophones(jvx_beampattern* hdl);

jvxDspBaseErrorType jvx_beampattern_process(jvx_beampattern* hdl,
                                            jvxData **out);

jvxDspBaseErrorType jvx_beampattern_terminate(jvx_beampattern* hdl);


jvxDspBaseErrorType jvx_beampattern_update(jvx_beampattern* hdl,
                                           jvxBool syncUpdate);

// data access
jvxData* jvx_beampattern_getOutputPointer(jvx_beampattern *hdl);

JVX_DSP_LIB_END

#endif
