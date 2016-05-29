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

#include "assert.h"
#include "serialELementDescriptor.h"
#include "serialConnection.h"
#include "serialConnectionUseRef.h"

/**
 * Start thread used for message sending, just return to the current class (private data)
 *///====================================================================================
DWORD WINAPI
global_startSend(void* param)
{
	serialElementDescriptor* this_pointer = (serialElementDescriptor*) param;
	return(this_pointer->sendThread());
}

/**
 * Start thread used for message reception, just return to the current class (private data)
 *///====================================================================================
DWORD WINAPI
global_startReceive(void* param)
{
	serialElementDescriptor* this_pointer = (serialElementDescriptor*) param;
	return(this_pointer->receiveThread());
}

/**
 * Start the message observing thread. As soon as a message has been completely received,
 * the message is reported to the host.
 *///====================================================================================
DWORD WINAPI
global_receiveToApplication(void* param)
{
	serialElementDescriptor* this_pointer = (serialElementDescriptor*) param;
	return(this_pointer->receiveThreadToApplication());
}

/**
 * The constructor sets up the default values for the current com port. These values
 * can be overwritten by activating the port or using appropriate function calls
 *///=================================================================================
serialElementDescriptor::serialElementDescriptor(unsigned idPort): 
		stateReport(REPORT_ANY),
		baudrate(0), bitsPerSymbol(0), callbackClass(NULL), 
		conState(RS232_PORT_NOTCONNECTED),
		flControl(NO_FLOWCONTROL), parentalRef(NULL), 
		id(idPort), maxBytesReceive(0), maxBytesSend(0),
		messageIDReceive(0), newElmGlobal(NULL)
{	
}

/** 
 * Initialize the class.
 *///=====================================================
bool
serialElementDescriptor::initialize(unsigned maxCharMes, serialConnection* parRef, serialConnectionUseRef* applRef)
{
	stateReport = REPORT_ANY;
	baudrate = 0;
	bitsPerSymbol = 0;
	callbackClass = applRef;
	conState = RS232_PORT_NOTCONNECTED;
	flControl = NO_FLOWCONTROL;
	parentalRef = parRef;
	maxBytesReceive = maxCharMes;
	maxBytesSend = maxCharMes;
	messageIDReceive = 0;
	newElmGlobal = NULL;
	return(true);
}

/**
 * Terminate function: Set port to initial state.
 *///=============================================================
bool
serialElementDescriptor::terminate()
{
	if(conState == RS232_PORT_CONNECTED)
		this->deactivateComPort();
	if(conState == RS232_PORT_OPEN)
		this->closePort();

	stateReport = REPORT_ANY;
	baudrate = 0;
	bitsPerSymbol = 0;
	callbackClass = NULL;
	conState = RS232_PORT_DONTUSE;
	flControl = NO_FLOWCONTROL;
	parentalRef = NULL;
	maxBytesReceive = 0;
	maxBytesSend = 0;
	messageIDReceive = 0;
	newElmGlobal = NULL;
	return(true);
}

/** 
 * Copy constructor: Used whenever pushing back an element to the vector. Should have used 
 * pointer though!! OK, I just replaced it by pointer, will keep this function anyway
serialElementDescriptor::serialElementDescriptor(const serialElementDescriptor& newElm)
{
	stateReport = newElm.stateReport;
	shortCut = newElm.shortCut;
	fName = newElm.fName; 
	description = newElm.description;
	location = newElm.location;
	id = newElm.id;
	portHandle = newElm.portHandle;
	baudrate = newElm.baudrate;
	bitsPerSymbol = newElm.bitsPerSymbol;
	parity = newElm.parity;
	stopBits = newElm.stopBits;
	hardwareFlow = newElm.hardwareFlow;
	softwareFlow = newElm.softwareFlow;
	callbackClass = newElm.callbackClass;
	conState = newElm.conState;
	flControl = newElm.flControl;
	parentalRef = newElm.parentalRef;
	maxBytesSend = newElm.maxBytesSend;
	maxBytesReceive = newElm.maxBytesReceive;
	messageIDReceive = newElm.messageIDReceive;
	newElmGlobal = NULL;
	if(conState == RS232_PORT_CONNECTED)
	{
		assert(messageQueueSend.size()==0);
		assert(messageQueueReceive.size()==0);
	}
}
*/

/**
 * No tidy up or something required right here
 *///========================================================
serialElementDescriptor::~serialElementDescriptor()
{
}

/**
 * Set the name for the current element
 *///======================================================
void
serialElementDescriptor::setFName(const std::string& nm)
{
	fName = nm;
}

/**
 * Get the name for the current element
 *///======================================================
const std::string&
serialElementDescriptor::getFName()
{
	return(fName);
}

