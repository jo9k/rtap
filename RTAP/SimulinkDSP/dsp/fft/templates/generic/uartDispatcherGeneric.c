/******************************************************
 * Filename: uartDispatcherGeneric.h                  *
 * Description: File to implement the functionality of*
 * 				the generic RS232 message dispatcher  *
 * copyright IND, RWTH Aachen, 2008, HK
 ******************************************************/

#include "userDef.h"
#include "memory.h"

#ifndef USE_UART_FOR_MIDI

#define USE_FLAGS_RS232

#ifdef USE_FLAGS_RS232
#define SRUDEBUG  // Check SRU Routings for errors.
#include <SRU.h>
#endif

#include <string.h>
#include <signal.h>
#include <Cdef21369.h>
#include <def21369.h>

#include "uartIND.h"
#include "uartDispatcherGeneric.h"
#include "IuartApplicationGeneric.h"
#include "memory.h"
#include "error.h"

#ifdef DEBUG_MATLAB
#include "debugCode.h"
extern debugCode theDebugCodeClass;
#endif

DATATYPE_RS232 RAM_DEFAULT_SECTION tokenHello[] = 
{ 
	0x2C444E49, 
	0x50534441,
	0x36333132,
	0x54522c39,
	0x434f5250,
	0x5053445f
};

DATATYPE_RS232 RAM_DEFAULT_SECTION procParaFld[] =
{
	SAMPLERATE,
	2,
#ifdef CHANNELSET2
#ifdef CHANNELSET3
	6,
#else
	4,
#endif
#else
	2,
#endif
	BUFFERSIZE,
#ifdef RTP_CHANNELFORMAT_FLOAT
	2
#else
#ifdef RTP_CHANNELFORMAT_LITTLE_ENDIAN_24BIT
	4
#else
	3
#endif
#endif
};

	
// Declaration of interrupt service routine for low level
// callback
void processMessageUser(int sigInt);

// Global pointer to return to class in interrupt service 
// (which is a global function)
CuartDispatcherGeneric RAM_DEFAULT_SECTION* theDispatcher;

/*==================================================
 * Constructor: Initialize all static data to revceive and 
 * transmit RS232 messages.
 *///===============================================
 CuartDispatcherGeneric::CuartDispatcherGeneric()
{

    inputStateMachine = MESSAGE_TO_START;
    newWord = 0;

    // Handle to uart component
    refHandleLink = 0;
    
    // Handle to application reference
    refHandleApplication = 0;

    // All for input 
 	inputMessage.ptrLoadField = NULL;
    inputMessage.cntMessageLoad = 0;
    inputMessage.msgPurpose = PMSG_PURPOSE_INVALID;
    inputMessage.msgOpcode = 0xFF;
    inputMessage.byteCounterWord = 0; 
    inputMessage.lLoadField = 0;
// 	inputMessage.idMessageInput = 0;
//	inputMessage.idMessageInputProcessed = -1;
	inputMessage.inputMessagePtr = NULL;    
	processInputMessage.loadFieldToProcess = NULL;
	processInputMessage.lLoadField = 0;
	processInputMessage.inProcessing = 0;
	processInputMessage.msgPurpose = PMSG_PURPOSE_INVALID;
    processInputMessage.msgOpcode = 0xFF;

    // Variables for output
    outputMessageVarLength = NULL;
    ptrContinue = NULL;
   	elementsToGo = 0;
    messageID = -1;
    sendInProgress = 0;

#ifdef DEBUG_MATLAB
	stateDebugger = DBG_ST_DEVICE_UNINITIALZED;
#endif

#ifdef USE_FLAGS_RS232
	asm("#include <def21369.h>");
	
	// Create link to control flags
    SRU(FLAG4_O,DPI_PB06_I);    //Drive DPI Pin 6 with Flag 4 (LED 1)
    SRU(FLAG5_O,DPI_PB07_I);    //Drive DPI Pin 7 with Flag 5 (LED 2)

	//Enabling the Buffer direction for DAI and DPI pins using the following sequence:
	//High -> Output, Low -> Input
    SRU(HIGH,DPI_PBEN06_I);
    SRU(HIGH,DPI_PBEN07_I);
 
    // Switch all (5) LEDs off.
    asm("bit set flags FLG4O|FLG5O;");
	asm("bit clr flags FLG4|FLG5;");
 
#endif

}

