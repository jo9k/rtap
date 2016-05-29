/******************************************************
 * Filename: uartReceiverGeneric.h					  *
 * Description: Class to realize the uart receiver    *
 *				default functionality. Incoming 	  *
 *				messages which are generic are passed *
 *				to the receiver. At the moment, the   *
 *				receiver only sends the content back. *
 *				The algorithm component should install*
 *			 	the receiving function to enable user *
 *				adapted functinality.				  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#ifndef USE_UART_FOR_MIDI

#ifndef __UARTRECEIVERGENERIC_H__
#define __UARTRECEIVERGENERIC_H__

#include "IuartApplicationGeneric.h"
#include "typedefs.h"

class CuartDispatcherGeneric;

/*==================================================
 * Class to implement the application side of the
 * RS232 communication. All base class (uartApplication)
 * callbacks have been implemented. The current
 * realization reacts on one message only be returning
 * a specific data field.
 *///==============================================
class CuartReceiverGeneric: public IuartApplicationGeneric
{
protected:
	
	// Backward reference to the dispatcher
    CuartDispatcherGeneric* _theDispatcher;
    
public:

    // Constructor
    CuartReceiverGeneric();

    // Set backward reference to RS232 dispatcher
	virtual void setBackwardReference(CuartDispatcherGeneric* theDispatcher);

    // Incoming default (previously "byte"-parsed) message
    virtual void incomingDefaultMessage(DATATYPE_RS232_CHAR purpose, 
    	DATATYPE_RS232* fldLoad, UWORD16 numElements);

    // incoming message in streaming mode: NOT SUPPORTED
    virtual void incomingStreamingMessage(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING]);

    // Callback to indicate that a message has been completely transmitted.
    virtual bool messageSendComplete(WORD32 messageID);

    // Report an error during byte parsing
    virtual void reportError(WORD32 errID);

    // Report a warning during byte parsing
    virtual void reportWarning(WORD32 warnID);
};

#endif
#endif