/**
 * Set the short cut name (e.g. COM1) for the current element
 *///======================================================
void
serialElementDescriptor::setShortCut(const std::string& nm)
{
	shortCut = nm;
}

/**
 * Get the short cut name (e.g. COM1) for the current element
 *///======================================================
const std::string&
serialElementDescriptor::getShortCut()
{
	return(shortCut);
}

/**
 * Activate the COM port with a specific rate etc. This will also set the CTS if required!
 * This function requires the COM port to be open from previously.
 *///=======================================================================================
bool
serialElementDescriptor::activateComPort(int bRate, int bits4Byte, int stopBitsEnum, 
										 int parityEnum, serialElementDescriptor::flowControlEnum enFlow,
										 messageCompleteEnum stopEnum, int toutReadSample, 
										 unsigned stopFieldLength, char charFieldStop, int syncW,
										 reportEnum stRep)
{
	// If port is open, set the DCBInit struct and pass to device handle
	if(conState == RS232_PORT_OPEN)
	{
		DCB rs232SetupElement;
		rs232SetupElement.DCBlength = sizeof(DCB);


		GetCommState(portHandle , &rs232SetupElement);

		// Set the specified Baudrate
		baudrate = bRate;
		rs232SetupElement.BaudRate = baudrate;
		

		// Always true
		rs232SetupElement.fBinary = 1;

		
		if(parityEnum == NOPARITY)
		{
			rs232SetupElement.fParity = 0;
			parity = false;
		}
		else
		{
			rs232SetupElement.fParity = 1;
			parity = true;
		}

		// CTS: I DONT KNOW 
		rs232SetupElement.fOutxCtsFlow = true;
			
		if((enFlow == NO_FLOWCONTROL)||
			(enFlow == SW_FLOWCONTROL))
		{
			flControl = enFlow;
			rs232SetupElement.fOutxDsrFlow = false;
		}
		else
		{
			flControl = enFlow;
			rs232SetupElement.fOutxDsrFlow = false;
		}

		// I DONT KNOW
		rs232SetupElement.fDtrControl = DTR_CONTROL_DISABLE;

		// I DONT KNOW
		rs232SetupElement.fDsrSensitivity = false;

		// I DONT KNOW
		rs232SetupElement.fTXContinueOnXoff = true;

			
		if((enFlow == SW_FLOWCONTROL)||(enFlow == HW_AND_SW_FLOWCONTROL))
		{
			rs232SetupElement.fOutX = true;
			rs232SetupElement.fInX = true;
		}
		else
		{
			rs232SetupElement.fOutX = false;
			rs232SetupElement.fInX = false;
		}

		// I DONT KNOW
		rs232SetupElement.fErrorChar = 0;
		rs232SetupElement.fNull = false;
		rs232SetupElement.fRtsControl = RTS_CONTROL_HANDSHAKE;
		rs232SetupElement.fAbortOnError = true;
		rs232SetupElement.wReserved = 0;

		// As specified with hyperterminal
		rs232SetupElement.XonLim = 2048;
		rs232SetupElement.XoffLim = 512;

		bitsPerSymbol = bits4Byte;
		rs232SetupElement.ByteSize = bits4Byte;

		rs232SetupElement.Parity = parityEnum;
		stopBits = stopBitsEnum;
		rs232SetupElement.StopBits = stopBitsEnum;

		// As provided by hyperterminal
		rs232SetupElement.XonChar = 17;
		rs232SetupElement.XoffChar = 19;

		// I DONT KNOW
		rs232SetupElement.ErrorChar = 0;
		rs232SetupElement.EofChar = 0;
		rs232SetupElement.EvtChar = 0;

		if(!SetCommState(portHandle , &rs232SetupElement))
		{
			DWORD sysError = GetLastError();
			CloseHandle(portHandle);
			return(false);
		}
		
		timeoutReadSample = toutReadSample;
		completeIndication = stopEnum;
		fieldLength = stopFieldLength;
		messageCompleteChar = charFieldStop;
		stateReport = stRep;
		this->syncWord = syncW;

		conState = RS232_PORT_CONNECTED;

		InitializeCriticalSection(&criticalSectionSendQueue);
		InitializeCriticalSection(&criticalSectionReceiveQueue);
		InitializeCriticalSection(&readBytesReceive);
		eventMessageQueueFilledSend = CreateEvent(NULL, true, false, ((std::string)"MESSAGE_Q_SEND" + shortCut).c_str());
		eventMessageQueueFilledReceive = CreateEvent(NULL, true, false, ((std::string)"MESSAGE_Q_RECEIVE" + shortCut).c_str());
		threadSend = CreateThread(NULL, 0, global_startSend, this, 0, &idThreadSend);

		// Create a new element to store arriving bytes in!
		newElmGlobal = new messageQueueElement(this->maxBytesReceive);
		threadReceive = CreateThread(NULL, 0, global_startReceive, this, 0, &idThreadReceive);
		threadReceiveToApp = CreateThread(NULL, 0, global_receiveToApplication, this, 0, &idThreadReceiveToApp);
	}
	else
	{
		return(false);
	}
	return(true);
}