/*==================================================
 * Function to initialize the dispatcher. Especially 
 * in this function all pointer shortcuts are set.
 *///===============================================
void
CuartDispatcherGeneric::initialize(uartIND* handleLink,
                                   IuartApplicationGeneric* handleApplication)
{
	// Set the link to the uart low level component.
    refHandleLink = handleLink;
    
    // set the link to the receiver
    refHandleApplication = handleApplication;

        // Set the link in global variable for ISR
    theDispatcher = this;
    
#ifdef DEBUG_MATLAB
	stateDebugger = DBG_ST_NORMAL_MODE;
#endif

    // Start interrupt handling
    interrupt(SIG_USR2, processMessageUser);

}

/*==================================================
 * Function to parse an incoming byte. All messages 
 * are parsed by a simple algorithm including a state machine. 
 * If the message is complete, a low level interrupt will be
 * triggered.
 *///===============================================
void
CuartDispatcherGeneric::byteReceivedDefault(DATATYPE_RS232_CHAR byteMessage)
{
    // State machine: check that the format is right
    switch (inputStateMachine)
    {
    case MESSAGE_TO_START:
        if (byteMessage == SYNCWORD_DEFAULT1)
        {
            inputStateMachine = MESSAGE_SYNC1_COMPLETE;
        }
        break;
    case MESSAGE_SYNC1_COMPLETE:
        if (byteMessage == SYNCWORD_DEFAULT2)
        {
            inputStateMachine = MESSAGE_SYNC2_COMPLETE;
        }
        else
        {
            inputStateMachine = MESSAGE_TO_START;
        }
        break;
    case MESSAGE_SYNC2_COMPLETE:

        if (byteMessage == SYNCWORD_DEFAULT3)
        {
            inputStateMachine = MESSAGE_SYNC3_COMPLETE;
        }
        else
        {
            inputStateMachine = MESSAGE_TO_START;
        }
        break;
    case MESSAGE_SYNC3_COMPLETE:
        if (byteMessage == SYNCWORD_DEFAULT4)
        {
            inputStateMachine = MESSAGE_SYNC4_COMPLETE;
	
            // Start to process incoming message
//			WORD32 newIdx = (inputMessage.idMessageInput+1)%2;
//			if(newIdx == inputMessage.idMessageInputProcessed)
//			{
				// REPORT ERROR
//				inputStateMachine = MESSAGE_TO_START;
//			}
//			else
//			{
				inputMessage.ptrLoadField = NULL;
    			inputMessage.msgPurpose = PMSG_PURPOSE_INVALID;
		    	inputMessage.msgOpcode = 0xFF;
		    	inputMessage.byteCounterWord = 0; 
		    	inputMessage.lLoadField = 0;
#ifdef USE_FLAGS_RS232
	    		asm("bit set flags FLG4;");
#endif
//			}
        }
        else
        {
            inputStateMachine = MESSAGE_TO_START;
        }
        break;
    case MESSAGE_SYNC4_COMPLETE:

    	inputMessage.msgPurpose = byteMessage;
        inputStateMachine = MESSAGE_PURPOSE_DONE;
        break;

    case MESSAGE_PURPOSE_DONE:

		inputMessage.msgOpcode = byteMessage;
		inputStateMachine = MESSAGE_OPCODE_DONE;
		inputMessage.byteCounterWord = 2;
        newWord = 0;
        break;
    case MESSAGE_OPCODE_DONE:

        newWord = (newWord>>8)|(((int)byteMessage) << 8);
        inputMessage.byteCounterWord--;
        if (inputMessage.byteCounterWord == 0)
        {
            inputMessage.lLoadField = newWord;
			if(newWord == 0)
			{
				if(processInputMessage.inProcessing)
				{
					reportGlobalError(ERROR_RS232_BUSY_IN);
					// report error here!
					// message is ignored
				}
				else
				{
					processInputMessage.loadFieldToProcess = NULL;
					processInputMessage.msgPurpose = inputMessage.msgPurpose;
					processInputMessage.msgOpcode = inputMessage.msgOpcode;
					processInputMessage.lLoadField = 0;
				    processInputMessage.inProcessing = 1;
				
#ifdef USE_FLAGS_RS232
					// Switch the LED off
		    		asm("bit clr flags FLG4;");
#endif
				    // Set incoming interrupt
					asm("#include <def21369.h>");
					asm("bit set IRPTL SFT2I;");
				}
				inputStateMachine = MESSAGE_TO_START;
			}
			else
			{
	            inputMessage.cntMessageLoad = 0;
				MEMORY_THREADSAFE_START	            
				inputMessage.ptrLoadField = (DATATYPE_RS232*)MALLOC2(inputMessage.lLoadField);
				MEMORY_THREADSAFE_STOP
				inputMessage.inputMessagePtr = inputMessage.ptrLoadField;
	            inputMessage.byteCounterWord = DATATYPE_LENGTH_RS232;
	            inputStateMachine = MESSAGE_SCAN_LOAD;
	            break;
			}
        }
        else
        {
        	break;
        }
    case MESSAGE_SCAN_LOAD:
        newWord = (newWord>>8)|(((int)byteMessage) << 8*(DATATYPE_LENGTH_RS232-1));
        inputMessage.byteCounterWord --;
        if (inputMessage.byteCounterWord == 0)
        {
			*inputMessage.inputMessagePtr++ = newWord;
			inputMessage.cntMessageLoad++;
			inputMessage.byteCounterWord = (WORD32)DATATYPE_LENGTH_RS232;
			if(inputMessage.cntMessageLoad == inputMessage.lLoadField)
			{
				if(processInputMessage.inProcessing)
				{
					reportGlobalError(ERROR_RS232_BUSY_IN);
					// report error here!
					// message is ignored
					free(inputMessage.ptrLoadField);
				}
				else
				{
					processInputMessage.loadFieldToProcess = inputMessage.ptrLoadField;
					processInputMessage.msgPurpose = inputMessage.msgPurpose;
					processInputMessage.msgOpcode = inputMessage.msgOpcode;
					processInputMessage.lLoadField = inputMessage.lLoadField;
					processInputMessage.inProcessing = 1;

			
#ifdef USE_FLAGS_RS232
		    		// Switch the LED off
		    		asm("bit clr flags FLG4;");
#endif
					// Set incoming interrupt
					asm("#include <def21369.h>");
					asm("bit set IRPTL SFT2I;");
				}
				inputStateMachine = MESSAGE_TO_START;
				// TODO processSingleMessage();
			}
			else
			{
				break;
			}
        }
        else
        {
        	break;
        }
    }
}

