#ifndef __TJVX_NETWORK_H__
#define __TJVX_NETWORK_H__

// NOTE byte order is little-endian

#define ETH_MIN_SIZE_RAW_SOCKET 64

typedef enum
{
	JVX_PROTOCOL_GLOBAL_ERROR,
	JVX_PROTOCOL_TYPE_AUDIO_DEVICE_LINK,
	JVX_PROTOCOL_TYPE_PROPERTY_STREAM,
} jvxProtocolType;

// DEFINTIONS FOR PROTOCOL JVX_AUDIO_DEVICE_LINK

#define JVX_PROTOCOL_ADVLINK_MESSAGE_PURPOSE_EXTRACT_MASK 0xC000

typedef enum
{
	JVX_PROTOCOL_ADVLINK_MESSAGE_PURPOSE_TRIGGER = 0x4000,
	JVX_PROTOCOL_ADVLINK_MESSAGE_PURPOSE_REQUEST = 0x8000,
	JVX_PROTOCOL_ADVLINK_MESSAGE_PURPOSE_SPECIFY = 0xC000,
	JVX_PROTOCOL_ADVLINK_MESSAGE_PURPOSE_RESPONSE = 0x0000
} jvxAdvLinkProtocolMessagePurpose;

typedef enum
{
	JVX_PROTOCOL_ADVLINK_TRIGGER_ROLE_SOURCE = 0x1,
	JVX_PROTOCOL_ADVLINK_TRIGGER_ROLE_DESTINATION = 0x2
} jvxAdvLinkProtocolClockTriggerRole;

typedef enum
{
	JVX_PROTOCOL_ADV_CHANNEL_IN_FRAME_TYPE_NON_INTERLEAVED,
	JVX_PROTOCOL_ADV_CHANNEL_IN_FRAME_TYPE_INTERLEAVED
}  jvxAdvLinkProtocolChannelInFrameType;

typedef enum 
{
	JVX_PROTOCOL_ADVLINK_ERROR_RESET = 0,

	JVX_PROTOCOL_ADVLINK_INIT = 1,
	JVX_PROTOCOL_ADVLINK_TERM = 2,

	// Following valid in state JVX_STATE_INIT
	JVX_PROTOCOL_ADVLINK_DESCR = 16,
	JVX_PROTOCOL_ADVLINK_TRIGGER_ROLE_MASK = 17,
	
	// From state JVX_STATE_INIT to JVX_STATE_ACTIVE
	JVX_PROTOCOL_ADVLINK_ACTIVATE = 32, // Specifies used clock trigger setting
	JVX_PROTOCOL_ADVLINK_DEACTIVATE = 33,

	// Following valid in state JVX_STATE_INIT
	JVX_PROTOCOL_ADVLINK_SRATE = 48,
	JVX_PROTOCOL_ADVLINK_BSIZE = 49,
	JVX_PROTOCOL_ADVLINK_FORMAT = 50,
	JVX_PROTOCOL_ADVLINK_NUM_INCHANNELS = 51,
	JVX_PROTOCOL_ADVLINK_NUM_OUTCHANNELS = 52,
	JVX_PROTOCOL_ADVLINK_CHANNEL_IN_FRAME_TYPE = 53,

	// From state JVX_STATE_ACTIVE to JVX_STATE_PROCESSING
	JVX_PROTOCOL_ADVLINK_START = 64,

	// From state JVX_STATE_PROCESSING to JVX_STATE_ACTIVE
	JVX_PROTOCOL_ADVLINK_STOP = 65,

	// state processing
	JVX_PROTOCOL_ADVLINK_SEND_DATA = 80,

	// all states
	JVX_PROTOCOL_ADVLINK_STATUS = 128
	
} jvxAdvLinkProtocolCommandType;