/**
 * Function to set a port into the disconnected state, that is: 
 * Stop all threads that await signals etc. and also stop the critical sections etc.
 *///================================================================================
bool 
serialElementDescriptor::deactivateComPort()
{
	if(conState == RS232_PORT_CONNECTED)
	{
		TerminateThread(threadSend, 0);
		TerminateThread(threadReceive, 0);
		TerminateThread(threadReceiveToApp, 0);
		CloseHandle(eventMessageQueueFilledSend );
		CloseHandle(eventMessageQueueFilledReceive);
		DeleteCriticalSection(&criticalSectionSendQueue);
		DeleteCriticalSection(&criticalSectionReceiveQueue);
		DeleteCriticalSection(&readBytesReceive);
		delete(newElmGlobal);
		conState = RS232_PORT_OPEN;
		return(true);
	}
	return(false);
}

/**
 * Function to set a port into theclose state: Release the handle value
 * and set the state to closed port.
 *///================================================================================
bool
serialElementDescriptor::closePort()
{
	if(conState == RS232_PORT_OPEN)
	{
		CloseHandle(portHandle);
		portHandle = INVALID_HANDLE_VALUE;
		conState = RS232_PORT_NOTCONNECTED;
		return(true);
	}
	return(false);
}

/**
 * Function to open the port associated with this element
 *///=========================================================
bool
serialElementDescriptor::openPort()
{
	if(conState == RS232_PORT_NOTCONNECTED)
	{
		portHandle  = CreateFile(shortCut.c_str(),
				GENERIC_WRITE|GENERIC_READ,
				0, NULL, OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED, NULL);
	
		if(portHandle == INVALID_HANDLE_VALUE)
			return(false);

		conState = RS232_PORT_OPEN;
		return(true);
	}
	return(false);
}

/**
 * Function to send a (byte-)message via com port. Of course this should only be tried in
 * connected state. Furthermore a message to be send should never be larger than the max number of bytes
 * applied for messages (send in constructor!)!
 *///======================================================================================
bool 
serialElementDescriptor::initSend(const char* message, int amBytes, long messageID, DWORD timeout)
{
	if(conState != RS232_PORT_CONNECTED)
	{
		return(false);
	}

	if(amBytes > this->maxBytesSend)
		return(false);

	// Allocate new message element
	messageQueueElement* newElm = new messageQueueElement(this->maxBytesSend);

	// Copy the message
	memcpy(newElm->buffer, message, sizeof(char)*amBytes);
	newElm->amBytes = amBytes;
	newElm->messageID = messageID;
	newElm->timeout = timeout;

	// If there is some space left in queue, place the message in the message queue
	if(messageQueueSend.size() >= MESSAGE_QUEUE_SIZE_MAX)
		return(false);
	
	// Of course have safe access to the message queue in multithreading
	EnterCriticalSection(&criticalSectionSendQueue);
	messageQueueSend.push_back(newElm);
	SetEvent(eventMessageQueueFilledSend);
	LeaveCriticalSection(&criticalSectionSendQueue);

	return(true);
}
	
/**
 * Return the number of messages currently available in the message send queue.
 *///==============================================================================0
int 
serialElementDescriptor::getAmountMessagesInQueue()
{
	return(messageQueueSend.size());
}

/**
 * Kill a message with a specific ID from the mesage Queue.
 * Attention: ID is the number of the message starting with 0!
 *///============================================================================
bool
serialElementDescriptor::killFromQueueID(unsigned int id)
{
	unsigned i;
	bool reachedEnd = false;
	bool success = false;
	std::list<messageQueueElement*>::iterator it;
	EnterCriticalSection(&criticalSectionSendQueue);
	it = messageQueueSend.begin();
	if(it != messageQueueSend.end())
	{
		for(i = 0; i < id; i++)
		{
			it++;
			if(it == messageQueueSend.end())
			{
				reachedEnd = true;
				break;
			}
		}
		if(!reachedEnd)
		{
			messageQueueSend.erase(it);
			success = true;
		}
	}
	LeaveCriticalSection(&criticalSectionSendQueue);
	return(success);
}

/**
 * Kill a message with a specific ID from the mesage Queue.
 * Attention: Kill the first one with the ID, applying IDs is 
 * in the response of the user of the class!
 *///==========================================================
