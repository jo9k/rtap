#ifndef __JVX_BASE_DSP_TYPES_WINDOWS_H__
#define __JVX_BASE_DSP_TYPES_WINDOWS_H__

#include <windows.h>

//! Size datatype to be used within RTProc
typedef  size_t jvxSize;

//! 8 Bit = 1 Byte datatype
typedef char jvxByte;

//! Conventional 32 bit data with sign
typedef signed __int32 jvxInt32;

//! Conventional 32 bit data without sign
typedef unsigned __int32 jvxUInt32;

//! Signed 16 bit datatype
typedef signed __int16 jvxInt16;

//! Signed 16 bit datatype
typedef unsigned __int16 jvxSize16;

//! Unsigned 16 bit datatype
typedef unsigned __int16 jvxUInt16;

//! Signed 64 bit datatype
typedef signed __int64 jvxInt64;

//! Unsigned 64 bit datatype
typedef unsigned __int64 jvxUInt64;

//! Signed 8 bit datatype
typedef signed __int8 jvxInt8;

//! Unsigned 8 bit datatype
typedef unsigned __int8 jvxUInt8;

//! Double precision floatdata format (64 bit float)
//typedef double jvxDouble;

//! Single precision floatdata format (64 bit float)
//typedef float jvxFloat;

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
typedef unsigned __int32 jvxBitField;

#ifndef __cplusplus
// It seems that defining this macro causes many problems in C++
#define inline __inline
#endif

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#ifndef isnan
#define isnan _isnan
#endif

#endif
