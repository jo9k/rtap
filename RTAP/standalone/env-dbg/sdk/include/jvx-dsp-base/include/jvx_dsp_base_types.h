#ifndef __JVX_BASE_DSP_TYPES_H__
#define __JVX_BASE_DSP_TYPES_H__


#include <float.h>
#include "jvx_dsp_base.h"
#include "jvx_dsp_base_types__os.h"
#include "jvx_helpers.h"


JVX_DSP_LIB_BEGIN

#ifdef JVX_DSP_DATA_FORMAT_DOUBLE

typedef double jvxData;
#define JVX_DBL_2_DATA(a) (a)
#define JVX_DATA_MAX_POS DBL_MAX
#define JVX_DATA_MAX_NEG -DBL_MAX
#define JVX_DATA_DESCRIPTOR "double"
#define JVX_DATA_DESCRIPTOR_THE_OTHER "single"
#define toData toDouble
#define JVX_DATA_MEX_CONVERT mxDOUBLE_CLASS
#define mxIsData mxIsDouble
#define sf_writef_data sf_writef_double
#define sf_readf_data sf_readf_double
#define JVX_FLOAT_DATAFORMAT_ID 1

#else // JVX_DSP_DATA_FORMAT_DOUBLE

#ifdef JVX_DSP_DATA_FORMAT_FLOAT

typedef float jvxData;

#define JVX_DBL_2_DATA(a) (float)(a)
#define JVX_DATA_MAX_POS FLT_MAX
#define JVX_DATA_MAX_NEG -FLT_MAX
#define JVX_DATA_DESCRIPTOR "single"
#define JVX_DATA_DESCRIPTOR_THE_OTHER "double"
#define toData toFloat
#define JVX_DATA_MEX_CONVERT mxSINGLE_CLASS
#define mxIsData mxIsSingle
#define sf_writef_data sf_writef_float
#define sf_readf_data sf_readf_float
#define JVX_FLOAT_DATAFORMAT_ID 0

#else // JVX_DSP_DATA_FORMAT_FLOAT

#error("unkown arithmetic datatype")

#endif // JVX_DSP_DATA_FORMAT_FLOAT
#endif // JVX_DSP_DATA_FORMAT_DOUBLE

typedef enum
{
        JVX_DSP_NO_ERROR = 0,
        JVX_DSP_ERROR_UNKNOWN,
        JVX_DSP_ERROR_INVALID_ARGUMENT,
        JVX_DSP_ERROR_WRONG_STATE,
        /* JVX_ERROR_ID_OUT_OF_BOUNDS, */
        JVX_DSP_ERROR_UNSUPPORTED,
        /* JVX_ERROR_ELEMENT_NOT_FOUND, */
        /* JVX_ERROR_DUPLICATE_ENTRY, */
        /* JVX_ERROR_OPEN_FILE_FAILED, */
        /* JVX_ERROR_UNEXPECTED, */
        /* JVX_ERROR_NOT_READY, */
        /* JVX_ERROR_COMPONENT_BUSY, */
        JVX_DSP_ERROR_BUFFER_OVERFLOW,
        /* JVX_ERROR_BUFFER_UNDERRUN, */
        /* JVX_ERROR_WRONG_STATE_SUBMODULE, */
        /* JVX_ERROR_MESSAGE_QUEUE_FULL, */
        /* JVX_ERROR_PARSE_ERROR, */
        /* JVX_ERROR_WRONG_SECTION_TYPE, */
        JVX_DSP_ERROR_INTERNAL,
        /* JVX_ERROR_EMPTY_LIST, */
        /* JVX_ERROR_CALL_SUB_COMPONENT_FAILED, */
        /* JVX_ERROR_THREADING_MISMATCH, */
        /* JVX_ERROR_SYSTEMCALL_FAILED, */
        /* JVX_ERROR_SIZE_MISMATCH, */
        /* JVX_ERROR_READ_ONLY, */
        JVX_DSP_ERROR_INVALID_SETTING,
        /* JVX_ERROR_TIMEOUT, */
        /* JVX_ERROR_ABORT, */
        /* JVX_ERROR_POSTPONE, */
        /* JVX_ERROR_END_OF_FILE, */
        /* JVX_ERROR_STANDARD_EXCEPTION, */
        /* JVX_ERROR_SEQUENCER_TOOK_CONTROL, */
        /* JVX_ERROR_THREAD_ONCE_MORE, */
        /* JVX_ERROR_PROCESS_COMPLETE, */

        JVX_DSP_ERROR_LIMIT
} jvxDspBaseErrorType;

