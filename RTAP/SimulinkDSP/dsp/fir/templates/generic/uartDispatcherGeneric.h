/******************************************************
 * Filename: uartDispatcherGeneric.h				  *
 * Description: Class to realize the uart dispatcher  *
 *				for generic messages. The generic 	  *
 *				dispatcher transforms incoming RS232  *
 *				bytes into generic messages which 	  *
 *				people can use for multi purposes.    *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#ifndef USE_UART_FOR_MIDI

#ifndef _UARTDISPATCHER_GENERIC_H__
#define _UARTDISPATCHER_GENERIC_H__

#include "uartIND.h"
#include "IuartDispatcher.h"
#include "IuartApplicationGeneric.h"

/*******************************************************/
// The generic message format is the following (in bytes)
// SW1 - SW2 - SW3 - SW4 - MSGPURP - OPCODE - 
// LENGTHLOAD1 - LENGTHLOAD2 - <GROUPS OF 4 BYTES, LENGTHLOAD x LOAD>
// 4 Syncwords (SW) are scanned before the message is accepted,
// Message purpose is used to control the dispatcher, opcode
// is a user field to communicate the message purpose.
// The length of the load field is passed in two bytes.
// This length is the number of DATATYPE_RS232 fields (in general 4 bytes)
// that is passed in the load field. All bytes are stored in the load field
// on incoming messages until the number of elements is reached.
// Only complete messages are pased to the processing interrupt which
// is the second lowest softwre interrupt (SFTI2).
// Streaming mode is currently not supported.
/*******************************************************/

// Syncword1
#define SYNCWORD_DEFAULT1 0x19

// Syncword2
#define SYNCWORD_DEFAULT2 0x06

// Syncword3
#define SYNCWORD_DEFAULT3 0x19

// Syncword4
#define SYNCWORD_DEFAULT4 0x75

#ifdef N32BIT_TO_CHAR_MSBFIRST
#define SYNCWORD_STREAMING 0x19061975
#else
#define SYNCWORD_STREAMING 0x75190619
#endif

enum ERROR_MESSAGE_IDS_SYSTEM
{
	ERR_NOERROR = 0,
	ERR_NO_INPUT_ID_FUNCTION_NAME = 1,
	ERR_FUNCTION_CALL_REQUEST_I = 2,
	ERR_FUNCTION_CALL_REQUEST_II = 3,
	ERR_NO_INPUT_FUNCTION_PARAM = 4,
	ERR_INPUT_PARAM_STATE = 5,
	ERR_TOO_MANY_INPUT_PARAM = 6,
	ERR_EXECUTE_STATE = 7,
	ERR_EXECUTE_FUNCTION_ID = 8,
	ERR_OUTPUT_PARAM_STATE = 9,
	ERR_TOO_MANY_OUTPUT_PARAM = 10	
	
};

class uartIND;
class IuartApplicationGeneric;

/*==================================================
 * UART dispatcher for generic messages. Messages towards the RS232
 * link are preceded by the header to achieve the right format,
 * data is directly copied to the DMA buffers.
 * Incoming messages are passed to the input parser where the right format is checked.
 * At the end a complete message is stored internally and the buffer is reported to the
 * application object handle.
 *///===============================================
class CuartDispatcherGeneric:public IuartDispatcher
{
public:
	
    // Message Purpose (transformed into integers in general)    
    enum purposeMessage
	{
		PMSG_POLLING = 0,
		PMSG_REQUEST_HANDSHAKE = 1,
		PMSG_ACKNOWLEDGE_HANDSHAKE = 2,
		PMSG_REQUEST_GOODBYE = 3,
		PMSG_ACKNOWLEDGE_GOODBYE = 4,
		PMSG_REQUEST_SYNCHRONIZATION = 5,
		PMSG_ACKNOWLEDGE_SYNCHRONIZATION = 6,
		PMSG_SWITCH_MODE_STREAMING,
		PMSG_SWITCH_MODE_DEFAULT,
		PMSG_EXCHANGE_GENERIC_CONFIGURATION_REQUEST,
		PMSG_EXCHANGE_GENERIC_CONFIGURATION_RESPONSE,
		PMSG_GENERIC_MESSAGE,
#ifdef DEBUG_MATLAB
		PMSG_REQUEST_HSK_DEBUG_MODE,
		PMSG_ACKNOWLEDGE_HSK_DEBUG_MODE,
		PMSG_REQUEST_GBY_DEBUG_MODE,
		PMSG_ACKNOWLEDGE_GBY_DEBUG_MODE,
		PMSG_REQUEST_NUMBER_FUNCTIONS,
		PMSG_ACKNOWLEDGE_NUMBER_FUNCTIONS,
		PMSG_REQUEST_FUNCTION_NAME_ID,
		PMSG_ACKNOWLEDGE_FUNCTION_NAME_ID,
		PMSG_REQUEST_FUNCTION_PARAM_ID,
		PMSG_ACKNOWLEDGE_FUNCTION_PARAM_ID,
		PMSG_REQUEST_FUNCTION_CALL,
		PMSG_ACKNOWLEDGE_FUNCTION_CALL,
		PMSG_REQUEST_SEND_INPUT_PARAMETER,
		PMSG_ACKNOWLEDGE_SEND_INPUT_PARAMETER,
		PMSG_REQUEST_EXECUTE_FUNCTION,
		PMSG_ACKNOWLEDGE_EXECUTE_FUNCTION,
		PMSG_REQUEST_REQUEST_OUTPUT_PARAMETER,
		PMSG_ACKNOWLEDGE_REQUEST_OUTPUT_PARAMETER,
		PMSG_REQUEST_STATUS,
		PMSG_ACKNOWLEDGE_STATUS,
#endif
		PMSG_REQUEST_PROCESSING_PARAMETERS,
		PMSG_ACKNOWLEDGE_PROCESSING_PARAMETERS,
		PMSG_REPORT_ERROR,
		PMSG_NOT_SUPPORTED,
		PMSG_PURPOSE_INVALID = 0xFF
	};