/*==================================================
 * Function to receive a streaming message. Streaming messages are
 actuially not yoet supported.
 *///===============================================
void
CuartDispatcherGeneric::messageReceivedStreaming(DATATYPE_RS232_CHAR ptrMess[MAX_NUM_BYTES_STREAMING])
{
    if (refHandleApplication)
        refHandleApplication->incomingStreamingMessage(ptrMess);

}

/*==================================================
 * Callback function to indicate that a message has been 
 * completely transmitted. Note that nothing is known at 
 * this moment about the receiver.
 * actuially not yoet supported.
 *///===============================================
void
CuartDispatcherGeneric::messageSendComplete()
{
	bool result = false;
 	if(elementsToGo == 0)
 	{
		// Pass indication that message has been transmitted to caller
		// The messageID can help to identify the message
 		if (refHandleApplication)
    	{
        	// If function return false, the link shall not be reserved any longer!
        	if (!refHandleApplication->messageSendComplete(messageID))
        	{
	            messageID = -1;
        	}
    	}
        
	    free(outputMessageVarLength);

#ifdef USE_FLAGS_RS232
		asm("bit clr flags FLG5;");    	   	
#endif

		// Semaphore here
	    asm("#include <def21369.h>");
        asm("bit clr mode1 IRPTEN;");
        sendInProgress = false;
        asm("bit set mode1 IRPTEN;");
 	}
    else
    {
    	// If we arrive here, parts of the message still need to be sent
        if (refHandleLink)
        {
            if(!refHandleLink->sendMessage(&ptrContinue, &elementsToGo))
            {
				reportGlobalError(ERROR_RS232_BUSY_OUT_LOWLEVEL);

            	// report error
            	// Semaphore
            	free(outputMessageVarLength);
	            asm("#include <def21369.h>");
            	asm("bit clr mode1 IRPTEN;");
	            sendInProgress = false;
	            asm("bit set mode1 IRPTEN;");
            }
        }  	
        messageID = -1;
    }
}

