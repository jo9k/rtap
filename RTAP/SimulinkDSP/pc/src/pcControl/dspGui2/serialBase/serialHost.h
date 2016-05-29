#pragma once
#include "serialconnectionuseref.h"

class serialConnection;

class serialHost :
	public serialConnectionUseRef
{
private:
	serialConnection* connection;
	unsigned idPort;
public:
	void messageComplete(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void messageTimeout(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void messageReceived(unsigned portID, long receiveID, const char* newField, unsigned amBytes);
	void messageDiscarded(unsigned portID, const char* field, unsigned amBytes, long sendID);
	void error(unsigned portID, errorEnum reason);
	void awakeTimer(timerWin32* timerPointer);
	void indicateRing(unsigned portID);
	void indicateRLSDChanged(unsigned portID);
	void indicateCTSChanged(unsigned portID);
	void indicateEvent(unsigned portID, DWORD evMask);

	serialHost(void);
	void setReference(serialConnection* conn, unsigned portID);
	~serialHost(void);
};
