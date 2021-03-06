#ifndef __TJVXTYPES_H__
#define __TJVXTYPES_H__


#include "jvx_dataformats.h"

/** Typedef for the maximum length of text fields */
#define JVX_MAXSTRING 256

typedef enum
{
	JVX_STATE_NONE = 0x0,
	JVX_STATE_INIT = 0x1,
	JVX_STATE_SELECTED = 0x2,			// Plugin has been opened but has not yet been initialized
	JVX_STATE_ACTIVE = 0x4,		// Plugin has currently been initialized
	JVX_STATE_PREPARED = 0x8,			// Plugin is ready for processing
	JVX_STATE_PROCESSING = 0x10,		// Plugin is in processing state
	JVX_STATE_COMPLETE = 0x20,		// Plugin is in processing state
	JVX_STATE_DONTCARE = 0xFFFFFFFF
} jvxState;

typedef enum
{
	JVX_STATE_SWITCH_NONE,
	JVX_STATE_SWITCH_INIT,
	JVX_STATE_SWITCH_SELECT,
	JVX_STATE_SWITCH_ACTIVATE,
	JVX_STATE_SWITCH_PREPARE,
	JVX_STATE_SWITCH_START,
	JVX_STATE_SWITCH_STOP,
	JVX_STATE_SWITCH_POSTPROCESS,
	JVX_STATE_SWITCH_DEACTIVATE,
	JVX_STATE_SWITCH_UNSELECT,
	JVX_STATE_SWITCH_TERMINATE,
	JVX_STATE_SWITCH_LIMIT
} jvxStateSwitch;

typedef enum
{
	JVX_COMPONENT_TYPE_NONE,
	JVX_COMPONENT_TYPE_TECHNOLOGY,
	JVX_COMPONENT_TYPE_NODE,

	JVX_COMPONENT_TYPE_LIMIT
} jvxComponentTypeClass;


typedef enum
{
	JVX_COMPONENT_UNKNOWN = 0,
#include "typedefs/TpjvxTypes_components.h"
	JVX_COMPONENT_AUDIO_TECHNOLOGY,
	JVX_COMPONENT_AUDIO_DEVICE,
	JVX_COMPONENT_AUDIO_NODE,
	JVX_COMPONENT_APPLICATION_CONTROLLER_TECHNOLOGY,
	JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE,
	JVX_COMPONENT_HOST,
#include "typedefs/TpjvxTypes_tools.h"
	JVX_COMPONENT_DATALOGGER,
	JVX_COMPONENT_DATALOGREADER,
	JVX_COMPONENT_RS232,
	JVX_COMPONENT_CONFIG_PROCESSOR,
	JVX_COMPONENT_EXTERNAL_CALL,
	JVX_COMPONENT_THREADCONTROLLER,
	JVX_COMPONENT_CRYPT,
	JVX_COMPONENT_WEBSERVER,


	JVX_COMPONENT_LIMIT
}  jvxComponentType;


typedef enum
{
	JVX_INTERFACE_UNKNOWN = 0,
	JVX_INTERFACE_DATAPROCESSOR,
	JVX_INTERFACE_DATAPROCESSOR_SELECTOR,
#include "typedefs/TpjvxTypes_interfaces.h"
	JVX_INTERFACE_CONFIGURATION,
	JVX_INTERFACE_TOOLS_HOST,
	JVX_INTERFACE_REPORT,
	JVX_INTERFACE_PROPERTIES,
	JVX_INTERFACE_SEQUENCERCONTROL,
	JVX_INTERFACE_SEQUENCER,
	JVX_INTERFACE_CONFIGURATIONLINE,
	JVX_INTERFACE_HOSTTYPEHANDLER,
	JVX_INTERFACE_QT_WIDGET_HOST,
	JVX_INTERFACE_QT_WIDGET_UI,
	JVX_INTERFACE_BINARY_FAST_DATA_CONTROL,

	JVX_INTERFACE_LIMIT
}  jvxInterfaceType;