//! Shortcut for complex datatypes, actually defined to support fftw
typedef struct
{
	jvxData re;
	jvxData im;
} jvxDataCplx;


/////////////////////////
// numerical constants
/////////////////////////

#define JVX_MAX_INT_64 ((double)0x7FFFFFFFFFFFFFFF)
#define JVX_MAX_INT_32 ((double)0x7FFFFFFF)
#define JVX_MAX_INT_24 ((double)0x7FFFFF)
#define JVX_MAX_INT_16 ((double)0x7FFF)
#define JVX_MAX_INT_8 ((double)0x7F)
#define JVX_MAX_INT_64_DIV ((double)1.0/(double)JVX_MAX_INT_64)
#define JVX_MAX_INT_32_DIV ((double)1.0/(double)JVX_MAX_INT_32)
#define JVX_MAX_INT_24_DIV ((double)1.0/(double)JVX_MAX_INT_24)
#define JVX_MAX_INT_16_DIV ((double)1.0/(double)JVX_MAX_INT_16)
#define JVX_MAX_INT_8_DIV ((double)1.0/(double)JVX_MAX_INT_8)

#ifndef M_PI
#define M_PI (jvxData)(3.14159265358979323846264338327950288)
#endif

#ifdef JVX_DSP_DATA_FORMAT_DOUBLE
static jvxData mat_epsMin = 2.220446049250313100000000000000e-016;
static jvxData mat_epsMax = 1.797693134862315700000000000000e+308;
#else
static jvxData mat_epsMin = JVX_DBL_2_DATA(1.192092895507812500000000000000e-007);
static jvxData mat_epsMax = JVX_DBL_2_DATA(3.402823466385288600000000000000e+038);
#endif

#define JVX_DATA_2_DATA(in) in
#define JVX_DATA_2_FLOAT(in) ((float)in)
#define JVX_DATA_2_DOUBLE(in) ((double)in)
#define JVX_FLOAT_2_DATA(in) ((jvxData)in)
#define JVX_DOUBLE_2_DATA(in) ((jvxData)in)
#define JVX_INT32_2_DATA(in) ((jvxData)in * JVX_MAX_INT_32_DIV)
#define JVX_INT16_2_DATA(in) ((jvxData)in * JVX_MAX_INT_16_DIV)
#define JVX_INT64_2_DATA(in) ((jvxData)in * JVX_MAX_INT_64_DIV)
#define JVX_INT8_2_DATA(in) ((jvxData)in * JVX_MAX_INT_8_DIV)

static inline jvxInt64
JVX_DATA2INT64(jvxData a)
{
	return (jvxInt64)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxInt64
JVX_DDOUBLE2INT64(double a)
{
	return (jvxInt64)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxInt64
JVX_DFLOAT2INT64(float a)
{
	return (jvxInt64)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxBitField
JVX_DATA2BITFIELD(jvxData a)
{
	return (jvxBitField)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxInt32
JVX_DATA2INT32(jvxData a)
{
	return (jvxInt32)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxInt16
JVX_DATA2INT16(jvxData a)
{
	return (jvxInt16)((a)+JVX_SIGN(a)*0.5);
}

static inline jvxInt8
JVX_DATA2INT8(jvxData a)
{
	return (jvxInt8)((a)+JVX_SIGN(a)*0.5);
}

static inline void
JVX_DATA2INT32Q(jvxData input, jvxInt32 Q, jvxInt32 *output)
{
	jvxData multCoeff = (jvxData)((long long)1<<Q);
	jvxData maxVal = (long long)1<<31;

	input *= multCoeff;

	// Apply saturation
	if(input > maxVal)
	{
		input = maxVal;
	}

	if(input < -maxVal)
	{
		input = -maxVal;
	}

	if(input > 0)
	{
		input += 0.5;
	}
	else
	{
		input -= 0.5;
	}

	*output = (jvxInt32)input;

	// For the maximum positive case: Avoid wrap around!!
	if((input > 0 )&&(*output < 0))
	{
		*output = ((long long)1<<31)-1;
	}
}

JVX_DSP_LIB_END

#endif
