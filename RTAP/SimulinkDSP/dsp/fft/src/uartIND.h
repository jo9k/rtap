/******************************************************
 * Filename: uartIND.h								  *
 * Description: UART functionality class. This class  *
 *				is the DRIVER to access RS232 		  *
 * 				messages. the functionality is full   *
 *				duplex. On the incoming side, each 	  *
 *				byte will be reported to the 		  *
 *				dispatcher, on the outgoing side, DMA *	
 *				transfers are used.					  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#ifndef __UART_IND_H__
#define __UART_IND_H__

/**************************************************************/
// Macro to define the length of all string fields
#define STRING_LENGTH 16 // <---- must be multiple of 4!
/**************************************************************/

#include "userDef.h"
#include "typedefs.h"
class IuartDispatcher;

/*========================================================
 * UART transmission class, containing some very basic functions
 * to transmit and receive bytes
 *///========================================================
class uartIND
{
public:

    //! Two operation modes, DEFAULT MODE (byte after byte)
    //! and STREAMING MODE
    typedef enum
    {
        RS_232_MODE_DEFAULT = 0,
        RS_232_MODE_STREAMING = 1
    } rs232OperationMode;

#ifdef UART_SAFE_TRANSMISSION
    //! State machine, only state ready and not ready
    typedef enum
    {
        RS_232_STATE_NOT_READY = 0,
        RS_232_STATE_READY = 1
    } rs232State;
#endif

private:
    // DMA buffers for receiving/transmission
   short outBufferidx;

#ifdef UART_SAFE_TRANSMISSION
public:
    // Semaphore to protect DMA buffers against parallel access
    bool transmissionBusy;

private:
    // Component state
    rs232State state;
#endif

    // Operation mode
    rs232OperationMode operationMode;

public://protected:
    // Backward reference link
    IuartDispatcher* callbackRef;

public:

    // Constructor
    uartIND();

    // Initialization function
    virtual void initializeRS232Link(rs232OperationMode mode, IuartDispatcher* disp);

    // Inverse of initialization function
    virtual void terminateRS232Link();

    // Mode switching function
    virtual void switchToMode(rs232OperationMode mode);

    // Send message function for integer buffers, header part and load part
    virtual bool sendMessage(DATATYPE_RS232_CHAR** charField, int* numElementsToGo);

    // Send streaming message
    //virtual bool sendMessageStreaming(char fld[MAX_NUM_BYTES_STREAMING]);

#ifdef UART_EXTENSIONS
    virtual int returnTransmissionCounter();
#endif

};

#endif