typedef enum
{
	JVX_NO_ERROR = 0,
#include "typedefs/TpjvxTypes_errorcodes.h"
	JVX_ERROR_UNKNOWN,
	JVX_ERROR_INVALID_ARGUMENT,
	JVX_ERROR_WRONG_STATE,
	JVX_ERROR_ID_OUT_OF_BOUNDS,
	JVX_ERROR_UNSUPPORTED,
	JVX_ERROR_ELEMENT_NOT_FOUND,
	JVX_ERROR_DUPLICATE_ENTRY,
	JVX_ERROR_OPEN_FILE_FAILED,
	JVX_ERROR_UNEXPECTED,
	JVX_ERROR_NOT_READY,
	JVX_ERROR_COMPONENT_BUSY,
	JVX_ERROR_BUFFER_OVERFLOW,
	JVX_ERROR_BUFFER_UNDERRUN,
	JVX_ERROR_WRONG_STATE_SUBMODULE,
	JVX_ERROR_MESSAGE_QUEUE_FULL,
	JVX_ERROR_PARSE_ERROR,
	JVX_ERROR_WRONG_SECTION_TYPE,
	JVX_ERROR_INTERNAL,
	JVX_ERROR_EMPTY_LIST,
	JVX_ERROR_CALL_SUB_COMPONENT_FAILED,
	JVX_ERROR_THREADING_MISMATCH,
	JVX_ERROR_SYSTEMCALL_FAILED,
	JVX_ERROR_SIZE_MISMATCH,
	JVX_ERROR_READ_ONLY,
	JVX_ERROR_WRITE_ONLY,
	JVX_ERROR_INVALID_SETTING,
	JVX_ERROR_TIMEOUT,
	JVX_ERROR_ABORT,
	JVX_ERROR_POSTPONE,
	JVX_ERROR_END_OF_FILE,
	JVX_ERROR_STANDARD_EXCEPTION,
	JVX_ERROR_SEQUENCER_TOOK_CONTROL,
	JVX_ERROR_THREAD_ONCE_MORE,
	JVX_ERROR_PROCESS_COMPLETE,
	JVX_ERROR_DSP,
	JVX_ERROR_ALREADY_IN_USE,

	JVX_ERROR_LIMIT
} jvxErrorType;

typedef enum
{
	JVX_BITFIELD_DLL_PROPERTY_DO_NOT_UNLOAD = 0x1
} jvxDllPropertyBitfield;


typedef enum
{
	JVX_PRIORITY_ABOVE_UNKNOWN,
	JVX_PRIORITY_ABOVE_NORMAL,
	JVX_PRIORITY_BELOW_NORMAL,
	JVX_PRIORITY_HIGH,
	JVX_PRIORITY_IDLE,
	JVX_PRIORITY_NORMAL,
	JVX_PRIORITY_REALTIME
} jvxPriorityType;

typedef enum
{
	JVX_FEATURE_CLASS_NONE = 0x0,
	JVX_FEATURE_CLASS_EXTERNAL_CALL = 0x1,
	JVX_FEATURE_CLASS_HOOKUP_CALL = 0x2,
	JVX_FEATURE_CLASS_COMPONENT_SOCKET_SERVER = 0x4,
	JVX_FEATURE_CLASS_COMPONENT_SOCKET_CLIENT = 0x8
} jvxFeatureClass;

typedef enum
{
	JVX_WIDGET_HOST_NONE,
	JVX_WIDGET_HOST_QT
} jvxWidgetHostType;

//! Single string element
typedef struct
{
	jvxSize lString;
	char* bString;
} jvxString;

//! List of string elements
typedef struct
{
	jvxSize lStrings;
	jvxString* bStrings;
} jvxStringList;

//! List of string elements
typedef struct
{
	jvxStringList* strList;
	jvxBitField bitFieldSelected;
	jvxBitField bitFieldExclusive;
} jvxSelectionList;

//! Integer list type
typedef struct
{
	jvxSize lIntList;
	jvxInt32* bIntList;
} jvxInt32List;

//! Integer list type
typedef struct
{
	jvxSize lDblList;
	jvxData* bDblList;
} jvxDataList;

typedef struct
{
	jvxData minVal;
	jvxData maxVal;
	jvxData val;
} jvxValueInRange;

//! Error code error type
typedef struct
{
	jvxInt32 errorCode;
	jvxString* errorDescription;
	jvxInt32 errorLevel;
} jvxError;

#include "typedefs/TpjvxTypes_datatypes.h"

#endif
