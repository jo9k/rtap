#ifndef __JVX_WINDOWS_H__
#define __JVX_WINDOWS_H__

#include "jvx_dsp_base_types.h"

JVX_DSP_LIB_BEGIN

//! Typedefs for window function
enum
{
	JVX_WINDOW_RECT = 0,
	JVX_WINDOW_HAMMING = 1,
	JVX_WINDOW_BLACKMAN = 2,
	JVX_WINDOW_FLATTOP_HANN = 3,
	JVX_WINDOW_SQRT_FLATTOP_HANN = 4,
	JVX_WINDOW_HANN = 5,
	JVX_WINDOW_SQRT_HANN = 6,
	JVX_WINDOW_HALF_HANN = 7,
	JVX_WINDOW_HALF_LINEAR = 8,
	JVX_WINDOW_HALF_HAMMING = 9,
	JVX_WINDOW_HAMMING_ASYM = 10,
        JVX_WINDOW_KAISER = 11
};
typedef jvxInt16 jvx_windowType;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WINDOW FUNCTIONS WINDOW FUNCTIONS WINDOW FUNCTIONS WINDOW FUNCTIONS WINDOW FUNCTIONS WINDOW FUNCTIONS
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Compute a window function in the buffer passed. Second arg may be of importance or may not be
jvxDspBaseErrorType
jvx_compute_window(jvxData* ptrField, jvxInt32 fieldSize,
                   jvxData secondArgDbl, jvxData thirdArg,
                   jvx_windowType winType, jvxData* normalization);


JVX_DSP_LIB_END

#endif
