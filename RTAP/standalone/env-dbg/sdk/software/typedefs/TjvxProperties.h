#ifndef __TJVXPROPERTYIDS_H__
#define __TJVXPROPERTYIDS_H__

typedef enum
{
	JVX_THREADING_NONE = 0,
	JVX_THREADING_INIT = 0x1,
	JVX_THREADING_ASYNC = 0x2,
	JVX_THREADING_SYNC = 0x4,
	JVX_THREADING_DONTCARE = 0xFFFFFFFF
} jvxPropertySetType;

typedef enum
{
	JVX_PROPERTY_CATEGORY_UNKNOWN = 0,
	JVX_PROPERTY_CATEGORY_PREDEFINED = 1,
	JVX_PROPERTY_CATEGORY_UNSPECIFIC = 2
} jvxPropertyCategoryType;

typedef enum
{
	JVX_PROPERTY_CONTEXT_UNKNOWN = 0,
	JVX_PROPERTY_CONTEXT_PARAMETER = 1,
	JVX_PROPERTY_CONTEXT_INFO = 2,
	JVX_PROPERTY_CONTEXT_RESULT = 3,
	JVX_PROPERTY_CONTEXT_COMMAND = 4,
	JVX_PROPERTY_CONTEXT_VIEWBUFFER = 5,
	JVX_PROPERTY_CONTEXT_LIMIT = 6
} jvxPropertyContext;

typedef enum
{
	JVX_PROPERTY_DECODER_NONE = 0,
	JVX_PROPERTY_DECODER_FILENAME_OPEN,
	JVX_PROPERTY_DECODER_DIRECTORY_SELECT,
	JVX_PROPERTY_DECODER_FILENAME_SAVE,
	JVX_PROPERTY_DECODER_IP_ADDRESS,
	JVX_PROPERTY_DECODER_PLOT_ARRAY,
	JVX_PROPERTY_DECODER_BITFIELD,
	JVX_PROPERTY_DECODER_SINGLE_SELECTION,
	JVX_PROPERTY_DECODER_MULTI_SELECTION,
	JVX_PROPERTY_DECODER_FORMAT_IDX,
	JVX_PROPERTY_DECODER_PROGRESSBAR,
	JVX_PROPERTY_DECODER_INPUT_FILE_LIST,
	JVX_PROPERTY_DECODER_OUTPUT_FILE_LIST,
	JVX_PROPERTY_DECODER_MULTI_SELECTION_CHANGE_ORDER,
	JVX_PROPERTY_DECODER_MULTI_CHANNEL_CIRCULAR_BUFFER,
	JVX_PROPERTY_DECODER_COMMAND,
	JVX_PROPERTY_DECODER_LIMIT
} jvxPropertyDecoderHintType;

typedef enum
{
	JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC = 0,
	JVX_PROPERTY_CALL_PURPOSE_INTERNAL_PASS = 1,
	JVX_PROPERTY_CALL_PURPOSE_TAKE_OVER = 2,
	JVX_PROPERTY_CALL_PURPOSE_INTERNAL_EVENT = 3,
	JVX_PROPERTY_CALL_PURPOSE_COMPONENT_RECONFIGURE = 4
} jvxPropertyCallPurpose;

typedef enum
{
	JVX_PROPERTY_ACCESS_NONE = 0,
	JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE = 1,
	JVX_PROPERTY_ACCESS_READ_AND_WRITE_CONTENT = 2,
	JVX_PROPERTY_ACCESS_READ_ONLY = 3,
	JVX_PROPERTY_ACCESS_WRITE_ONLY = 4,
	JVX_PROPERTY_ACCESS_LIMIT
} jvxPropertyAccessType;

/*
typedef enum
{

	JVX_COMMON_PROPERTY_SCANNER_BATCHSIZE = 0
} jvxCommonPropertiesScanner;
*/

typedef struct
{
	std::string nameProperty;
	jvxInt32 id;
	jvxDataFormat format;
	jvxInt32 numElements;
	jvxPropertyDecoderHintType hintTp;
} oneEntryProperties;

typedef jvxErrorType (*try_lock_circ_buffer)(jvxHandle* priv);
typedef void (*lock_circ_buffer)(jvxHandle* priv);
typedef void (*unlock_circ_buffer)(jvxHandle* priv);

