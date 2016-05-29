/* 
 *****************************************************
 * Filename: TrtpRuntimeMessages.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: Definitions of data types and enumeration 
 *				values as used by the runtime messaging 
 *				principle. A data type is self containing, 
 *				therefore the message package contents 
 *				must be interpreted in the context of 
 *				specified types.
 *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/


#ifndef __RUNTIMETYPES_H__
#define __RUNTIMETYPES_H__

/**
 * Typedef for message of the JVX_ID_REQUEST type.
 *///================================================================
typedef enum
{
	JVX_ID_STOPPROCESSING_NONBLOCKING = 0,
	JVX_ID_STAJVXROCESSING_NONBLOCKING = 1,
	JVX_ID_SEND_TEXT_MESSAGE_FORCE = 2,
	JVX_ID_SEND_TEXT_MESSAGE_NONBLOCKING = 3,
	JVX_ID_SEND_REQUEST_POLL_GENCONF_OUTPUT_NONBLOCKING_UPDATE = 4,
	JVX_ID_SEND_REQUEST_POLL_GENCONF_INPUT_NONBLOCKING_UPDATE = 5,
	JVX_ID_SEND_REQUEST_POLL_GENCONF_OUTPUT_NONBLOCKING_RECREATE = 6,
	JVX_ID_SEND_REQUEST_POLL_GENCONF_INPUT_NONBLOCKING_RECREATE = 7,
	JVX_ID_SEND_REQUEST_UPDATE_UI = 8,
	//JVX_ID_SEND_INFORMATION_GENERIC = 6,
	JVX_ID_REQ_INVALID = 0xFFFFFFFF
} jvxRequestType;

/**
 * Typedef for message of the JVX_ID_REPORT type.
 *///================================================================
typedef enum
{
	JVX_ID_BROKENPIPE = 0,
	JVX_ID_REP_INVALID = 0xFFFFFFFF
} jvxReportType;

/**
 * Typedef for message types.
 *///================================================================
typedef enum
{
	JVX_ID_REQUEST = 0,
	JVX_ID_REPORT = 1,
	JVX_ID_FULLSPEEDDEBUG = 2,
	JVX_ID_FREEFORMAT = 0xFFFFFFFE,
	JVX_ID_MSG_INVALID = 0xFFFFFFFF
} jvxMessageType;

/**
 * Typedef for the data struct that represents the binary data package format 
 * used by th real-time runtime messaging principle.
 *///=========================================================================
typedef struct
{
	// START OF HEADER PART
	struct
	{
		//! Purpose ID, according to JVX_MESSAGE
		int idMessageType;

		//! Purpose ID, according to JVX_REQUEST or another of the subfields
		int idMessageSubType;

		//! Handle for backward reference, still open, will probably be a pointer
		long long backwardReference;

		//! ID for the sender, used if a dispatcher in in use (not yet implemented)
		int idSender;

		//! ID for the receiver, used if a dispatcher in in use (not yet implemented)
		int idReceiver;

		//! Indication for the priority of the message: time critical or regular
		int priorityMessage;

		//! Message counter to respond in case of non blocking function
		long long countMessage;

		//! ID specified by user
		long long userID;
	
		//! Reserved field
		long long reserved;

		//! Length of field: overall size of message
		signed int lengthMessage;

		//! Length of load part of the field
		signed int lengthMessageLoad;

		//! Error code if response is given
		int errorCodeOnResponse;
	} header;

	// START OF LOAD PART
	//! Entry for following field, convert as struct* start = (struct*)&fieldChar
	char fieldStart;
} TjvxRuntimeMessage;


#endif
