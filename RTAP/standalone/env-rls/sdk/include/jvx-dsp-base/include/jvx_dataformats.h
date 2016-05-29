#ifndef __JVX_DATAFORMATS_H__
#define __JVX_DATAFORMATS_H__

#include <string.h>
#include <assert.h>
#include "jvx_dsp_base.h"
#include "jvx_dsp_base_types.h"


JVX_DSP_LIB_BEGIN

typedef enum
{
        JVX_DATAFORMAT_NONE = 0,                                        // Undefined (invalid)
        JVX_DATAFORMAT_DATA = 1,                                        // double or float datatype
        JVX_DATAFORMAT_16BIT_LE = 2,                                    // 16 Bit signed
        JVX_DATAFORMAT_24BIT_LE = 3,                                    // 24 Bit signed
        JVX_DATAFORMAT_32BIT_LE = 4,                                    // 32 Bit signed
        JVX_DATAFORMAT_64BIT_LE = 5,                                    // 32 Bit signed
        JVX_DATAFORMAT_8BIT = 6,                                                // 8 Bit
        JVX_DATAFORMAT_STRING = 7,                                              // String type (character buffers)
        JVX_DATAFORMAT_STRING_LIST = 8,                                         // String type (character buffers)
        JVX_DATAFORMAT_SELECTION_LIST = 9,
        JVX_DATAFORMAT_VALUE_IN_RANGE = 10,
        JVX_DATAFORMAT_VOID = 11, // Void: Gateway for other datatypes
        JVX_DATAFORMAT_LIMIT

} jvxDataFormat;

static const char* jvxDataFormat_str[JVX_DATAFORMAT_LIMIT] =
{
	"none",
        "jvxData(" JVX_DATA_DESCRIPTOR ")",
	"16 Bit le",
	"24 Bit le",

	"32 Bit le",
	"64 Bit le",
	"8 Bit",
	"string",
	"string list",

	"selection list",
	"value in range",
	"void"
};

static inline const char* jvxDataFormat_txt(jvxSize id)
{
	assert(id < JVX_DATAFORMAT_LIMIT);
	return jvxDataFormat_str[id];
}

static inline jvxDataFormat jvxDataFormat_decode(const char * token)
{
	jvxSize i;
	jvxDataFormat res = JVX_DATAFORMAT_NONE;
	for(i = 0; i < JVX_DATAFORMAT_LIMIT; i++)
	{
          if(!strcmp(jvxDataFormat_str[i], token))
		{
			res = (jvxDataFormat) i;
			break;
		}
	}
	return(res);
}

//! Typedef enum for sampleformat
static jvxSize jvxDataFormat_size[JVX_DATAFORMAT_LIMIT] =
{
	0,
	sizeof(jvxData),
	sizeof(jvxInt16),
	sizeof(jvxInt32),
	sizeof(jvxInt32),
	sizeof(jvxInt64),
	sizeof(jvxInt8),
	sizeof(jvxInt8),
	0,
	0,
	0,
	0
} ;

JVX_DSP_LIB_END

#endif
