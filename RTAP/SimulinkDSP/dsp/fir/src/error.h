/******************************************************
 * Filename: errorCodes.h							  *
 * Description: Definition of error codes.			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum
{
	ERROR_NO_ERROR = 0, 		// No error
	ERROR_RS232_BUSY_OUT = 1, 	// Error when trying to send a RS232 message
	ERROR_RS232_BUSY_OUT_LOWLEVEL = 2, 	// Low level Error when trying to send a RS232 message
	ERROR_RS232_BUSY_IN = 3, 	// Error when trying to receive a RS232 message
	ERROR_RS232_UNKNOWN_MSG_PURPOSE = 4, // A received RS232 message has an unknown purpose 
	ERROR_RS232_UNEXPECTED_INPUT = 5,
	ERROR_RS232_BUSY_OUT_REPORT_ERROR = 6, 	// Error when trying to send a RS232 message to report an error
	ERROR_FATAL_ERROR = 0xFFFFFF
} errorCodes;

extern void reportGlobalError(int errorCode);

#endif
