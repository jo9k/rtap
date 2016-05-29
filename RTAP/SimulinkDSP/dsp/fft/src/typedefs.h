/******************************************************
 * Filename: typedefs.h						  		  *
 * Description: Definition of memory field types   	  *
 *				(there is only 32 bit memory fields   *
 *				anyway (acc. to LDF).				  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 
 #ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#define WORD16 short
#define WORD32 int
#define UWORD16 unsigned short
#define UWORD32 unsigned int

// Typedefs for processing states
typedef enum
{
	RTP_STATE_NONE = -1,
	RTP_STATE_INIT = 0,
	RTP_STATE_SELECTED = 1,			// Plugin has been opened but has not yet been initialized
	RTP_STATE_ACTIVE = 2,		// Plugin has currently been initialized
	RTP_STATE_PREPARED = 3,			// Plugin is ready for processing
	RTP_STATE_PROCESSING =4		// Plugin is in processing state
} RTP_STATE;


// Typedefs for generic configure elements
typedef enum
{
    RTP_GEN_CONF_DSP_ELEMENT_FLOAT_ARRAY = 1,
    RTP_GEN_CONF_DSP_ELEMENT_INT32_ARRAY = 2,
} RTP_GEN_CONF_TYPE_DSP;


// Generic configure purpose enumeration values
typedef enum
{
    RTP_GEN_CONF_DSP_SET = 0,
    RTP_GEN_CONF_DSP_GET = 1,
    RTP_GEN_CONF_DSP_IGNORE = 2
} RTP_GEN_CONF_PURPOSE_DSP;


// specialize the objects
typedef enum
{
	RTP_OBJECT_NONE = -1,
	RTP_OBJECT_TECHNOLOGY = 0,
	RTP_OBJECT_DEVICE = 1,
	RTP_OBJECT_ALGORITHM = 2,
	RTP_OBJECT_HOST = 3,
	RTP_OBJECT_PROCESSINGNODE = 4,
	RTP_OBJECT_ENHANCEMENT = 5,
	RTP_OBJECT_ENHANCEMENT_OBJECT = 6,
	RTP_OBJECT_COMMUNICATION = 7,
	RTP_OBJECT_COMMUNICATION_OBJECT = 8,
	RTP_OBJECT_CODEC = 9,
	RTP_OBJECT_CODEC_OBJECT = 10
	//RTP_P_RS232_PORTS = 6,
	//RTP_P_MATLAB_CALLHANDLER = 7,
} RTP_OBJECT;


typedef enum
{ 
	RTP_INTERFACE_NONE = -1,						
	RTP_INTERFACE_CONTROLWINDOW = 0,						
	RTP_INTERFACE_NODESYSTEM = 1,
	RTP_INTERFACE_BUFFERING = 2,//*
	RTP_INTERFACE_WRITELOGFILES = 3,//*
	RTP_INTERFACE_WRITEAUDIOFILES = 4,//*
	RTP_INTERFACE_READAUDIOFILES = 5,//*
	RTP_INTERFACE_COMMUNICATION = 6,
	RTP_INTERFACE_CONFIGURATION = 7,					
	RTP_INTERFACE_RUNTIMEMESSAGING = 8,						
	RTP_INTERFACE_RESAMPLER = 9, //*
	RTP_INTERFACE_ENHANCEMENT = 10
} RTP_INTERFACE;


//! Typedef enums for error types
typedef enum
{ 
	RTP_ERROR_NOERROR = -1,						
	RTP_ERROR_INTERNAL = 0,
	RTP_ERROR_NOT_READY = 1,
	RTP_ERROR_NOT_COMPATIBLE = 2,	
	RTP_ERROR_WRONGSTATEFORCALL = 3,
	RTP_ERROR_IDMISMATCH = 4,		
	RTP_ERROR_VOIDDATASIZEMISMATCH = 5,
	RTP_ERROR_ELEMENTNOTFOUND = 6, 
	RTP_ERROR_OPERATIONNOTSUPPORTED = 7, 
	RTP_ERROR_MISC = 128,
	RTP_ERROR_MATLAB = 256				

} RTP_ERROR;


//! Typedef enum for sampleformat
typedef enum
{
	RTP_DATAFORMAT_NONE = -1,						// Undefined (invalid)
	RTP_DATAFORMAT_DOUBLE = 0,					// double datatype
	RTP_DATAFORMAT_FLOAT = 1,					// float datatype
	RTP_DATAFORMAT_16BIT_LE = 2,					// 16 Bit signed
	RTP_DATAFORMAT_24BIT_LE = 3,					// 24 Bit signed
	RTP_DATAFORMAT_32BIT_LE = 4,					// 32 Bit signed
	RTP_DATAFORMAT_8BIT = 5,						// 8 Bit 
	RTP_DATAFORMAT_VOID = 6						// Void: Gateway for other datatypes
} RTP_DATAFORMAT;


// Struct typedef for runtime messaging
typedef struct
{
    struct
    {
        unsigned short idMessageType;
        unsigned short lengthMessage;
    } header;

    char fieldStart;
} runtimeMessage_DSP;


// Single element for generic configure types
typedef struct
{
    signed short IDConfigureElement;
    char nameConfigureElement[STRING_LENGTH];
    RTP_GEN_CONF_PURPOSE_DSP purposeConfigureElement;
    RTP_GEN_CONF_TYPE_DSP typeConfigureElement;
    signed short errorCode;
    void* fieldConfigureElement;
    unsigned short lengthFieldConfigureElement;
} rtpGenericConfigureElement_DSP;


// Struct to combine more than one generic configure type
typedef struct
{
    rtpGenericConfigureElement_DSP* fieldElements;
    unsigned short numberElements;
} rtpGenericConfigure_DSP;

#endif