bool 
serialElementDescriptor::killFromQueueMessageID(long messageID)
{
	bool reachedEnd = false;
	bool success = false;
	std::list<messageQueueElement*>::iterator it;

	EnterCriticalSection(&criticalSectionSendQueue);
	it = messageQueueSend.begin();
	while(it != messageQueueSend.end())
	{
		if((*it)->messageID == messageID)
		{
			messageQueueSend.erase(it);
			success = true;
			break;
		}
	}
	LeaveCriticalSection(&criticalSectionSendQueue);

	return(success);
}

/**
 * Class internal implementation of the sending thread: Await 
 * events that indicate outgoing messages. If one has arrived, 
 * copy the bytes to the hardware and wait for overlapping to
 * terminate applying the specified timeout.
 *///========================================================
DWORD
serialElementDescriptor::sendThread()
{
	DWORD res;
	DWORD resWait;

	OVERLAPPED osSend = {0};
	HANDLE eventSendOL = CreateEvent(0, true, false, ("OL_SEND" + shortCut).c_str());
	osSend.hEvent = eventSendOL;

	messageQueueElement* elm = NULL;
	int listSize;
	unsigned long written;
	std::list<messageQueueElement*>::iterator it;

	// While in connected state only!
	while(conState == RS232_PORT_CONNECTED)
	{
		// Wait for event to be triggered
		res = WaitForSingleObject(eventMessageQueueFilledSend, INFINITE);
		if(res == WAIT_OBJECT_0)
		{
			listSize = 1;
			while(listSize > 0)
			{
				elm = NULL;
				EnterCriticalSection(&criticalSectionSendQueue);

				listSize = messageQueueSend.size();
				if(listSize != 0)
				{
					it = messageQueueSend.begin();
					elm = *it;
					messageQueueSend.pop_front();
					listSize = messageQueueSend.size();
					if(listSize == 0)
					{
						ResetEvent(eventMessageQueueFilledSend);
					}
				}
				LeaveCriticalSection(&criticalSectionSendQueue);

				
				if(elm)
				{
					// Write bytes to HW
					if(!WriteFile(portHandle, elm->buffer, elm->amBytes, &written, &osSend))
					{
							resWait = WaitForSingleObject(eventSendOL, elm->timeout);
							if(resWait == WAIT_OBJECT_0)
							{
								// If message has been completely written in time for timeout, indicate success
								if(callbackClass)
									callbackClass->messageComplete(id, elm->buffer, elm->amBytes, elm->messageID);
							}
							else
							{
								// If message has NOT been completely written in time for timeout, indicate failure
								if(callbackClass)
									callbackClass->messageTimeout(id, elm->buffer, elm->amBytes, elm->messageID);
							}
					}
					else
					{
						if(written == elm->amBytes)
						{
							// If message has been completely written in time for timeout, indicate success
							if(callbackClass)
								callbackClass->messageComplete(id, elm->buffer, elm->amBytes, elm->messageID);
						}
						else
						{
							// Right here we have an error
							if(callbackClass)
								callbackClass->error(id, serialConnectionUseRef::ERROR_SEND_FAILED);
						}
					}

					// Delete the element stored in the message queue, as it has previously been removed from queue
					delete(elm);
				}
			}
		}
	}
	return(0);
}

/**
 * Function implementation of the read thread in class specfic environment
 *///========================================================================0
