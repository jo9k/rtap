/***********************************************************************
 * DESCRIPTION:
 * serial connection: Base class for handling all RS-232 events
 ***********************************************************************
 * This class provides functions to list up all RS-232 ports on the current PC.
 * Afterwards each serialElemementDescriptor element stands for one port that can be activated
 * and openend.
 * To the parental reference if nonzero, events are reached by calling associated functions passing purpose
 * and also the id from which port the message comes from.
 * IMPORTANT: It seems that Windows uses the name of an event to trigger an event to the 
 * waiting queue, that is, be sure that different names are used for each event!
 *
 * Copyright Hauke Krueger, IND, 2004, 02-06-04
 *********************************************************************/

#ifndef __SERIALCONNECTION_H__
#define __SERIALCONNECTION_H__

#include <string>
#include <vector>

#include "Windows.h"

#include "serialConnectionUseRef.h"
#include "serialElementDescriptor.h"

/**
 * Base class for serial communication, list up all ports available, 
 * provide functions to activate the ports, to open it and finally to
 * connect to another device (CTS, RTS etc.).
 *///==================================================================
class serialConnection  
{
private:

	typedef enum
	{
		PORTS_ON,
		PORTS_INITIALIZED
	} enumPortClass;

	enumPortClass portState;

	/** Maximum of characters reported at the same time. If buffers arrive longer than this
	 * the field is separated into segments of maximum maxChar characters*/
	int maxChar;

	//! Link to parental host, used for backward reference (abstract class)
	serialConnectionUseRef* parentalApp;

	//! List of all available COM ports
	std::vector<serialElementDescriptor*> elementList;

private:

	//! Scan for available com ports on the PC, this is just to change the inner states
	bool getAvailableComPorts();

public:

	//! Intialize this class, set parental reference, set maxChar value for buffer reporting
	bool initialize(int maxChar, serialConnectionUseRef* parRef);

	//! Terminate, opposite of initialize
	bool terminate();

	//! Open the port with a specific ID
	bool openPort(unsigned id);

	//! Close port: Opposite of openPort
	bool closePort(unsigned id);

	//! Activate a specific port whenever it is open, there is a default setup which need not be used
	bool activateComPort(unsigned id, int baudrate = 38400, int bites4Byte = 8, 
		int stopBitsEnum = ONESTOPBIT, int parity = NOPARITY,
		serialElementDescriptor::flowControlEnum enFlow = serialElementDescriptor::HW_FLOWCONTROL,
		serialElementDescriptor::messageCompleteEnum stopConstr = serialElementDescriptor::STOP_TIMEOUT,
		int timeoutReadSample = 200, int fieldLength = 4, char stopChar = '\r',
		int syncword = 0x00000000, serialElementDescriptor::reportEnum stRep = serialElementDescriptor::REPORT_ANY);

	//! Function to deactivate a COM port (opposite of activate)
	bool deactivateComPort(unsigned id);

	//! Initialize a send process: Pass ID of port, pass message as character buffer, length, a messageID and a timeout for message
	bool initSend(unsigned portID, const char* message, int amBytes, long messageID, DWORD timeout = INFINITE);

	//! Reporting buffers will wait for a specific stop signal, timeout is the criterion right here
	bool setStopModeTimeout(unsigned portID, int newTimeout);
	
	//! Reporting buffers will wait for a specific stop signal, length of the field is the criterion right here
	bool setStopModeFieldLength(unsigned portID,int newLength);

	//! Reporting buffers will wait for a specific stop signal, special character is the criterion right here
	bool setStopModeSpecialChar(unsigned portID, char newChar);

	//! Return the amount of available com ports
	int getAmountAvailableComPorts();

	//! Return the shortcut name for comPort with id
	const char* getShortcutComPort(unsigned id);

	//! Return the name for comPort with id
	const char* getNameComPort(unsigned id);

	//! Return the descriptive name for comPort with id
	const char* getDescriptionComPort(unsigned id);

	//! Return the location name for comPort with id
	const char* getLocationComPort(unsigned id);

	//! Function to test the CTS flag on portID port
	bool verifyCTSSet(unsigned portID);

	//! Function to test the DSR flag on portID port
	bool verifyDSRSet(unsigned portID);

	//! Function to test the RING flag on portID port
	bool verifyRINGSet(unsigned portID);

	//! Function to test the RLSD flag on portID port
	bool verifyRLSDSet(unsigned portID);

	//! Kill all messages that are currently in the queue to be send for portID
	int killAllMessagesQueue(unsigned ID);

	//! Constructor: Do nothing
	serialConnection();

	//! Destructor: Close all ports
	virtual ~serialConnection();

};

#endif 
