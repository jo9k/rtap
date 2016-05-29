/******************************************************
 * Filename: debugCode.c							  *
 * Description: Functionality to enable MATLAB 		  *
 *				debugging from PC.		  			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/
#include "userDef.h"
#ifdef DEBUG_MATLAB

#include "userDef.h"
#include "configureUart.h"

#include <string.h>

#include "debugCode.h"
#include "memory.h"

// Default character field for functions
char invalidFctName[STRING_LENGTH] = "****invalid****";

#ifdef USE_FCT_TIMEOUT
#include <signal.h>
#include <processor_include.h>

extern debugCode theDebugCodeClass;

int cntResets = 0;

void timerZero(int aigInt)
{
	cntResets++;
	// Do not interfere with functioncall!
	if(theDebugCodeClass.state == debugCode::DST_FUNCTIONEXECUTE)
	{
		// Retart timeout to postpone the hard stop
		timer_set(TIMER_MAT_DEBUG, TIMER_MAT_DEBUG);
	    timer_on();
	}
	else
	{
		theDebugCodeClass.setBack();		
		timer_off();
	}
}
#endif

/*==================================================
 * Constructor: Initialize all static data
 *///===============================================
debugCode::debugCode()
{
	numberFunctions = 0;
	fctPointerTable = NULL;

	debugActive = 0;
	
	execute_numInArgs = 0;
	execute_numOutArgs = 0;
	execute_fctCallID = 0;
	
	ptrFieldCallIn = NULL;
	ptrFieldCallOut = NULL;
	numValuesIn = NULL;
	numValuesOut = NULL;
	
	inCounter = 0;
	outCounter = 0; 	
	
	state = DST_INITIALIZED;

#ifdef USE_FCT_TIMEOUT
	// Prepare interrupt for timeouts
	interrupt(SIG_TMZ, timerZero);
#endif
}

/*==================================================
 * Function to register a function to be exposed in MATLAB on the PC.
 * The function pointer will be stored in a table together with a name and the number
 * of input parameters. Note that the functionname can have only STRING_LENGTH-1
 * characters. 
 *///===============================================
void
debugCode::registerFunction(Fx fPtr, WORD32 numInParams, WORD32 numOutParams,
	char fName[STRING_LENGTH-1])
{
	WORD16 i;
	FCT_POINTER_FIELD* fctPointerTableTmp = NULL;
	if(numberFunctions != 0)
	{
		// Dynamic lookup tables very simple
		fctPointerTableTmp = NEW2 FCT_POINTER_FIELD[numberFunctions+1];
		for(i = 0; i < numberFunctions; i++)
		{
			fctPointerTableTmp[i].fPtr = fctPointerTable[i].fPtr;
			fctPointerTableTmp[i].numArgIn = fctPointerTable[i].numArgIn; 
			fctPointerTableTmp[i].numArgOut = fctPointerTable[i].numArgOut;
			memset(fctPointerTableTmp[i].nFct, 0, STRING_LENGTH);
			memcpy(fctPointerTableTmp[i].nFct, fctPointerTable[i].nFct, STRING_LENGTH-1);			
		} 
		
		fctPointerTableTmp[numberFunctions].fPtr = fPtr;
		fctPointerTableTmp[numberFunctions].numArgIn = numInParams;
		fctPointerTableTmp[numberFunctions].numArgOut = numOutParams;
		memset(fctPointerTableTmp[numberFunctions].nFct, 0, STRING_LENGTH);
		memcpy(fctPointerTableTmp[numberFunctions].nFct, fName, STRING_LENGTH-1);
		delete[](fctPointerTable);
		fctPointerTable = fctPointerTableTmp;
		numberFunctions++;
	}
	else
	{
		fctPointerTable = NEW2 FCT_POINTER_FIELD[1];
		fctPointerTable[0].fPtr = fPtr;
		fctPointerTable[0].numArgIn = numInParams;
		fctPointerTable[0].numArgOut = numOutParams;
		memset(fctPointerTable[0].nFct, 0, STRING_LENGTH);
		memcpy(fctPointerTable[0].nFct, fName, STRING_LENGTH-1);
		numberFunctions = 1;
	}
}

/*==================================================
 * Return the number of registered functions
 *///===============================================
WORD32
debugCode::returnNumberFcts()
{
	return(numberFunctions);
}

/*==================================================
 * Return the name of a function, addressed by the specified function ID.
 *///===============================================
WORD32
debugCode::copyNameFct(char fldOut[STRING_LENGTH], WORD32 fID)
{
	memset(fldOut, 0, STRING_LENGTH);
	if((fID >= 0)&&(fID < numberFunctions))
	{
		memcpy(fldOut, fctPointerTable[fID].nFct, STRING_LENGTH);
		return(0);
	}
	memcpy(fldOut, invalidFctName, STRING_LENGTH);
	return(-1);
}

/*==================================================
 * Return the number of in/out parameters.
 *///===============================================
WORD32
debugCode::getInOutArgs(WORD32* inArgs, WORD32* outArgs, WORD32 fID)
{
	if((fID >= 0)&&(fID < numberFunctions))
	{
		*inArgs = fctPointerTable[fID].numArgIn;
		*outArgs = fctPointerTable[fID].numArgOut;
		return(0);
	}
	return(-1);
}
	