DWORD
serialElementDescriptor::receiveThread()
{
	typedef enum
	{
		SCAN_FOR_SYNCWORD,
		SCAN_FOR_LENGTH,
		SCAN_FOR_PARAMETERS
	} SYNCWORDSTATE;

	SYNCWORDSTATE syncwordState = SCAN_FOR_SYNCWORD;
	int wordSync = 0;
	char readChar;
	int numberWords = 0;
	int byteCounter = 0;
	int numberBytes = 0;

	DWORD read = 0;
	bool resRead;
	
	// Create the overlapped events
	OVERLAPPED osObservate = {0};
	HANDLE eventObserve = CreateEvent(0, true, false, ("OS_OBSERVE" + shortCut).c_str());
	
	OVERLAPPED osRead = {0};
	HANDLE eventRead = CreateEvent(0, true, false, ("OS_READ" + shortCut).c_str());

	DWORD evtMask;
	
	unsigned ind;

	int valuesInQueue;
	DWORD errorWord;
	COMSTAT staErr;
	
	osObservate.hEvent = eventObserve;
	osRead.hEvent = eventRead;

	// Expect indications for all handshake control and arriving bytes
	SetCommMask(portHandle, EV_RXCHAR|EV_RING|EV_CTS|EV_RLSD);

	// This critical section protectes that changing the receive-mode for buffers 
	// (that is timeout, stop char or bufferlength) is only changed on the
	// bound of a new buffer. This section is only blocked in the active phases 
	// in order to avaoid blocking behavior
	EnterCriticalSection(&readBytesReceive);

	while(conState == RS232_PORT_CONNECTED)
	{
		evtMask = 0;
		LeaveCriticalSection(&readBytesReceive);

		// Do notz block critical section
		if(!WaitCommEvent(portHandle, &evtMask, &osObservate))
		{
			WaitForSingleObject(osObservate.hEvent, INFINITE);
		}

		// Now enter the critical section as long as bytes are read from device
		EnterCriticalSection(&readBytesReceive);

		// If base class wants to get any event reported, report the triggered event
		if(stateReport == REPORT_ANY)
		{
			if(callbackClass)
				callbackClass->indicateEvent(id, evtMask);
		}

		// Now process the single event..
		if(evtMask|EV_RXCHAR)
		{
			resRead = true;
			ind = 0;
			int readVals;

			// Obtain the input FIFO status: How many bytes are ready to be read?
			ClearCommError(portHandle, &errorWord, &staErr);
			valuesInQueue = staErr.cbInQue;			
			
			switch(this->completeIndication)
			{

			// Case 0: A SYNCWORD is scanned for in order to find beginning of
			// a message. Directly following is the amount of 32-Bit-Words
			// that will arrive. After these values are stored, the scanner looks 
			// for the next syncword.
			case serialElementDescriptor::STOP_SYNCHWORDSIZE:
				while(valuesInQueue)
				{
					switch(syncwordState)
					{
					case SCAN_FOR_SYNCWORD:
						if(!ReadFile(portHandle, (((byte*)&wordSync)+3), 1, &read, &osRead))
						{
							assert(0);
						}
						else
						{
							if(wordSync == this->syncWord)
							{
								syncwordState = SCAN_FOR_LENGTH;

								//! We need at least 8 characters for syncword and length!
								// Be sure that the syncword fits in!
								assert((newElmGlobal->amBytesMax-newElmGlobal->amBytes) >= 8);
								memcpy(&(newElmGlobal->buffer[newElmGlobal->amBytes]), &wordSync, sizeof(wordSync));
								newElmGlobal->amBytes += sizeof(wordSync);
								byteCounter = 0;
								numberWords = 0;
							}
							else
							{
								wordSync = wordSync >> 8;
							}

						}
						if((--valuesInQueue) == 0)
						{
							ClearCommError(portHandle, &errorWord, &staErr);
							valuesInQueue = staErr.cbInQue;			
						}
						break;
					case SCAN_FOR_LENGTH:
						readVals = 4-byteCounter;
						if(valuesInQueue < readVals)
						{
							readVals = valuesInQueue;
						}
						if(!ReadFile(portHandle, (((byte*)&numberWords) + byteCounter), readVals, &read, &osRead))
						{
							assert(0);
						}
						else
						{
							byteCounter += readVals;
						}
						if(byteCounter == 4)
						{
							syncwordState = SCAN_FOR_PARAMETERS;

							//! We need at least 8 characters for syncword and length!
							// Be sure that the syncword fits in!
							assert((newElmGlobal->amBytesMax-newElmGlobal->amBytes) >= 4);

							// Be sure to add at least one word to each header, otherwhise a deadlock is possible
							if(numberWords >0)
								assert((newElmGlobal->amBytesMax-newElmGlobal->amBytes) >= 8);

							memcpy(&(newElmGlobal->buffer[newElmGlobal->amBytes]), &numberWords, sizeof(wordSync));
							newElmGlobal->amBytes += sizeof(numberWords);
							numberBytes = numberWords * sizeof(int);
						}
						ClearCommError(portHandle, &errorWord, &staErr);
						valuesInQueue = staErr.cbInQue;			
						break;
					case SCAN_FOR_PARAMETERS:
						if(newElmGlobal->amBytes == 0)
						{
							// First case: The final element was too short for complete message
							// Add syncword and -1 as length indication for double field
							assert((newElmGlobal->amBytesMax-newElmGlobal->amBytes) >= 12);
							memcpy(&(newElmGlobal->buffer[newElmGlobal->amBytes]), &wordSync, sizeof(wordSync));
							newElmGlobal->amBytes += sizeof(wordSync);
							
							// Indicate the following bytes as following partial of a message
							numberWords = -1;
							memcpy(&(newElmGlobal->buffer[newElmGlobal->amBytes]), &numberWords, sizeof(wordSync));
							newElmGlobal->amBytes += sizeof(numberWords);
						}
						readVals = numberBytes;

						// If element would be too short..
						if((newElmGlobal->amBytesMax - newElmGlobal->amBytes) < readVals)
						{
							readVals = newElmGlobal->amBytesMax - newElmGlobal->amBytes;
						}

						// If there arent that many bytes available..
						if(valuesInQueue < readVals)
						{
							readVals = valuesInQueue;
						}
						if(!ReadFile(portHandle, &newElmGlobal->buffer[newElmGlobal->amBytes], readVals, &read, &osRead))
						{
							assert(0);
						}
						else
						{
							newElmGlobal->amBytes+= readVals;
							numberBytes -= readVals;
						}
						ClearCommError(portHandle, &errorWord, &staErr);
						valuesInQueue = staErr.cbInQue;			
						break;
					}
					if(syncwordState == SCAN_FOR_PARAMETERS)
					{
						if((newElmGlobal->amBytesMax == newElmGlobal->amBytes)||(numberBytes == 0))
						{
							// If there have no further bytes arrived, place message into receive queue
							newElmGlobal->messageID = messageIDReceive++;
							EnterCriticalSection(&criticalSectionReceiveQueue);
							messageQueueReceive.push_back(newElmGlobal);
							SetEvent(eventMessageQueueFilledReceive);
							LeaveCriticalSection(&criticalSectionReceiveQueue);
							newElmGlobal = new messageQueueElement(this->maxBytesReceive);
						}
						if(numberBytes == 0)
						{
							// Wait for the next syncword!
							syncwordState = SCAN_FOR_SYNCWORD;
						}
					}
				}
				break;
			case serialElementDescriptor::STOP_TIMEOUT:

				// Case 1: Timeout inbetween arriving bytes is used to find a separation character
				// OPEN ISSUE: How can a user see where is a separation based on timeout and that based on field bounds?
				while(valuesInQueue)
				{
					// Read as many values as fit in the message queue element
					if(newElmGlobal->amBytes + valuesInQueue > newElmGlobal->amBytesMax)
					{
						readVals = newElmGlobal->amBytesMax - newElmGlobal->amBytes;
					}
					else
					{
						readVals = valuesInQueue;
					}
					valuesInQueue -= readVals;

					if(!ReadFile(portHandle, &(newElmGlobal->buffer[newElmGlobal->amBytes]), readVals, &read, &osRead))
					{
						assert(0);
					}
					else
					{
						newElmGlobal->amBytes += read;

						// If field for incoming is complete, pass the buffer to the message queue
						if(newElmGlobal->amBytes == newElmGlobal->amBytesMax)
						{
							newElmGlobal->messageID = messageIDReceive++;
							EnterCriticalSection(&criticalSectionReceiveQueue);
							messageQueueReceive.push_back(newElmGlobal);
							SetEvent(eventMessageQueueFilledReceive);
							LeaveCriticalSection(&criticalSectionReceiveQueue);
							newElmGlobal = new messageQueueElement(this->maxBytesReceive);
						}	
					}

					// If there have been further bytes in the queue read those first
					if(valuesInQueue)
						continue;

					// If there have been no more bytes in queue, apply timeout
					Sleep(timeoutReadSample);

					// Obtain number of newly arrived bytes
					ClearCommError(portHandle, &errorWord, &staErr);
					valuesInQueue = staErr.cbInQue;			
					if(valuesInQueue == 0)
					{
						// If there have no further bytes arrived, place message into receive queue
						newElmGlobal->messageID = messageIDReceive++;
						EnterCriticalSection(&criticalSectionReceiveQueue);
						messageQueueReceive.push_back(newElmGlobal);
						SetEvent(eventMessageQueueFilledReceive);
						LeaveCriticalSection(&criticalSectionReceiveQueue);
						newElmGlobal = new messageQueueElement(this->maxBytesReceive);
					}
				}
				break;
			case serialElementDescriptor::STOP_FIELDLENGTH:

				// Stop to collect bytes until the field has reached a certain length
				while(valuesInQueue)
				{
					if(newElmGlobal->amBytes + valuesInQueue > (signed)this->fieldLength)
					{
						readVals = fieldLength - newElmGlobal->amBytes;
					}
					else
					{
						readVals = valuesInQueue;
					}

					valuesInQueue -= readVals;

					if(!ReadFile(portHandle, &(newElmGlobal->buffer[newElmGlobal->amBytes]), readVals, &read, &osRead))
					{
						assert(0);
					}
					else
					{
						newElmGlobal->amBytes += read;

						// If a message has been completed pass it to the message queue
						if(newElmGlobal->amBytes == fieldLength)
						{
							newElmGlobal->messageID = messageIDReceive++;
							EnterCriticalSection(&criticalSectionReceiveQueue);
							messageQueueReceive.push_back(newElmGlobal);
							SetEvent(eventMessageQueueFilledReceive);
							LeaveCriticalSection(&criticalSectionReceiveQueue);
							newElmGlobal = new messageQueueElement(this->maxBytesReceive);
						}	
					}
					if(valuesInQueue)
						continue;

					ClearCommError(portHandle, &errorWord, &staErr);
					valuesInQueue = staErr.cbInQue;			
				}
				break;
			case serialElementDescriptor::STOP_SPECIALCHAR:

				// Scanning input until a specific stop character has been scanned
				while(valuesInQueue)
				{
					// Read byte by byte and compare it
					readVals = 1;
					if(!ReadFile(portHandle, &(newElmGlobal->buffer[newElmGlobal->amBytes]), readVals, &read, &osRead))
					{
						assert(0);
					}
					else
					{
						if(newElmGlobal->buffer[newElmGlobal->amBytes] == this->messageCompleteChar)
						{
							newElmGlobal->amBytes += read;
							newElmGlobal->messageID = messageIDReceive++;
							EnterCriticalSection(&criticalSectionReceiveQueue);
							messageQueueReceive.push_back(newElmGlobal);
							SetEvent(eventMessageQueueFilledReceive);
							LeaveCriticalSection(&criticalSectionReceiveQueue);
							newElmGlobal = new messageQueueElement(this->maxBytesReceive);
						}	
						else
						{
							
							newElmGlobal->amBytes += read;
							if(newElmGlobal->amBytes == newElmGlobal->amBytesMax)
							{
								newElmGlobal->messageID = messageIDReceive++;
								EnterCriticalSection(&criticalSectionReceiveQueue);
								messageQueueReceive.push_back(newElmGlobal);
								SetEvent(eventMessageQueueFilledReceive);
								LeaveCriticalSection(&criticalSectionReceiveQueue);
								newElmGlobal = new messageQueueElement(this->maxBytesReceive);
							}
						}
					}
					valuesInQueue -= readVals;
					if(valuesInQueue)
						continue;
					ClearCommError(portHandle, &errorWord, &staErr);
					valuesInQueue = staErr.cbInQue;			
				}
				break;
			}
		}

		// All other events are handled on a reporting level
		if(stateReport == REPORT_RING)
		{
			if(evtMask|EV_RING)
			{
				if(callbackClass)
				{
					callbackClass->indicateRing(this->id);
				}
			}
		}

		// All other events are handled on a reporting level
		if(stateReport == REPORT_RLSD)
		{
			if(evtMask|EV_RLSD)
			{
				if(callbackClass)
					callbackClass->indicateRLSDChanged(this->id);
			}
		}

		// All other events are handled on a reporting level
		if(stateReport == REPORT_CTS)
		{
			if(evtMask|EV_CTS)
			{
				if(callbackClass)
					callbackClass->indicateCTSChanged(this->id);
			}
		}
	}
	LeaveCriticalSection(&readBytesReceive);
	return(0);
}