/*==================================================
 * Function to be called to initiate a send process 
 * with a reduced set of parameters.
 *///===============================================
bool
CuartDispatcherGeneric::sendMessageNonBlocking(DATATYPE_RS232_CHAR purpose,
	DATATYPE_RS232* ptrField, DATATYPE_RS232_CHAR numElements, WORD32 messageID)
{
	DATATYPE_RS232_CHAR msgPurpose = CuartDispatcherGeneric::PMSG_GENERIC_MESSAGE;
	
	return(sendMessageNonBlocking(msgPurpose, purpose, ptrField, numElements, messageID));
}
	
/*==================================================
 * Function to be called to initiate a send process.
 *///===============================================
bool
CuartDispatcherGeneric::sendMessageNonBlocking(DATATYPE_RS232_CHAR purpose,
	DATATYPE_RS232_CHAR opcodeID, DATATYPE_RS232* ptrField, WORD32 numElements, 
	WORD32 msgID)
{
    short i,j;

    DATATYPE_RS232_CHAR* ptrChar;
    DATATYPE_RS232* ptrInt;
    DATATYPE_RS232 singleWord;
	
    // Start a new transmission
    // Semaphore
	// Allow to be called from different threads/interrupts
	asm("#include <def21369.h>");
  	asm("bit clr mode1 IRPTEN;");
	if(sendInProgress == false)
	{
    	sendInProgress = true;
	    asm("bit set mode1 IRPTEN;");
#ifdef USE_FLAGS_RS232
		asm("bit set flags FLG5;");    	   	
#endif
		messageID = msgID;
		elementsToGo = (numElements*DATATYPE_LENGTH_RS232)+8;
		
		MEMORY_THREADSAFE_START		
		outputMessageVarLength = (DATATYPE_RS232_CHAR*)MALLOC2(elementsToGo);
		MEMORY_THREADSAFE_STOP
		ptrContinue = outputMessageVarLength;
		ptrChar = outputMessageVarLength;
		
        *ptrChar++ = 0xFF & SYNCWORD_DEFAULT1;
        *ptrChar++ = 0xFF & SYNCWORD_DEFAULT2;
        *ptrChar++ = 0xFF & SYNCWORD_DEFAULT3;
        *ptrChar++ = 0xFF & SYNCWORD_DEFAULT4;
        *ptrChar++ = 0xFF & purpose;
        *ptrChar++ = 0xFF & opcodeID;
#ifdef N32BIT_TO_CHAR_MSBFIRST
		*ptrChar++ = 0xFF & (numElements >> 8); 
		*ptrChar++ = 0xFF & (numElements); 
#else
		*ptrChar++ = 0xFF & (numElements); 
		*ptrChar++ = 0xFF & (numElements >> 8); 
#endif

#ifdef N32BIT_TO_CHAR_MSBFIRST
		ptrChar = &outputMessageVarLength[8+numElements*DATATYPE_LENGTH_RS232-1];
		ptrInt = ptrField+numElements-1;
		
		for(i = 0; i < (numElements); i++)
		{
			singleWord = *ptrInt--;
			for(j = 0; j < DATATYPE_LENGTH_RS232; j++)
			{
				*ptrChar-- = 0xFF & singleWord;
				singleWord = singleWord >> 8;
			}
		}
#else
		// ptrChar has correct position
		ptrInt = ptrField;
		
		for(i = 0; i < (numElements); i++)
		{
			singleWord = *ptrInt++;
			for(j = 0; j < DATATYPE_LENGTH_RS232; j++)
			{
				*ptrChar++ = 0xFF & singleWord;
				singleWord = singleWord >> 8;
			}
		}
#endif
		
			
        // Here start sending
        if (refHandleLink)
        {
            if(!refHandleLink->sendMessage(&ptrContinue, &elementsToGo))
            {
				reportGlobalError(ERROR_RS232_BUSY_OUT_LOWLEVEL);
            	// report error
            	// Semaphore
            	free(outputMessageVarLength);
	            asm("#include <def21369.h>");
            	asm("bit clr mode1 IRPTEN;");
	            sendInProgress = false;
	            asm("bit set mode1 IRPTEN;");
            }
        }
    }
    else
    {
    	// Semaphore end
	    asm("bit set mode1 IRPTEN;");
    }

    return(true);
}

