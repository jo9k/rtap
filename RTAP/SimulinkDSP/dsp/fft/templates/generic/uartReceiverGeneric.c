/******************************************************
 * Filename: uartReceiver.c							  *
 * Description: Component to receive RS232 generic    *
 * 				messages. This default implementation *
 *				does not do anything relevant except  *
 * 				for sending the incoming message back.*
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "userDef.h"
#include "configureUart.h"

#ifndef USE_UART_FOR_MIDI

#include <string.h>
#include "uartReceiverGeneric.h"
#include "uartDispatcherGeneric.h"

/*==================================================
 * Constructor: Initialize the dispatcher reference to zero.
 *///===============================================
CuartReceiverGeneric::CuartReceiverGeneric()
{
	_theDispatcher = NULL;
}

/*==================================================
 * Function to set backward reference from main program.
 *///===============================================
void 
CuartReceiverGeneric::setBackwardReference(CuartDispatcherGeneric* theDisp)
{
	_theDispatcher = theDisp;
}
	
/*==================================================
 * Callback to receive RS232 messages. Default functionality is
 * to simply send the message back.
 *///===============================================
void
CuartReceiverGeneric::incomingDefaultMessage(DATATYPE_RS232_CHAR purpose, 
	DATATYPE_RS232* fldLoad, UWORD16 numElements)
{
	switch(purpose)
	{
		case 0:
			
			// send message right back
			if(!_theDispatcher->sendMessageNonBlocking(purpose, fldLoad, numElements, 0));
			{
				// report error
			}
			break;
		default:
			// send message unsopported back
			break;
	}
}

/*==================================================
 * Callback to receive RS232 streaming messages. This
 * functionality is not yet supported.
 *///===============================================
void
CuartReceiverGeneric::incomingStreamingMessage(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING])
{
}

/*==================================================
 * Callback to indicate that a message has been completely sent.
 * The message ID is the same which was previously passed to the 
 * dispatcher when the mesage was triggered to be sent.
 *///===============================================
bool
CuartReceiverGeneric::messageSendComplete(WORD32 messageID)
{
    return(true);
}

/*==================================================
 * Callback to indicate that an error has occurred with respect to the 
 * RS232 link. In general a repair is not required.
 *///===============================================
void
CuartReceiverGeneric::reportError(WORD32 errID)
{
	// Do nothing here, just set a breakpoint if desired
}

/*==================================================
 * Callback to indicate that a warning has been caused with respect to the 
 * RS232 link. In general a repair is not required.
 *///===============================================void
void
CuartReceiverGeneric::reportWarning(WORD32 warnID)
{
	// Do nothing here, just set a breakpoint if desired
}


#endif
