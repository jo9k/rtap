#ifndef __JVX_BASE_DSP_TYPES_IOS_H__
#define __JVX_BASE_DSP_TYPES_IOS_H__

#include <sys/types.h>
#include <inttypes.h>

//! Size datatype to be used within RTProc
typedef size_t jvxSize;

//! 8 Bit = 1 Byte datatype
typedef char jvxByte;

//! Conventional 32 bit data with sign
typedef int32_t jvxInt32;

//! Conventional 32 bit data without sign
typedef uint32_t jvxUInt32;

//! Signed 16 bit datatype
typedef int16_t jvxInt16;

//! Signed 16 bit datatype
typedef uint16_t jvxSize16;

//! Unsigned 16 bit datatype
typedef uint16_t jvxUInt16;

//! Signed 64 bit datatype
typedef int64_t jvxInt64;

//! Unsigned 64 bit datatype
typedef uint64_t jvxUInt64;

//! Signed 8 bit datatype
typedef int8_t jvxInt8;

//! Unsigned 8 bit datatype
typedef uint8_t jvxUInt8;

//! Double precision floatdata format (64 bit float)
typedef float jvxDouble;

//! Single precision floatdata format (64 bit float)
typedef float jvxFloat;

//! Bool variable
#ifdef __cplusplus
typedef bool jvxBool;
#else
typedef int jvxBool;
#define true 1
#define false 0
#endif

//! Variable type for every unspecific data types.
typedef void jvxHandle;

//! Unsigned 64 bit datatype
typedef unsigned long jvxBitField;


#endif
