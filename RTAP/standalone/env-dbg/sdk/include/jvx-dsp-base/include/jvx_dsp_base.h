#ifndef __JVXL_DSP_BASE_H__
#define __JVXL_DSP_BASE_H__

/* #undef JVX_DSP_BASE_USE_DATA_FORMAT_FLOAT */
#define JVX_VERSION_STRING "iks.rtap.v.1.0-dirty"

#ifdef JVX_DSP_BASE_USE_DATA_FORMAT_FLOAT
#define JVX_DSP_DATA_FORMAT_FLOAT
#else
#define JVX_DSP_DATA_FORMAT_DOUBLE
#endif

#ifdef __cplusplus
#define JVX_DSP_LIB_BEGIN extern "C" {
#define JVX_DSP_LIB_END }
#else
#define JVX_DSP_LIB_BEGIN
#define JVX_DSP_LIB_END
#endif

// for unique module IDs
extern unsigned int jvxID;
unsigned int jvx_id();

#endif