/*==================================================
 * Prepare functioncall to be called in the following. 
 * If this function is called, the input parameters will arrive one
 * after another in the following. The fields allocated to store the 
 * input and the output data will be freed in the call to SetBack.
 * A complete functioncall must be terminated before another function call can be called.
 * If everything is OK, 0 is returned, otherwise -1.
 *///===============================================
WORD32
debugCode::prepareFctCall(WORD32 numInArgs, WORD32 numOutArgs, WORD32 fctID, WORD32* lFieldOut)
{
	WORD32 i;
	
	if(state != DST_FUNCTIONCALL)
	{
		// Set the number of parameters
		execute_numInArgs = numInArgs;
		execute_numOutArgs = numOutArgs;
		execute_fctCallID = fctID;
	
		// Allocate the fields to call one function
		ptrFieldCallIn = NEW2 float*[execute_numInArgs];
		ptrFieldCallOut  = NEW2 float*[execute_numOutArgs];
		numValuesIn = NEW2 WORD32[execute_numInArgs];
		numValuesOut = NEW2 WORD32[execute_numOutArgs];

		// For the output fields, we cann allocate fields already	
		for(i = 0; i < numOutArgs; i++)
		{
			ptrFieldCallOut[i] = NEW2 float[lFieldOut[i]];
			numValuesOut[i] = lFieldOut[i];
		}
	
		// Counters to collect fields
		inCounter = 0;
		outCounter = 0;
	
		state = DST_FUNCTIONCALL;
		
#ifdef USE_FCT_TIMEOUT
		// Start timeout
		timer_set(TIMER_MAT_DEBUG, TIMER_MAT_DEBUG);
	    timer_on();
#endif
		return(0);
	}
	return(-1);
}

/*==================================================
 * Add another field for functionall input.
 * Returnvalue of -1 indicates that lesser input arguments were expected
 * Returnvalue of -2 indicates that the module is in the wrong state.
 *///===============================================
WORD32
debugCode::addFieldInput(float* fld, WORD32 szFld)
{	
	float oneElm;
	WORD32 i;
	
	// Check that we are in the right state
	if(state != DST_FUNCTIONCALL)
	{
		return(-2);
	}

		// Check that the number of input fields matches the specification from preparation
	if(inCounter >= execute_numInArgs)
	{
		return(-1);
	}
	
	// Now allocate another field	
	ptrFieldCallIn[inCounter] = NEW2 float[szFld];
		
	memcpy(ptrFieldCallIn[inCounter], fld, szFld);
	numValuesIn[inCounter] = szFld;
	inCounter++;
	return(0);
}

/*==================================================
 * Copy another output field when functioncall is complete.
 * Returnvalue of -1 indicates that lesser output arguments were expected
 * Returnvalue of -2 indicates that the module is in the wrong state.
 *///===============================================
WORD32
debugCode::getNextFieldOutput(float** fld, WORD32* szFld)
{
	WORD32 i= 0;

	// Check that we are in the right state
	if(state != DST_FUNCTIONCALL)
	{
		return(-2);
	}

		// Check that the number of output fields matches the specification from preparation
	if(outCounter >= execute_numOutArgs)
	{
		return(-1);
	}
		
	*fld = ptrFieldCallOut[outCounter];
	*szFld = numValuesOut[outCounter];
	outCounter++;
	if(outCounter == execute_numOutArgs)
		return(1);
	return(0);
}

/*==================================================
 * Call the remote debugging functions and pass input and output field references as arguments.
 * If the function ID is out of range of valid IDs, -1 is returned.
 * if the module is not in the right state, -2 is returned.
 *///===============================================
WORD32 
debugCode::executeFunction()
{
	WORD32 retVal = 0;
	if(state == DST_FUNCTIONCALL)
	{
		state = DST_FUNCTIONEXECUTE;
		if((execute_fctCallID >= 0)&&(execute_fctCallID < numberFunctions))
		{
			retVal = fctPointerTable[execute_fctCallID].fPtr(ptrFieldCallIn, execute_numInArgs, 
				numValuesIn, ptrFieldCallOut, execute_numOutArgs, 
				numValuesOut);
		}
		else
		{
			retVal = -1;
		}
		state = DST_FUNCTIONCALL;
		return(retVal);
	}
	return(-2);
}
	
/*==================================================
 * Functioncall to set the debugger to initiali state and to deallocate all allocated memory for 
 * a previous functioncall.
 *///===============================================
WORD32
debugCode::setBack()
{
	WORD32 i;
	if( state == DST_FUNCTIONCALL)
	{
		for (i = 0; i <execute_numOutArgs; i++)
		{
			delete[](ptrFieldCallOut[i]); 
		}

//	for (i = 0; i <execute_numInArgs; i++)
		for (i = 0; i <inCounter; i++)
		{
			delete[](ptrFieldCallIn[i]); 
		}
	
		inCounter = 0;
		outCounter = 0;
	
		delete[](ptrFieldCallIn);
		delete[](ptrFieldCallOut);
		delete[](numValuesIn);
		delete[](numValuesOut);
	
		execute_numInArgs = 0;
		execute_numOutArgs = 0;
		execute_fctCallID = 0;

		state = DST_INITIALIZED;
#ifdef USE_FCT_TIMEOUT
		timer_off();
#endif

		return(0);
	}
	return(-1);
}
#endif