/**
 * Return the state of the CTS line
 *///===================================
bool
serialElementDescriptor::verifyCTSSet()
{
	DWORD modemState;
	if(GetCommModemStatus(portHandle, &modemState))
	{
		return((modemState&MS_CTS_ON)!= 0);
	}
	return(false);
}

/**
 * Return the state of the DSR line
 *///===================================
bool
serialElementDescriptor::verifyDSRSet()
{
	DWORD modemState;
	if(GetCommModemStatus(portHandle, &modemState))
	{
		return((modemState&MS_DSR_ON) != 0);
	}
	return(false);
}

/**
 * Return the state of the RING line
 *///===================================
bool
serialElementDescriptor::verifyRINGSet()
{
	DWORD modemState;
	if(GetCommModemStatus(portHandle, &modemState))
	{
		return((modemState&MS_RING_ON) != 0);
	}
	return(false);
}

/**
 * Return the state of the RLSD line
 *///===================================
bool
serialElementDescriptor::verifyRLSDSet()
{
	DWORD modemState;
	if(GetCommModemStatus(portHandle, &modemState))
	{
		return((modemState&MS_RLSD_ON) != 0);
	}
	return(false);
}

/** 
 * Thread to asyncly connect the input thread to the application:
 * If there is a new element in the message queue that can be 
 * passed to the application. Why do I use another message queue and
 * single thread to buffer messages? To react to external events as 
 * soon as possible. Indications of events are DIRECTLY reported however 
 * (so the application must ensure that indications ay be called in a realtime 
 * context! If the buffers arriving are not processed in realtime they will only 
 * be lost in software (missing buffers).
 *///=======================================================================