typedef struct
{
	jvxPropertyCategoryType category;
	jvxUInt64 settype;
	jvxUInt64 stateAccess;
	jvxDataFormat format;
	jvxSize num;
	jvxPropertyAccessType accessType;
	jvxPropertyDecoderHintType decTp;
	jvxSize globalIdx;
	jvxPropertyContext ctxt;
	jvxString* name;
	jvxString* description;
	jvxString* descriptor;
	jvxBool isValid;
} jvxPropertyDescriptor;

#ifdef JVX_OS_WINDOWS
#pragma pack(push, 1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct
{
	try_lock_circ_buffer try_lockf;
	lock_circ_buffer lockf;
	unlock_circ_buffer unlockf;
	jvxHandle* priv;
	jvxBool allowRemove;
} jvxSafeAccessBufferHdl;

typedef struct
{
	jvxInt32 bLength;
	jvxSize16 numChannels;
	jvxSize idxRead;
	jvxSize fHeight;
	jvxSize16 allowOverwrite;
	jvxSize numWraps;
	jvxSafeAccessBufferHdl safeAccessBuffer;
} oneMultichannelCircBufferHeader;

#ifdef JVX_OS_WINDOWS
#pragma pack(pop)
#else
#pragma pack(pop)
#endif

// Entries of predefined properties
#define JVX_PROPERTIES_OFFSET_SYSTEM 0
#define JVX_PROPERTIES_OFFSET_PRODUCT 64

#define JVX_PROPERTIES_OFFSET_SUBCOMPONENT 1024

static oneEntryProperties jvxProperties[] =
{
	/*	Identifier-string						,	ID as integer									,	dataformat						,	number elements */
	{	"jvx_command_string"					,	JVX_PROPERTIES_OFFSET_SYSTEM + 0				,	JVX_DATAFORMAT_STRING			,	1	, JVX_PROPERTY_DECODER_COMMAND},
	{	"jvx_framesize_int32"					,	JVX_PROPERTIES_OFFSET_SYSTEM + 1				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_selection_framesize_sellist"		,	JVX_PROPERTIES_OFFSET_SYSTEM + 2				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_SINGLE_SELECTION},
	{	"jvx_rate_int32"						,	JVX_PROPERTIES_OFFSET_SYSTEM + 3				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_selection_rate_sellist"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 4				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_SINGLE_SELECTION},
	{	"jvx_num_input_channels_int32"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 5				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_num_output_channels_int32"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 6				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_selection_input_channels_sellist"	,	JVX_PROPERTIES_OFFSET_SYSTEM + 7				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_MULTI_SELECTION},
	{	"jvx_selection_output_channels_sellist"	,	JVX_PROPERTIES_OFFSET_SYSTEM + 8				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_MULTI_SELECTION},
	{	"jvx_processing_format_int16"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 9				,	JVX_DATAFORMAT_16BIT_LE			,	1	, JVX_PROPERTY_DECODER_FORMAT_IDX},
	{	"jvx_selection_formats_sellist"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 10				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_SINGLE_SELECTION},
	{	"jvx_skip_select_sellist"				,	JVX_PROPERTIES_OFFSET_SYSTEM + 11				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_SINGLE_SELECTION},
	{	"jvx_component_path_str_list"			,	JVX_PROPERTIES_OFFSET_SYSTEM + 12				,	JVX_DATAFORMAT_STRING_LIST		,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_load_dlls_sellist"					,	JVX_PROPERTIES_OFFSET_SYSTEM + 13				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_MULTI_SELECTION},
	{	"jvx_input_delay_int32"					,	JVX_PROPERTIES_OFFSET_SYSTEM + 14				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_output_delay_int32"				,	JVX_PROPERTIES_OFFSET_SYSTEM + 15				,	JVX_DATAFORMAT_32BIT_LE			,	1	, JVX_PROPERTY_DECODER_NONE},
	{	"jvx_engage_sellist"					,	JVX_PROPERTIES_OFFSET_SYSTEM + 16				,	JVX_DATAFORMAT_SELECTION_LIST	,	1	, JVX_PROPERTY_DECODER_SINGLE_SELECTION},
#include "typedefs/TpjvxProperties_entries.h"
	{	""										,	-1				,	JVX_DATAFORMAT_NONE			,	-1}
};


#endif
