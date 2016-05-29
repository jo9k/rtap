/***********************************************************************
 * DESCRIPTION:
 * serial connection Descriptor: Base class for handling all RS-232 events on one PORT
 ***********************************************************************
 * This class provides functions to implement all functionality for ONE COM port:
 * Open the port, connecting the port, sending messages, awaiting
 * incoming events.
 * FUNCTIONS ARE ASSUMPTIONS OF THE MAIN FUNCTIONALITY REQUIRED AND MAY HAVE TO
 * BE MODIFIED FOR SPECIAL APPLICATIONS. HOWEVER I WILL ONLY BE ABLE TO FIND
 * THESE CASES IN EXTENSIVE TESTING APPLICATIONS!
 *
 * Copyright Hauke Krueger, IND, 2004, 02-06-04
 *********************************************************************/

#ifndef _SERIALELEMENTDESCRIPTOR_H__
#define _SERIALELEMENTDESCRIPTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
#include "windows.h"

class serialConnectionUseRef;
class serialConnection;

/** Hard limitation of the maximum number of elements in buffer for message queue
 *///===========================================================================
#define MESSAGE_QUEUE_SIZE_MAX 1024

/**
 * Class to implement one RS-232-connection. Open the port first, after that connect.
 * You can now send messages, will obtain feedback about a possible timeout 
 * passed to send the messages (disconnect detection). Messages from the port are reported 
 * asynchronously based on a message queue system using the UseRef-link. Indications of
 * single lines are synchronously reported and must be handled immediately and in the context
 * of real time processing (no blocking!). The messages are stored in byte fields. The criterion 
 * when to report the arrival of new bytes is to be selected as stop mode. The application 
 * may wait for timeouts inbetween arrival of bytes (timeout mode) may scan for a certain amount of
 * bytes (fieldlength) or may wait for specific stop characters (stop char). Depending on the
 * application one of the methodolies should be in compliance with the needs.
 * However there is no indication whether e.g. in stop mode the current buffer was reported due to the
 * maximum buffersize reached or a possible timeout. The reporting modes apply only for the timing 
 * behavior! (To make clear when to use what: If waiting for a stop character, no byte may be reported
 * until the last byte has been scanned, this may even take minutes. So if a fast response is to be guaranteed,
 * use the timeout mode because this mode guarantees that the first arrival of a byte is reported
 * as latest the timeout later (even though this may also be abused (new byte means to start the timeout again!)).
* FUNCTIONS ARE ASSUMPTIONS OF THE MAIN FUNCTIONALITY REQUIRED AND MAY HAVE TO
* BE MODIFIED FOR SPECIAL APPLICATIONS. HOWEVER I WILL ONLY BE ABLE TO FIND
* THESE CASES IN EXTENSIVE TESTING APPLICATIONS!
*///=============================================================================================
class serialElementDescriptor  
{
public:

	//! State for the current com port: Open means that a file handle exists, all actions allowed in mode connected only
	typedef enum
	{
		RS232_PORT_CONNECTED,		// Connected: Messages can be received or sent
		RS232_PORT_OPEN,			// Port is open, obtained a file handle, no connection yet
		RS232_PORT_NOTCONNECTED,	// Initial state
		RS232_PORT_DONTUSE
	} connectionStateEnum;

	//! Enums for RS-232-control, I am not sure what exactly means which mode so it is good to use the def values!
	typedef enum
	{
		NO_FLOWCONTROL,				// No flowcontrol
		HW_FLOWCONTROL,				// HW flowcontrol
		SW_FLOWCONTROL,				// SW flowcontrol
		HW_AND_SW_FLOWCONTROL		// Hardware combined with SW flowcontrol
	} flowControlEnum;

	//! Stop mode: Desription of how to report arrival of bytes in frames!
	typedef enum
	{
		STOP_TIMEOUT,		// Wait for timeout between arriving bytes
		STOP_FIELDLENGTH,	// Collect a specific amount of bytes before reporting
		STOP_SPECIALCHAR,	// Wait for a special character
		STOP_SYNCHWORDSIZE	// Wait for a snycword to arrive and then directly following the fieldsize
	} messageCompleteEnum;

