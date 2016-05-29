/******************************************************
 * Filename: IuartApplication.h						  *
 * Description: Interface class to define the receiver*
 * 				for the message dispatcher, currently *
 *				only support for RS232. 			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef __UARTAPPLICATION_H__
#define __UARTAPPLICATION_H__

#include "userDef.h"

class IuartDispatcher;

/*==================================================
 * Handle to provide callbacks for the dispatcher to
 * report incoming RS232 (and other) messages.
 * The application inerets these functions and can
 * process all incoming data.
 *///================================================
class IuartApplicationGeneric
{
public:
	
    // Callback for an incoming default message (already parsed)
    virtual void incomingDefaultMessage(DATATYPE_RS232_CHAR purpose, 
    		DATATYPE_RS232* fldLoad, UWORD16 numElements) = 0;

    // Callback for incoming streaming messages
    virtual void incomingStreamingMessage(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING]) = 0;

    // Callback to report that the currently started message has been completed
    virtual bool messageSendComplete(WORD32 messageID) = 0;

    // Function to issue errors to the application (on a word level)
    virtual void reportError(WORD32 errID) = 0;

    // Function to issue warnings to the application (on a word level)
    virtual void reportWarning(WORD32 warnID) = 0;
};

#endif