// Variables defined by jvx software repository:
// 1) Variable definitions that are used in the dsp base library as well in all CPP components: 
//		-> jvx/base/sources/jvxLibraries/jvx-dsp-base/include/jvx_dataformats.h
// 2) Variable definitions that are used in the CPP component system only since those may be CPP 
// 	  code or may be useful only in CPP components (example: jvxState)
//  	-> jvx/base/software/typedefs/TjvxTypes.h
// 3) All OS specific variable type definitions: 
//		-> jvx/base/sources/jvxLibraries/jvx-dsp-base/include/os-<OS>/jvxL_jvx_dsp_base_types__os.h"
//	  	(with OS representing win32, Linux, MacOSx or ios at the moment)

// We need to make sure that struct element boundaries are not extended by 32 bit or similar
// alignments caused by the compiler
#ifdef JVX_OS_WINDOWS
#pragma pack(push, 1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

// Raw socket header. Note that this struct is only used in case of raw sockets,
// in UDB and TCP, there is no raw socket header 
typedef struct
{
	jvxUInt8  dest_mac[6]; // destination MAC address
	jvxUInt8  src_mac[6];  // source MAC address
	jvxUInt8 proto_ident[2];   // 0x4a58 = 'J''X'
} jvxRawSocketHeader;

// Javox Protocol header: Contains just a 2-byte family id
typedef struct
{
	jvxUInt16 proto_family;
} jvxProtocolFamilyHeader; 

// Subtype adv protocol header: extends family header by 2 and 4 byte fields 
typedef struct
{
	jvxProtocolFamilyHeader fam_hdr;
	
	jvxUInt16 advlink_purpose;
	jvxUInt32 advlink_paketsize;
} jvxAdvProtocolHeader;

// Definition of a description frame
typedef struct
{
	jvxAdvProtocolHeader adv_header;
	
	jvxUInt8 description[32];
} jvxAdvProtocolDescriptionP;

// Frame type for simple passing of different parameters. Note that there
// may also be more that one entry - indicated by packet length
typedef struct
{
	jvxAdvProtocolHeader adv_header;
	
	jvxUInt32 first_transfer_load;
} jvxAdvProtocolTransferP;

typedef struct
{
	jvxUInt64 channel_mask;
	jvxUInt32 sequence_id;
} jvxAdvProtocolDataChunkHeader;

// Frame type for a single audio frame
typedef struct
{
	jvxAdvProtocolHeader adv_header;
	jvxAdvProtocolDataChunkHeader adv_data_header;
	jvxUInt8 first_byte_transfer;
} jvxAdvProtocolDataP;

typedef union
{
	jvxAdvProtocolDescriptionP descr;
	jvxAdvProtocolTransferP transfer;
	jvxAdvProtocolDataP aframe;
} jvxAdvProtocolUnion;


// Subtype adv protocol header: extends family header by 2 and 4 byte fields 
typedef struct
{
	jvxProtocolFamilyHeader fam_hdr;
	
	jvxUInt16 propid;
	jvxUInt32 dataformat;
	jvxUInt32 numberElements;
	jvxUInt32 offsetElements;

} jvxPropStreamProtocolHeader;

/*
 * Why I would not use unions: If you use the 
 * sizeof operator it will return the maximum size -
 * even for short frame data types
 
typedef struct {
	UInt8  dest_mac[6]; // destination MAC address
	UInt8  src_mac[6];  // source MAC address
	UInt16 packed_id;   // 0x4a58 = 'J''X'
	UInt16 protocol;    // protocol identifier (Jvx_protocol)
	UInt16 command;     // command identifier (Avdlink_id); MSB distinguishes between request (0) and return (1)
	UInt16 len;         // payload length
	// frame overlay structures
	union {
		jvx_init_frame       init_frame;
		jvx_active_frame     active_frame;
		jvx_processing_frame processing_frame;
	};
} jvx_frame;
*/

#ifdef JVX_OS_WINDOWS
#pragma pack(pop)
#else
#pragma pack(pop)
#endif 

#endif /* __TJVX_NETWORK_H__ */