DWORD 
serialElementDescriptor::receiveThreadToApplication()
{
	messageQueueElement* elm = NULL;
	int listSize;
	std::list<messageQueueElement*>::iterator it;

	while(conState == RS232_PORT_CONNECTED)
	{
		if(WaitForSingleObject(eventMessageQueueFilledReceive, INFINITE) == WAIT_OBJECT_0)
		{
			listSize = 1;
			while(listSize > 0)
			{
				elm = NULL;
				EnterCriticalSection(&criticalSectionReceiveQueue);

				listSize = messageQueueReceive.size();
				if(listSize != 0)
				{
					listSize = 0;
					it = messageQueueReceive.begin();
					elm = *it;
					messageQueueReceive.pop_front();
					listSize = messageQueueReceive.size();
					if(listSize == 0)
					{
						ResetEvent(eventMessageQueueFilledReceive);
					}
				}
				LeaveCriticalSection(&criticalSectionReceiveQueue);

				// Report the arriving message to the application
				callbackClass->messageReceived(id, elm->messageID, elm->buffer, elm->amBytes);
				if(elm)
				{
					delete(elm);
				}
			}
		}
	}
	return(0);
}
				
/**
 * Modify the STOP mode of buffer reports to timeout methodology
 *///================================================================
bool
serialElementDescriptor::setStopModeTimeout(int newTimeout)
{
	if(	conState != RS232_PORT_CONNECTED)
		return(false);
	if(!newElmGlobal)
		return(false);
	EnterCriticalSection(&readBytesReceive);
	if(newElmGlobal->amBytes != 0)
	{
		EnterCriticalSection(&criticalSectionReceiveQueue);
		messageQueueReceive.push_back(newElmGlobal);
		SetEvent(eventMessageQueueFilledReceive);
		LeaveCriticalSection(&criticalSectionReceiveQueue);
		newElmGlobal = new messageQueueElement(this->maxBytesReceive);
	}
	LeaveCriticalSection(&readBytesReceive);
	timeoutReadSample = newTimeout;
	completeIndication = STOP_TIMEOUT;
	return(true);
}