/*==================================================
 * Function gateway to report en error through RS232. The only 
 * function argument is an ID.
 *///===============================================
bool
CuartDispatcherGeneric::reportError(WORD32 errID)
{
	return(sendMessageNonBlocking(PMSG_REPORT_ERROR,
	(DATATYPE_RS232*)&errID, 1, -1));
}

/*==================================================
 * Interrupt service routine to process an incoming message.
 * If this function is called all bytes have been received before.
 *///===============================================
void processMessageUser(int sigInt)
{
	WORD32 tempVal1;
	WORD32 tempVal2[2];

	char fldName[STRING_LENGTH];
	DATATYPE_RS232 cmpctFld[STRING_LENGTH/4];
	WORD16 i, j;
	
	float* ptrFieldOut;
	WORD32 numValuesOut;
	
#ifdef DEBUG_PROJECT_RS232
	asm("#include <def21369.h>");
    asm("bit clr mode1 IRPTEN;");
#endif

	switch(theDispatcher->processInputMessage.msgPurpose)
	{
		// Handshake: Respond with terminate Handshake
		case CuartDispatcherGeneric::PMSG_REQUEST_HANDSHAKE:
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_HANDSHAKE,0, &tokenHello[0], 6, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		case CuartDispatcherGeneric::PMSG_REQUEST_GOODBYE:
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_GOODBYE,0, &tokenHello[0], 6, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		// Enable re-synchronization
		case CuartDispatcherGeneric::PMSG_REQUEST_SYNCHRONIZATION:				// Suppoert for only one type of message
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_SYNCHRONIZATION, 0, NULL, 0, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		// Generic message: passed to receiver
		case CuartDispatcherGeneric::PMSG_GENERIC_MESSAGE:
			theDispatcher->refHandleApplication->incomingDefaultMessage(
			theDispatcher->processInputMessage.msgOpcode,
			theDispatcher->processInputMessage.loadFieldToProcess, 
			theDispatcher->processInputMessage.lLoadField);									
			break;
#ifdef DEBUG_MATLAB			
		case CuartDispatcherGeneric::PMSG_REQUEST_HSK_DEBUG_MODE:
			theDebugCodeClass.debugActive = true;
			theDispatcher->stateDebugger = CuartDispatcherGeneric::DBG_ST_DEBUG_MODE;
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_HSK_DEBUG_MODE,0, &tokenHello[0], 6, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		case CuartDispatcherGeneric::PMSG_REQUEST_GBY_DEBUG_MODE:
			theDebugCodeClass.debugActive = false;
			theDispatcher->stateDebugger = CuartDispatcherGeneric::DBG_ST_NORMAL_MODE;
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_GBY_DEBUG_MODE,0, &tokenHello[0], 6, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		case CuartDispatcherGeneric::PMSG_REQUEST_NUMBER_FUNCTIONS:
			tempVal1 = theDebugCodeClass.returnNumberFcts();
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_NUMBER_FUNCTIONS,0, (DATATYPE_RS232*)&tempVal1, 4, -1))
//			if(!theDispatcher->sendMessageNonBlocking(,0, &tempVal1, 4, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;		
		case CuartDispatcherGeneric::PMSG_REQUEST_FUNCTION_NAME_ID:
			if(theDispatcher->processInputMessage.lLoadField > 0)
			{
				tempVal1 = 	theDispatcher->processInputMessage.loadFieldToProcess[0];
				theDebugCodeClass.copyNameFct(fldName, tempVal1);
				for(i = 0; i < STRING_LENGTH/4; i++)
				{
					cmpctFld[i] = 0;
					for(j = 3; j >= 0; j--)
					{
						cmpctFld[i] <<= 8;
						cmpctFld[i] |= (fldName[i*4+j]&0xFF);
					}
				}
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_FUNCTION_NAME_ID,0, (DATATYPE_RS232*)cmpctFld, STRING_LENGTH/4, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}
			}
			else
			{
				if(!theDispatcher->reportError(ERR_NO_INPUT_ID_FUNCTION_NAME))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
				}
			}
			break;		
		case CuartDispatcherGeneric::PMSG_REQUEST_FUNCTION_PARAM_ID:
			if(theDispatcher->processInputMessage.lLoadField > 0)
			{
				tempVal1 = 	theDispatcher->processInputMessage.loadFieldToProcess[0];
				theDebugCodeClass.getInOutArgs(&tempVal2[0], &tempVal2[1], tempVal1);
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_FUNCTION_PARAM_ID,0, (DATATYPE_RS232*)tempVal2, 2, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}
			}
			else
			{
				if(!theDispatcher->reportError(ERR_NO_INPUT_FUNCTION_PARAM))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
				}
			}
			break;		
		case CuartDispatcherGeneric::PMSG_REQUEST_FUNCTION_CALL:
			if(theDispatcher->processInputMessage.lLoadField > 2)
			{
				tempVal1 =  theDispatcher->processInputMessage.loadFieldToProcess[0];
				tempVal2[0] = theDispatcher->processInputMessage.loadFieldToProcess[1];
				tempVal2[1] = theDispatcher->processInputMessage.loadFieldToProcess[2];
				if(theDispatcher->processInputMessage.lLoadField == tempVal2[1]+3)
				{
					theDebugCodeClass.prepareFctCall(tempVal2[0], tempVal2[1], tempVal1, (WORD32*)&theDispatcher->processInputMessage.loadFieldToProcess[3]);
					theDispatcher->stateDebuggerPrevious = theDispatcher->stateDebugger;
					theDispatcher->stateDebugger = CuartDispatcherGeneric::DBG_ST_FUNCTION_PROCESSING;
					if(!theDispatcher->sendMessageNonBlocking(
						CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_FUNCTION_CALL,0, NULL, 0, -1))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT);
					}
				}
				else
				{
					if(!theDispatcher->reportError(ERR_FUNCTION_CALL_REQUEST_I))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}
			}
			else
			{
				if(!theDispatcher->reportError(ERR_FUNCTION_CALL_REQUEST_II))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
				}
			}
			break;
		case CuartDispatcherGeneric::PMSG_REQUEST_SEND_INPUT_PARAMETER:
			//tempVal1 = theDispatcher->processInputMessage.loadFieldToProcess[0];
			if((tempVal1 = theDebugCodeClass.addFieldInput((float*)theDispatcher->processInputMessage.loadFieldToProcess,
				theDispatcher->processInputMessage.lLoadField)) != 0)
			{
				theDebugCodeClass.setBack();
				if(tempVal1 == -2)
				{
					if(!theDispatcher->reportError(ERR_INPUT_PARAM_STATE))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}	
				if(tempVal1 == -1)
				{
					if(!theDispatcher->reportError(ERR_TOO_MANY_INPUT_PARAM))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}	
			}
			else
			{
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_SEND_INPUT_PARAMETER,0, NULL, 0, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}

			}
			break;
		case CuartDispatcherGeneric::PMSG_REQUEST_EXECUTE_FUNCTION:
			if((tempVal1 =theDebugCodeClass.executeFunction()) == 0)
			{
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_EXECUTE_FUNCTION,0, 
						(DATATYPE_RS232*)&tempVal1, 1, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}
				if(!theDebugCodeClass.areThereOutputArgs())
				{
					theDebugCodeClass.setBack();
				}					
			}
			else
			{
				theDebugCodeClass.setBack();
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_EXECUTE_FUNCTION,0, 
						(DATATYPE_RS232*)&tempVal1, 1, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}
				
				/*
				if(tempVal1 == -2)
				{
					if(!theDispatcher->reportError(ERR_EXECUTE_STATE))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}
				else
				{	
					if(tempVal1 == -1)
					{
						if(!theDispatcher->reportError(ERR_EXECUTE_FUNCTION_ID))
						{
							reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
						}
					}		
					else
					{
						if(!theDispatcher->reportError(tempVal1))
						{
							reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
						}
					}
				}*/		
			}
			break;	
		case CuartDispatcherGeneric::PMSG_REQUEST_REQUEST_OUTPUT_PARAMETER:
			tempVal1 = theDebugCodeClass.getNextFieldOutput(&ptrFieldOut, &numValuesOut);
			if((tempVal1 == 0)||(tempVal1 == 1))
			{
				if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_REQUEST_OUTPUT_PARAMETER,0, (DATATYPE_RS232*)ptrFieldOut, numValuesOut, -1))
				{
					reportGlobalError(ERROR_RS232_BUSY_OUT);
				}
				
				if(tempVal1 == 1)
				{
					theDebugCodeClass.setBack();
					theDispatcher->stateDebugger = theDispatcher->stateDebuggerPrevious;
				}
			}
			else
			{
				theDebugCodeClass.setBack();
				if(tempVal1 == -2)
				{
					if(!theDispatcher->reportError(ERR_OUTPUT_PARAM_STATE))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}	
				if(tempVal1 == -1)
				{
					if(!theDispatcher->reportError(ERR_TOO_MANY_OUTPUT_PARAM))
					{
						reportGlobalError(ERROR_RS232_BUSY_OUT_REPORT_ERROR);
					}
				}	
			}
			break;	
		case CuartDispatcherGeneric::PMSG_REQUEST_STATUS:
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_REQUEST_STATUS,0, (DATATYPE_RS232*)&theDispatcher->stateDebugger, 1, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
#endif
		case CuartDispatcherGeneric::PMSG_REQUEST_PROCESSING_PARAMETERS:
			if(!theDispatcher->sendMessageNonBlocking(
					CuartDispatcherGeneric::PMSG_ACKNOWLEDGE_PROCESSING_PARAMETERS,
					0, (DATATYPE_RS232*)procParaFld, 5, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
		default:
		case CuartDispatcherGeneric::PMSG_NOT_SUPPORTED:
			if(!theDispatcher->sendMessageNonBlocking(
				CuartDispatcherGeneric::PMSG_NOT_SUPPORTED, 0, NULL, 0, -1))
			{
				reportGlobalError(ERROR_RS232_BUSY_OUT);
			}
			break;
	}	

	free(theDispatcher->processInputMessage.loadFieldToProcess);
	
    theDispatcher->processInputMessage.inProcessing = 0;
#ifdef DEBUG_PROJECT_RS232
	asm("bit set mode1 IRPTEN;");
#endif
}
#endif
