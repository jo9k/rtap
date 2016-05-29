/******************************************************
 * Filename: algorithmIND_DSP.h						  *
 * Description: Base class to realize algorithm 	  *
 *              components. The most easy access to   *
 * 				new algorithms is to derive from this *
 * 				class.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef _UARTDISPATCHER_H__
#define _UARTDISPATCHER_H__

#include "configureUart.h"

class uartIND;
class uartApplication;

/**
 * UART dispatcher class to transform RS232 messages into an application
 * specific format. The class provides callbacks that the
 * uartIND block calls whenever required.
 *///==================================================
class IuartDispatcher
{
public:

    // A new byte has been received in default mode (parsing!)
    virtual void byteReceivedDefault(DATATYPE_RS232_CHAR byteMess) = 0;

    // A complete streaming message has been parsed
    virtual void messageReceivedStreaming(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING]) = 0;

    //! Callback to report that an outgoing message has been completed
    virtual void messageSendComplete() = 0;

};

#endif