	//! Reporting strategy: Report one of the events for flowcontrol or any
	typedef enum
	{
		REPORT_CTS,		// Report CTS set/unset
		REPORT_RLSD,	// Report RLSD set/unset
		REPORT_RING,	// Report Ring line state changed
		REPORT_ANY		// Report all events
	} reportEnum;

	//! Struct to store one buffer of bytes in the message queue
	struct messageQueueElement
	{
		//! Field of user data
		char* buffer;

		//! Number of bytes in the field
		int amBytes;

		//! Number of bytes maximum (amBytes <= amBytesMax)
		int amBytesMax;

		//! Message ID
		long messageID;

		//! Tiemout for this message (if it is sent!)
		DWORD timeout;

		//! Operator to compare message elements
		bool operator ==(long id){if(id == messageID) return(true);};
	
		//! Default constructor
		messageQueueElement(int maxSize, DWORD tOut = INFINITE):amBytes(0), amBytesMax(maxSize), messageID(0), timeout(tOut){buffer = new char[maxSize];};
		~messageQueueElement(){delete[](buffer);};
	};

private:

	//! Indication: Report strategy, see enum for description
	reportEnum stateReport;

	//! Shortcut name for COM port (e.g. COM1)
	std::string shortCut;

	//! Full name (WIN path description)
	std::string fName;

	//! Description of a COM port
	std::string description;

	//! Location obtained from registry
	std::string location;

	//! COM port ID (chronological)
	unsigned id;

	//! Handle (file descriptor) for COM port
	HANDLE portHandle;

	//============================================================
	// RUNTIME PARAMETERS  RUNTIME PARAMETERS  RUNTIME PARAMETERS
	//============================================================

	//! Runtime parameters: Baudrate
	int baudrate;

	//! Runtime parameters: Bits per symbol
	int bitsPerSymbol;

	//! Runtime parameters:  Use parity bit
	bool parity;

	//! Runtime parameters: Use stop bits
	int stopBits;

	//! Runtime parameters: Use HW flowcontrol
	bool hardwareFlow;

	//! Runtime parameters: Use SW flowcontrol
	bool softwareFlow;

	//============================================================

	//! Reference to parental application (RS-232-client)
	serialConnectionUseRef* callbackClass;

	//! State of the connection for current port
	connectionStateEnum conState;

	//! Enum specified for flow control (application)
	flowControlEnum flControl;

	//! Parental reference
	serialConnection* parentalRef;

	//! Maximum number of bytes to be sent to the port in one buffer (see constructor), applies for initSend!
	int maxBytesSend;

	//! Maimum number of bytes in one buffer to be reported to application, larger fields come in >1 buffer
	int maxBytesReceive;

	//! Message queue for sending (elements must be deleted whenever removed from list)
	std::list<messageQueueElement*> messageQueueSend;

	//! Message queue for receiving (elements must be deleted whenever removed from list)
	std::list<messageQueueElement*> messageQueueReceive;

	//! Safe acces to multithreading: Section for send queue
	CRITICAL_SECTION criticalSectionSendQueue;

	//! Safe acces to multithreading: Section for receive queue
	CRITICAL_SECTION criticalSectionReceiveQueue;

	//! Safe acces to multithreading: Section for changing the reporting strategy (may block for as long as one field is scanned!)
	CRITICAL_SECTION readBytesReceive;

	//! Events to report a new arriving message for send
	HANDLE eventMessageQueueFilledSend;

	//! Events to report a new arriving message for receive
	HANDLE eventMessageQueueFilledReceive;

	//! Thread handle for send
	HANDLE threadSend;

	//! Thread ID for send
	DWORD idThreadSend;

	//! Thread handle for receive
	HANDLE threadReceive;

