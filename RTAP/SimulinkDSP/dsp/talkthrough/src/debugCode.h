#include "userDef.h"
#ifdef DEBUG_MATLAB

#ifndef __DEBUGCODE_H__
#define __DEBUGCODE_H__

typedef WORD32 (*Fx)(float** fldIn, WORD32 numArgIn, WORD32* lFieldsIn,
 	float** fldOut, WORD32 numArgOut, WORD32* lFieldsOut);

class debugCode
{
public:

	enum debuggerState 
	{
		DST_INITIALIZED = 0,
		DST_FUNCTIONCALL = 1,
		DST_FUNCTIONEXECUTE = 2
	};
	
	typedef struct
	{
		Fx fPtr;
		WORD32 numArgIn;
		WORD32 numArgOut;
		char nFct[16];
	}FCT_POINTER_FIELD;
	
	WORD32 debugActive;
	
	WORD32 execute_numInArgs;
	WORD32 execute_numOutArgs;
	WORD32 execute_fctCallID;
	
	float** ptrFieldCallIn;
	float** ptrFieldCallOut;
	WORD32* numValuesIn;
	WORD32* numValuesOut;
	
	WORD32 inCounter;
	WORD32 outCounter;
	
	debuggerState state;
	
private:

	FCT_POINTER_FIELD* fctPointerTable;
	WORD32 numberFunctions;

public:
	debugCode();
	void registerFunction(Fx fPtr, WORD32 numInParams, WORD32 numOutParams,
		char fName[STRING_LENGTH-1]);


	WORD32 returnNumberFcts();
	WORD32 copyNameFct(char fldOut[STRING_LENGTH], WORD32 idFct);
	WORD32 getInOutArgs(WORD32* inArgs, WORD32* outArgs, WORD32 idFct);
	WORD32 prepareFctCall(WORD32 numInArgs, WORD32 numOutArgs, WORD32 fctID, WORD32* ptrLOutArgs);

	WORD32 addFieldInput(float* fld, WORD32 szFld);
	WORD32 getNextFieldOutput(float** fld, WORD32* szFld);
	WORD32 executeFunction();
	WORD32 setBack();
};

#endif
#endif
