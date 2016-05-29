#include ".\serialhost.h"
#include "serialConnection.h"
#include <cmath>

static int msgCntID0 = 0;
static int msgCntID1 = 0;

serialHost::serialHost(void)
{
	idPort = 0;
	connection = NULL;
}

void
serialHost::setReference(serialConnection* conn, unsigned portID)
{
	idPort = portID;
	connection = conn;
}

serialHost::~serialHost(void)
{
}

void 
serialHost::messageComplete(unsigned portID, const char* newField, unsigned amBytes, long sendID)
{
/*	std::cout << "MESSAGE COMPLETE!" << std::endl;
	std::cout << "Port Origin:" << portID << std::endl;
	std::cout << "Number bytes:" << amBytes << std::endl;
	int val = 0;
	int cntBytes = amBytes;
	while(cntBytes > 0)
	{
		if(cntBytes >= 4)
		{
			memcpy(&val, newField, 4);
			newField += 4;
			cntBytes -= 4;
			std::cout << "INT: " << val << std::endl;
		}
		else
		{
			std::cout << "CHAR: " << (int)*newField << std::endl;
			cntBytes--;
			newField++;
		}
	}
	*/
}
	
void 
serialHost::messageTimeout(unsigned portID, const char* field, unsigned amBytes, long sendID)
{
	std::cout << "MESSAGE TIMEOUT!" << std::endl;
}
	
void 
serialHost::messageReceived(unsigned portID, long receiveID, const char* newField, unsigned amBytes)
{
/*	std::cout << "MESSAGE RECEIVED" << std::endl;
	std::cout << "Port Origin:" << portID << std::endl;
	std::cout << "Number bytes:" << amBytes << std::endl;*/
	const char * sameField = newField;
	int val = 0;
	int cntBytes = amBytes;
	int cntWords = 0;
	while(cntBytes > 0)
	{
		if(cntBytes >= 4)
		{
			memcpy(&val, newField, 4);
			newField += 4;
			cntBytes -= 4;
			if(cntWords == 4)
			{
				if(val == 4660)
				{
					msgCntID1++;
				}
				else
				{
					msgCntID0++;
				}
				printf("DIFF ID:%d\r", abs((msgCntID0- msgCntID1)));
			}
			cntWords++;
//			std::cout << "INT: " << val << std::endl;
		}
		else
		{
			std::cout << "CHAR: " << (int)*newField << std::endl;
			cntBytes--;
			newField++;
		}
	}
	if(connection)
		connection->initSend(idPort,sameField, amBytes, 0);
}
	
void 
serialHost::messageDiscarded(unsigned portID, const char* field, unsigned amBytes, long sendID)
{
	std::cout << "MESSAGE DISCARDED!" << std::endl;
}
	
void 
serialHost::error(unsigned portID, errorEnum reason)
{
	std::cout << "ERROR" << std::endl;
}
	
void 
serialHost::awakeTimer(timerWin32* timerPointer)
{
	std::cout << "AWAKE TIMER!" << std::endl;
}
	
void 
serialHost::indicateRing(unsigned portID)
{
	std::cout << "RING CHANGED!" << std::endl;
}
	
void 
serialHost::indicateRLSDChanged(unsigned portID)
{
	std::cout << "RLSD CHANGED!" << std::endl;
}
	
void 
serialHost::indicateCTSChanged(unsigned portID)
{
	std::cout << "CTS CHANGED!" << std::endl;
}
	
void 
serialHost::indicateEvent(unsigned portID, DWORD evMask)
{
//	std::cout << "EVENT!" << std::endl;
}