/**
 * Set stop mode for buffer reports to field length
 *///================================================
bool
serialElementDescriptor::setStopModeFieldLength(int newLength)
{
	if(	conState != RS232_PORT_CONNECTED)
		return(false);

	if(!newElmGlobal)
		return(false);
	if(newLength <= this->maxBytesReceive)
	{
		EnterCriticalSection(&readBytesReceive);
		if(newElmGlobal->amBytes != 0)
		{
			EnterCriticalSection(&criticalSectionReceiveQueue);
			messageQueueReceive.push_back(newElmGlobal);
			SetEvent(eventMessageQueueFilledReceive);
			LeaveCriticalSection(&criticalSectionReceiveQueue);
			newElmGlobal = new messageQueueElement(this->maxBytesReceive);
		}
		LeaveCriticalSection(&readBytesReceive);
		fieldLength = newLength;
		completeIndication = STOP_FIELDLENGTH;
		return(true);
	}
	return(false);
}

/**
 * Set the stop mode for buffer reporting to 
 * stop character mode (awaiting a specific character)
 *///=====================================================
bool
serialElementDescriptor::setStopModeSpecialChar(char newChar)
{
	if(	conState != RS232_PORT_CONNECTED)
		return(false);
	if(!newElmGlobal)
		return(false);
	EnterCriticalSection(&readBytesReceive);
	if(newElmGlobal->amBytes != 0)
	{
		EnterCriticalSection(&criticalSectionReceiveQueue);
		messageQueueReceive.push_back(newElmGlobal);
		SetEvent(eventMessageQueueFilledReceive);
		LeaveCriticalSection(&criticalSectionReceiveQueue);
		newElmGlobal = new messageQueueElement(this->maxBytesReceive);
	}
	LeaveCriticalSection(&readBytesReceive);
	messageCompleteChar = newChar;
	completeIndication = STOP_SPECIALCHAR;
	return(true);
}

/**
 * Function to kill all elements from the send queue
 *///=====================================================
int 
serialElementDescriptor::killElementsSendQueue()
{
	int res = 0;
	messageQueueElement* elm;
	std::list<messageQueueElement*>::iterator it;

	EnterCriticalSection(&criticalSectionSendQueue);
	while (messageQueueSend.size())
	{
		it = messageQueueSend.begin();
		if(it != messageQueueSend.end())
		{
			elm = *it;
			if(callbackClass)
				callbackClass->messageDiscarded(this->id, elm->buffer, elm->amBytes, elm->messageID);
			messageQueueSend.pop_front();
			res++;
			free(elm);
		}
	}
	LeaveCriticalSection(&criticalSectionSendQueue);
	return(res);
}