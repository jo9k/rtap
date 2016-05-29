#ifndef __JVX_BLOCK_MUSICALTONES_H__
#define __JVX_BLOCK_MUSICALTONES_H__

JVX_DSP_LIB_BEGIN

#include "jvx_dsp_base_types.h"

jvxDspBaseErrorType jvx_block_musicalTones_process(jvxData* bufInOut, jvxData* bufStoreSmoothedVersion,
	jvxData* bufFwdBwdHeadroom, int bufLInOut,
	jvxData coeff, jvxData corrPlus, jvxData corrMinus);

JVX_DSP_LIB_END

#endif