	//! Thread ID for receive
	DWORD idThreadReceive;

	//! Thread handle to report to application
	HANDLE threadReceiveToApp;

	//! Thread ID to report to application
	DWORD idThreadReceiveToApp;

	//! Current messageID (incremented with every message)
	int messageIDReceive;

	//! Currently active element to select incoming bytes (not yet on message queue)
	messageQueueElement* newElmGlobal;

	//! Strategy for complete messages
	messageCompleteEnum completeIndication;

	//! Timeout to wait for further bytes if in timeout mode
	int timeoutReadSample;

	//! Length of the field to wait for bytes in fieldlength mode
	unsigned fieldLength;

	//! Stop character for a complete message
	char messageCompleteChar;

	int syncWord;
public:

	//! Constructor: Specify the references to parents and set maximum bufferlength
	serialElementDescriptor(unsigned id);
	
	//! Set this class to the initialized state
	bool initialize(unsigned maxCharMes, serialConnection* parRef, serialConnectionUseRef* applRef);

	//! Terminate this class: Set to initiali state
	bool terminate();

	//	serialElementDescriptor(const serialElementDescriptor& newElm);

	//! Activate the current com port (must have been opened previously)
	bool activateComPort(int bRate, 
						  int bits4Byte, int stopBitsEnum, 
						  int parityEnum, serialElementDescriptor::flowControlEnum enFlow,
						  messageCompleteEnum stopEnum, int toutReadSample, 
						  unsigned stopFieldLength, char charFieldStopint,
						  int syncword, reportEnum stRep);

	//! Deactivate the current com port
	bool deactivateComPort();

	//! Set full port name
	void setFName(const std::string& nm);

	//! Get full port name
	const std::string& getFName();

	//! Set shortcut (e.g. COM1) name
	void setShortCut(const std::string& nm);

	//! Get shortcut (e.g. COM1) name
	const std::string& getShortCut();

	//! Set the location name
	void setLocation(const std::string& nm){location = nm;};

	//! Get the location name
	const std::string& getLocation(){return(location);};

	//! Set the port description
	void setDescription(const std::string& nm){description = nm;};

	//! Get the port description
	const std::string& getDescription(){return(description);};

	//! Return the current state of the port 
	connectionStateEnum getConState(){return(conState);};
	
	//! Set the ID for current COM port (chronological numbers)
	void setID(unsigned pID){id = pID;};

	//! Open the current COM port
	bool openPort();

	//! Close the current COM port
	bool closePort();

	//! Send a message(must be CONNECTED)
	bool initSend(const char* message, int amBytes, long messageID, DWORD timeout);
	
	//! Thread awaiting arrival of messages to be send 
	DWORD sendThread();

	//! Thread awaiting arrival of messages from port
	DWORD receiveThread();

	//! Thread to separate application from HW routines (threads)
	DWORD receiveThreadToApplication();

	//! Return the number of messages in message queue
	int getAmountMessagesInQueue();

	//! Kill a message from queue (Number in list, always 0 for first message)
	bool killFromQueueID(unsigned int id);

	//! Kill a message from queue (Number is message ID)
	bool killFromQueueMessageID(long messageID);
	
	//! Set the stop mode to another value
	bool setStopModeTimeout(int newTimeout);
	
	//! Set the stop mode to another value
	bool setStopModeFieldLength(int newLength);

	//! Set the stop mode to another value
	bool setStopModeSpecialChar(char newChar);

	//! Obtain the exact status of the CTS line
	bool verifyCTSSet();

	//! Obtain the exact status of the DSR line
	bool verifyDSRSet();

	//! Obtain the exact status of the RING line
	bool verifyRINGSet();

	//! Obtain the exact status of the RLSD line
	bool verifyRLSDSet();

	//! Kill all elements in the message queue (for example if disconnection has occurred)
	int killElementsSendQueue();

	//! Destructor
	virtual ~serialElementDescriptor();
};

#endif
