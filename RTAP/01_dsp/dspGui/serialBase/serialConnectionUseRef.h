/***********************************************************************
 * DESCRIPTION:
 * PORT events to application: Base class for handling all RS-232 events to the application
 ***********************************************************************
 * This is the definition of the abstract base class for backward
 * messaging from port to application. Each application using the 
 * port classes must be derived from this base class and thus implement the 
 * specified callbacks
 *
 * Copyright Hauke Krueger, IND, 2004, 02-06-04
 *********************************************************************/

#ifndef _SERIALCONNECTIONUSEREF_H__
#define _SERIALCONNECTIONUSEREF_H__

#include <windows.h>
#include "timerWin32.h"

/**
 * Abstract interface class to implement the back communication from
 * RS-232 port to application: Provide callbacks for events being triggered from 
 * port.
 *///===========================================================================
class serialConnectionUseRef  
{
public:

	//! Error indication in case that error callback is used
	typedef enum
	{
		ERROR_ID_RECEIVE_BUFFER_FULL,	// Receive buffer is full
		ERROR_ID_SEND_BUFFER_FULL,		// send buffer is full
		ERROR_SEND_FAILED				// Send failed
	} errorEnum;

	//! Callback for indication: Message has been completely sent
	virtual void messageComplete(unsigned portID, const char* field, unsigned amBytes, long sendID) = 0;

	//! Callback for indication: Message has not been sent due to a timeout that has passed by
	virtual void messageTimeout(unsigned portID, const char* field, unsigned amBytes, long sendID) = 0;

	//! Callback for indication: A message has been received
	virtual void messageReceived(unsigned portID, long receiveID, const char* newField, unsigned amBytes) = 0;

	//! A message has been discharded from message queue (e.g. kill messages)
	virtual	void messageDiscarded(unsigned portID, const char* field, unsigned amBytes, long sendID) = 0;

	//! An error has occurred
	virtual void error(unsigned portID, errorEnum reason) = 0;

	//! Time has been woken up, the timer itself is passed as pointer
	virtual void awakeTimer(timerWin32* timerPointer) = 0;

	//! A ring line event is indicated
	virtual void indicateRing(unsigned portID) = 0;

	//! An RLSD line event is indicated
	virtual void indicateRLSDChanged(unsigned portID) = 0;

	//! An CTS line event is indicated
	virtual void indicateCTSChanged(unsigned portID) = 0;

	//! A line event is indicated (pass complete WIN32 mask (see WIN32 Platform SDK)
	virtual void indicateEvent(unsigned portID, DWORD evMask) = 0;

	//! Constructor: Do nothing
	serialConnectionUseRef(){};

	//! Destructor: Do nothing
	virtual ~serialConnectionUseRef(){};

};

#endif // !defined(AFX_SERIALCONNECTIONUSEREF_H__54EB6B16_D9D5_497F_9F81_BA2ED9E2FFB1__INCLUDED_)