    // States for the parser state machine
    enum stateScanMessages
    {
        MESSAGE_TO_START = 0,
        MESSAGE_SYNC1_COMPLETE = 1,
        MESSAGE_SYNC2_COMPLETE = 2,
        MESSAGE_SYNC3_COMPLETE = 3,
        MESSAGE_SYNC4_COMPLETE = 4,
        MESSAGE_PURPOSE_DONE = 5,
        MESSAGE_OPCODE_DONE = 6,
        MESSAGE_SCAN_LOAD = 7
    };

#ifdef DEBUG_MATLAB
	typedef enum
	{
		DBG_ST_DEVICE_UNINITIALZED,
		DBG_ST_NORMAL_MODE,
		DBG_ST_DEBUG_MODE,
		DBG_ST_FUNCTION_PROCESSING
	} DEBUGGER_STATE;
#endif

private:

	// State machine for input
    stateScanMessages inputStateMachine;

    //! Memory to store the next word
    DATATYPE_RS232 newWord;

    // Reference to the uartIND handle
    uartIND* refHandleLink;

     
    // Input, all stored in a struct
	struct
    {
    	// Pointer to load field for quick access in ISR
    	DATATYPE_RS232* ptrLoadField;

    	// Word counter in incoming message
    	WORD32 cntMessageLoad;
    	
    	// Scanned message purpose in ISR
    	DATATYPE_RS232_CHAR msgPurpose;

    	// Scanned message purpose in ISR
    	DATATYPE_RS232_CHAR msgOpcode;
	   	
    	// Pointer to incoming field in OSR
    	DATATYPE_RS232* inputMessagePtr;
    	
    	// Max Length of load field
    	WORD32 lLoadField;
    	
    	// Byte counter in word (in general 0,1,2,3)
    	WORD32 byteCounterWord;
    	
    	// I think I do not need these anymore
//	    WORD32 idMessageInput;
//		WORD32 idMessageInputProcessed;
    } inputMessage;
    	
    
	// All for output
	// The output field is of variable length and dynamic memory
    DATATYPE_RS232_CHAR* outputMessageVarLength;

    // Pointer to a certain segement in the output buffer if sent in chunks
    DATATYPE_RS232_CHAR* ptrContinue;
   	WORD32 elementsToGo;
   	
   	// Message ID to identify completed mesages by the sender
    WORD32 messageID;
    
    // Flag (thread safe) to avoid multithreading problems
    WORD32 sendInProgress;

public:

	// Struct for message processing, must be public because it is accessed in ISR
    struct
    {
    	// Pointer to the field allocated in high level ISR
    	DATATYPE_RS232* loadFieldToProcess;
    	
    	// Length of the incoming load field
    	WORD32 lLoadField;
    	
    	// Message purpose incoming message
    	WORD32 msgPurpose;

    	// Message opcode incoming message
    	WORD32 msgOpcode;
    	
    	// Flag to indicate that message is currently processed
    	WORD32 inProcessing;
    } processInputMessage;

    // Reference to the application handle
    IuartApplicationGeneric* refHandleApplication;

#ifdef DEBUG_MATLAB
	DEBUGGER_STATE stateDebugger;
	DEBUGGER_STATE stateDebuggerPrevious;
#endif

public:

    // Constructor
    CuartDispatcherGeneric();

    // API FUNCTIONS
    //! Initialize by setting the handles for the callbacks
    virtual void initialize(uartIND* handleLink, IuartApplicationGeneric* handleApplication);

    //! Callback from uartIND to report another incoming byte
    virtual void byteReceivedDefault(DATATYPE_RS232_CHAR byteMess);

    //! Callback from uartIND to report a complete DMA message
    virtual void messageReceivedStreaming(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING]);

    //! Function call to report an error message through RS-232
    virtual bool reportError(WORD32 errID);

    //! Callback from uartIND to report that a message has been completely sent
    virtual void messageSendComplete();

    // CONTROL FUNCTIONS

	// Send non blocking message, API function for user
   	virtual bool sendMessageNonBlocking(DATATYPE_RS232_CHAR opcode,
		DATATYPE_RS232* ptrField, DATATYPE_RS232_CHAR numElements, WORD32 messageID);

	// Send non blocking message, API function for anyone
	virtual bool sendMessageNonBlocking(DATATYPE_RS232_CHAR purpose,
		DATATYPE_RS232_CHAR opcodeID, DATATYPE_RS232* ptrField, WORD32 numElements,
		WORD32 messageID);
	
};

#endif
#endif
