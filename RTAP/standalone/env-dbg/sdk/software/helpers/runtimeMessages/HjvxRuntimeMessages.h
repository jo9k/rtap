/* 
 *****************************************************
 * Filename: HjvxRuntimeMessages.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: This file contains static functions 
 *			that provide often used functionality in 
 *			the context of the allocation/deallocation 
 *			of runtime messages.                                     *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#include <string>

#ifndef _HJVXRUNTIMEMESSAGES_H__
#define _HJVXRUNTIMEMESSAGES_H__

/** 
 * Function to prepare a text mesage runtime message. Besides the string that contains the
 * text message for transmission, also a message counter (userID) is required and a sender ID.
 *///=====================================================================================
static TjvxRuntimeMessage*
HjvxRuntimeMessage_allocateTextMessage(const std::string& txt, int messCounter, int sendID)
{
	// Allocate the TjvxRuntimeMessage field
	TjvxRuntimeMessage* mess = (TjvxRuntimeMessage*) new byte[sizeof(mess->header)+txt.size()+1];
	memset(mess, 0, sizeof(mess->header)+txt.size()+1);
	
	// Set the required data in the header
	mess->header.idMessageType = JVX_ID_REQUEST;
	mess->header.idMessageSubType = JVX_ID_SEND_TEXT_MESSAGE_FORCE;
	mess->header.backwardReference = (long long)NULL;
	mess->header.idSender = sendID;
	mess->header.idReceiver = JVX_COMPONENT_UNKNOWN;
	mess->header.priorityMessage = -1;
	mess->header.userID = messCounter;
	mess->header.lengthMessage = (int)(sizeof(mess->header)+txt.size()+1);
	mess->header.lengthMessageLoad = (int)(txt.size()+1);

	memcpy(&mess->fieldStart, txt.c_str(), txt.size());
	return(mess);
}
			
/** 
 * Function to prepare a text mesage runtime message. Besides the string that contains the
 * text message for transmission, also a message counter (userID) is required and a sender ID.
 *///=====================================================================================
static TjvxRuntimeMessage*
HjvxRuntimeMessage_allocateTextMessageNonBlocking(const std::string& txt, int messCounter, int sendID)
{
	// Allocate the TjvxRuntimeMessage field
	TjvxRuntimeMessage* mess = (TjvxRuntimeMessage*) new byte[sizeof(mess->header)+txt.size()+1];
	memset(mess, 0, sizeof(mess->header)+txt.size()+1);
	
	// Set the required data in the header
	mess->header.idMessageType = JVX_ID_REQUEST;
	mess->header.idMessageSubType = JVX_ID_SEND_TEXT_MESSAGE_NONBLOCKING;
	mess->header.backwardReference = (long long)NULL;
	mess->header.idSender = sendID;
	mess->header.idReceiver = JVX_COMPONENT_UNKNOWN;
	mess->header.priorityMessage = -1;
	mess->header.userID = messCounter;
	mess->header.lengthMessage = (int)(sizeof(mess->header)+txt.size()+1);
	mess->header.lengthMessageLoad = (int)(txt.size()+1);
	
	memcpy(&mess->fieldStart, txt.c_str(), txt.size());
	return(mess);
}

/** 
 * Function to prepare a runtime message to request an update of the UI.
 *///=====================================================================================
static TjvxRuntimeMessage*
HjvxRuntimeMessage_allocateMessageUpdateUi(int messCounter, int sendID)
{
	// Request UI update
	TjvxRuntimeMessage* mess = (TjvxRuntimeMessage*) new byte[sizeof(mess->header)];
	memset(mess, 0, sizeof(mess->header));

	// Set the required data in the header
	mess->header.idMessageType = JVX_ID_REQUEST;
	mess->header.idMessageSubType = JVX_ID_SEND_REQUEST_UPDATE_UI;
	mess->header.backwardReference = (long long)NULL;
	mess->header.idSender = sendID;
	mess->header.idReceiver = JVX_COMPONENT_UNKNOWN;
	mess->header.priorityMessage = -1;
	mess->header.userID = messCounter;
	mess->header.lengthMessage = sizeof(TjvxRuntimeMessage);
	mess->header.lengthMessageLoad = 0;

	return(mess);
}

/**
 * Function to release the memory used for the transmission of runtime message object.
 *///===============================================================================
static void
HjvxRuntimeMessage_deallocateTextMessage(TjvxRuntimeMessage* mess)
{
	delete[](mess);
}

#endif