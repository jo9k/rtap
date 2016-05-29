#ifndef __TJVXTYPES_CPP_H__
#define __TJVXTYPES_CPP_H__

#include "jvx_platform.h"

JVX_INTERFACE IjvxObject;

/*
typedef enum
{
	JVX_PROCESSING_PURPOSE_UNSPECIFIC = 0x0,
	JVX_PROCESSING_PURPOSE_AUDIO_IO_N0_7 = 0x1,
	JVX_PROCESSING_PURPOSE_DATA_IO_N0_7 = 0x10
} jvxProcessingPurpose;
*/

typedef enum
{
	JVX_DATAPROCESSOR_HINT_DESCRIPTOR_TYPE_NONE = 0,
	JVX_DATAPROCESSOR_HINT_DESCRIPTOR_NUMBER_LOST_FRAMES_INT32,
	JVX_DATAPROCESSOR_HINT_DESCRIPTOR_ASYNC_FRAME_COUNT_INT32,
	JVX_DATAPROCESSOR_HINT_DESCRIPTOR_TYPE_BWD_REFERENCE,

#include "typedefs/TpjvxTypes_dataProcessorHintDescriptor.h"
} jvxDataProcessorHintDescriptorType;

struct jvxDataProcessorHintDescriptor
{
	jvxHandle* hint;
	jvxDataProcessorHintDescriptorType hintDescriptorId;
	jvxComponentType assocHint;
	jvxDataProcessorHintDescriptor* next;
} ;

typedef struct
{
	struct
	{
		jvxSize number_buffers;
		jvxSize buffersize;
		jvxSize number_channels;
		jvxDataFormat format;
		jvxInt32 rate;
		jvxHandle*** to_receiver_buffer_allocated_by_receiver;
		jvxSize slot_id;
		jvxDataProcessorHintDescriptor* user_hints;
	} sender_to_receiver;

	struct
	{
		jvxSize number_buffers;
		jvxSize buffersize;
		jvxSize number_channels;
		jvxDataFormat format;
		jvxInt32 rate;
		jvxHandle*** from_receiver_buffer_allocated_by_sender;
		jvxSize slot_id;
		jvxDataProcessorHintDescriptor* user_hints;
	} receiver_to_sender;

	struct
	{
		jvxComponentType tp;
		IjvxObject* source;
	} sender;

	struct
	{
		jvxComponentType tp;
		IjvxObject* destination;
	} receiver;

	jvxSize slot_id;
	jvxInt64 tStamp_us;
	jvxInt32 frames_lost_since_last_time;

#include "typedefs/TpjvxTypes_linkdata.h"
} jvxLinkDataDescriptor;


// Some C++ datatypes
typedef struct
{
	std::vector<std::string> entries;
	jvxBitField selection;
	jvxBitField exclusive;
} jvx_oneSelectionOption_cpp;

#endif
