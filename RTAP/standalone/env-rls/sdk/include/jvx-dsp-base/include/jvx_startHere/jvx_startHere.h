#ifndef __JVX_STARTHERE__H__
#define __JVX_STARTHERE__H__

#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN

#define JVX_STARTHERE_DESCRIPTION "Javox Demo Algorithm"
#define JVX_STARTHERE_VERSION "0.1"

// Configuration parameters which can be changed anytime
typedef struct  {
  jvxData gain_dB;
  jvxInt16 flip;
} jvx_startHere_prm;

// Configuration parameters which can be changed between buffer processing
typedef struct  {
  int dummy;
} jvx_startHere_prmSync;

// debug struct
typedef struct {
  unsigned int frameCounter;
} jvx_startHere_debug;


// main struct
typedef struct {

  // constant parameters
  unsigned int id;
  char *description;
  char *version;
  int frameSize;
  int samplingRate;
  int nChannelsIn;
  int nChannelsOut;

  // sub-structs
  jvxHandle* prv;
  jvx_startHere_prmSync prmSync;
  jvx_startHere_prm prm;
  jvx_startHere_debug debug;

  // public submodules
  // ... none ...

} jvx_startHere;



jvxDspBaseErrorType
jvx_startHere_configInit(jvx_startHere* hdl);

jvxDspBaseErrorType
jvx_startHere_init(jvx_startHere* hdl,
                   int frameSize,
                   int nChannelsIn,
                   int nChannelsOut,
                   int samplingRate);

jvxDspBaseErrorType
jvx_startHere_process(jvx_startHere* hdl,
                      jvxData** in,
                      jvxData** out);

jvxDspBaseErrorType
jvx_startHere_terminate(jvx_startHere* hdl);


jvxDspBaseErrorType
jvx_startHere_update(jvx_startHere* hdl,
                     jvxBool syncUpdate);

JVX_DSP_LIB_END

#endif
