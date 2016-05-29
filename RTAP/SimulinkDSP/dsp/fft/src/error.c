/******************************************************
 * Filename: errorCodes.c							  *
 * Description: Callback functions where all errors   *
 *              are reported.			 			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include "error.h"
 
// Static global variable to read out the memory
static int lastError;

/*==================================================
 * Function as a callback for all possible errors
 *///===============================================
void reportGlobalError(int errorCode)
{
	lastError = errorCode;
 	
 	// Do whatever you like!
}
