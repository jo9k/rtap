/******************************************************
 * Filename: uartIND.c								  *
 * Description: Low level driver to access RS232 to   *
 *				transmit bytes. A dispatcher must be  *
 *				put on top of this to transform the   *
 *				bytes into useful messages.			  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

#include <def21369.h>
#include <Cdef21369.h>
#include <signal.h>
#include <string.h>
#include "uartIND.h"
#include "IuartDispatcher.h"

#define SRUDEBUG  // Check SRU Routings for errors.
#include <SRU.h>

// Declare interrupt service routines
void UARTisr_receiveSample(int sigInt);
void UARTisr_receiveDMA(int sigInt);
void UARTisr_transmitDMA(int sigInt);

//================================================================
// LOCATE IN INTERNAL MEMORY
//================================================================

// DMA Transfer control structs
#define PCI 0x00080000
#define OFFSET 0x00080000
int TCB_Block_DMA_UARTRX_0[5] = { 0, MAX_NUM_BYTES_STREAMING, 1, 0, 0};
int TCB_Block_DMA_UARTRX_1[5] = { 0, MAX_NUM_BYTES_STREAMING, 1, 0, 0};

int TCB_Block_DMA_UARTTX[2][5] = { 0, 0, 1, 0, 0};

DATATYPE_RS232_CHAR outputBufferRS232[2][MAX_NUM_BYTES_STREAMING];
DATATYPE_RS232_CHAR inputBufferRS232[2][MAX_NUM_BYTES_STREAMING];

//================================================================
//================================================================

// Hold a pointer to this class for global ISR 
uartIND RAM_DEFAULT_SECTION* theRS232Class;

static int buf[1000];
int cnt = 0;

/*==================================================
 * Constructor: Setup some static data.
 *///===============================================
uartIND::uartIND()
{
	theRS232Class = this;
    operationMode = RS_232_MODE_DEFAULT;
    callbackRef = 0;
    outBufferidx = 0;
    TCB_Block_DMA_UARTTX[0][0] = (int) PCI ;
    TCB_Block_DMA_UARTTX[0][1] = 0;
    TCB_Block_DMA_UARTTX[0][2] = 1;
    TCB_Block_DMA_UARTTX[0][3] = (unsigned int) outputBufferRS232[0] - OFFSET ;
    TCB_Block_DMA_UARTTX[0][4] = (unsigned int) outputBufferRS232[0];

    TCB_Block_DMA_UARTTX[1][0] = (int) PCI ;
    TCB_Block_DMA_UARTTX[1][1] = 0;
    TCB_Block_DMA_UARTTX[1][2] = 1;
    TCB_Block_DMA_UARTTX[1][3] = (unsigned int) outputBufferRS232[1] - OFFSET ;
    TCB_Block_DMA_UARTTX[1][4] = (unsigned int) outputBufferRS232[1];

}

/*==================================================
 * Initialization: Route all signals as required and
 * setup DMA transfers.
 *///===============================================
void
uartIND::initializeRS232Link(rs232OperationMode mode, IuartDispatcher* disp)
{
    // Set the uart dispatcher link
    callbackRef = disp;

    // Set the operation mode
    operationMode = mode;

    // Route all signals propperly
    SRU2(UART0_TX_O,DPI_PB09_I); // UART transmit signal is connected to DPI pin 9
    SRU2(HIGH,DPI_PBEN09_I);
    SRU2(DPI_PB10_O,UART0_RX_I); // connect the pin buffer output signal to the UART0 receive
    SRU2(LOW,DPI_PB10_I);
    SRU2(LOW,DPI_PBEN10_I);      // disables DPI pin10 as input

    *pPICR2 &= ~(0x1F<<5); //Sets the UART0 receive interrupt to P13
    *pPICR2 |= (0x13<<5);
    *pPICR2 &= ~(0x1F<<15); //Sets the UART0 transmit interrupt to P15
    *pPICR2 |= (0x15<<15);

    // Set the baud rate
    *pUART0LCR = UARTDLAB;  //enables access to Divisor register to set baud rate
	
	// Set the actual baud rate
    //int divisorRS232 = (331776000.0/(32.0*BAUDRATE));
    int divisorRS232 = (331776000.0/(32.0*BAUDRATE));
    *pUART0DLL = divisorRS232 & 0xFF;
    *pUART0DLH = (divisorRS232 >> 8) & 0xFF;

    /* Configures UART0 LCR */
    *pUART0LCR = FLAGS_LINE;

    *pUART0IER = UARTTXFIE | UARTLSIE | UARTRBFIE | UARTTBEIE;

    switch (operationMode)
    {
    case RS_232_MODE_DEFAULT:

        // Also in default mode use DMA, otherwise each byte is reported more
        // than once and I do not know what to do against it.
        TCB_Block_DMA_UARTRX_0[0] = (int) TCB_Block_DMA_UARTRX_1 + 3 - OFFSET + PCI ;
        TCB_Block_DMA_UARTRX_0[1] = 1;
        TCB_Block_DMA_UARTRX_0[3] = (unsigned int) inputBufferRS232[0] - OFFSET ;
        TCB_Block_DMA_UARTRX_0[4] = (unsigned int) inputBufferRS232[0];
        TCB_Block_DMA_UARTRX_1[0] = (int) TCB_Block_DMA_UARTRX_0 + 3 - OFFSET + PCI ;
        TCB_Block_DMA_UARTRX_1[1] = 1;
        TCB_Block_DMA_UARTRX_1[3] = (unsigned int) inputBufferRS232[1] - OFFSET;
        TCB_Block_DMA_UARTRX_1[4] = (unsigned int) inputBufferRS232[1];
        *pUART0RXCTL = UARTEN|UARTDEN|UARTCHEN;       //enables UART0 in receive mode, DMA and DMA chaining

        // Start chaining
        *pCPUART0RX = (unsigned int)TCB_Block_DMA_UARTRX_0-OFFSET+3;
        interrupt(SIG_P13,UARTisr_receiveSample);
//	    	//enables UART0 in receive mode
//	    	*pUART0RXCTL = UARTEN;
//	    	interrupt(SIG_P13,UARTisr_receiveSample);
        break;
    case RS_232_MODE_STREAMING:

        // Streaming mode purely DMA based
        TCB_Block_DMA_UARTRX_0[0] = (int) TCB_Block_DMA_UARTRX_1 + 3 - OFFSET + PCI ;
        TCB_Block_DMA_UARTRX_0[3] = (unsigned int) inputBufferRS232[0] - OFFSET ;
        TCB_Block_DMA_UARTRX_0[4] = (unsigned int) inputBufferRS232[0];
        TCB_Block_DMA_UARTRX_1[0] = (int) TCB_Block_DMA_UARTRX_0 + 3 - OFFSET + PCI ;
        TCB_Block_DMA_UARTRX_1[3] = (unsigned int) inputBufferRS232[1] - OFFSET;
        TCB_Block_DMA_UARTRX_1[4] = (unsigned int) inputBufferRS232[1];
        *pUART0RXCTL = UARTEN|UARTDEN|UARTCHEN;       //enables UART0 in receive mode, DMA and DMA chaining

        // Start chaining
        *pCPUART0RX = (unsigned int)TCB_Block_DMA_UARTRX_0-OFFSET+3;
        interrupt(SIG_P13,UARTisr_receiveDMA);
        break;
    }

    // Setup DMA interrupt service for completed transmission,
    // the rest for each DMA transfer will be done on call of send function
    interrupt(SIG_P15,UARTisr_transmitDMA);

}

/*==================================================
 * Terminate RS 232 link, set into inactive state.
 *///===============================================
void
uartIND::terminateRS232Link()
{
    callbackRef = 0;
}

/*==================================================
 * Not suuported at the moment.
 *///===============================================
void
uartIND::switchToMode(rs232OperationMode mode)
{
    // Dynamic switching not yet supported
    return;
}

/*==================================================
 * Function t initiate the send process of a new message.
 * Note that this function is not thread safe, the 
 * dispatcher must take care of this aspect.
 *///===============================================
bool
uartIND::sendMessage(DATATYPE_RS232** fldSendBytes, int* numElementsToGo)
{
	int numElementsSend = *numElementsToGo;
    if ( *numElementsToGo < MAX_NUM_BYTES_STREAMING)
    {
    	*numElementsToGo = 0;    	
    }
    else
    {
    	*numElementsToGo -= MAX_NUM_BYTES_STREAMING;
    	numElementsSend = MAX_NUM_BYTES_STREAMING;
    }

	memcpy(outputBufferRS232[outBufferidx], *fldSendBytes, numElementsSend);

	*fldSendBytes += numElementsSend;
	
	// Stop DMA but do not stop DMA chaining
	*pUART0TXCTL = UARTEN|UARTCHEN;

    // Set the number of elements to transmit
    TCB_Block_DMA_UARTTX[outBufferidx][1] = numElementsSend;

    // Store the TCB start address
    unsigned int* addrStart = (unsigned int*)TCB_Block_DMA_UARTTX[outBufferidx];

    // Update the modulo counter
    outBufferidx = (outBufferidx+1);
    outBufferidx = outBufferidx%2;

    // Restart DMA including the next data field for RS232 transmission
    *pCPUART0TX = (unsigned int)addrStart -OFFSET+3;
    *pUART0TXCTL = UARTEN|UARTDEN|UARTCHEN;       //enables UART0 in receive mode, DMA and DMA chaining

        /* Old version that demanded a delay in the transmit ISR
        //memcpy(outputBuffer, fld, numBytes);
         *pUART0TXCTL = 0;
         *pIIUART0TX = (unsigned int)outputBuffer-OFFSET;
         *pIMUART0TX = 1;
         *pCUART0TX = numBytesAll;
        *pUART0TXCTL = UARTEN|UARTDEN;       //enables UART0 in receive mode, DMA and DMA chaining
        return(true);*/

	return(true);
}

/*==================================================
 * Not supported
 *///===============================================
/*bool
uartIND::sendMessageStreaming(char fld[MAX_NUM_BYTES_STREAMING])
{

#ifdef UART_SAFE_TRANSMISSION
    bool result = true;

    // Semaphore start
    asm("#include <def21369.h>");
    asm("bit clr mode1 IRPTEN;");
    if (transmissionBusy)
    {
        result = false;
    }
    else
    {
        transmissionBusy = true;
    }
    // Semaphore stop
    asm("bit set mode1 IRPTEN;");

    if (result == true)
    {
#endif
        memcpy(outputBuffer[outBufferidx], fld, MAX_NUM_BYTES_STREAMING);
        *pUART0TXCTL = 0;
        *pIIUART0TX = (unsigned int)outputBuffer-OFFSET;
        *pIMUART0TX = 1;
        *pCUART0TX = MAX_NUM_BYTES_STREAMING;
        *pUART0TXCTL = UARTEN|UARTDEN;       //enables UART0 in receive mode, DMA and DMA chaining
        outBufferidx = (outBufferidx+1)/2;
        return(true);

#ifdef UART_SAFE_TRANSMISSION
    }
    return(false);
#endif
//    return(sendMessage(fld, MAX_NUM_BYTES_STREAMING));
}
*/

/*==================================================
 * Not supported
 *///===============================================
/******************************************************/

/******************************************************/
/* INTERRUPT SERVICE ROUTINES */
/******************************************************/
/* INTERRUPT SERVICE ROUTINES */
/******************************************************/

/*==================================================
 * Function to rteceive another byte in the default mode.
 * If switching to streaming mode, also the Interrupts 
 * must be routed to another ISR.
 *///===============================================
 void UARTisr_receiveSample(int sigInt)
{
    unsigned int* addrUARTBuf = (unsigned int*)*pCPUART0RX;
    unsigned int* ptrUARTInputField = (unsigned int*)addrUARTBuf[1];
    DATATYPE_RS232_CHAR inChar;

    inChar = (DATATYPE_RS232_CHAR)*ptrUARTInputField;
	buf[cnt++] = inChar;
	if(cnt == 1000)
		cnt = 0;
    theRS232Class->callbackRef->byteReceivedDefault(inChar);


}

/*==================================================
 * Function to receive a complete message in streaming mode.
 * Currently not supported.
 *///===============================================
void UARTisr_receiveDMA(int sigInt)
{
    unsigned int* addrUARTBuf = (unsigned int*)*pCPUART0RX;
    unsigned int* ptrUARTInputField = (unsigned int*)addrUARTBuf[1];
    theRS232Class->callbackRef->messageReceivedStreaming((DATATYPE_RS232_CHAR*)ptrUARTInputField);
}

/*==================================================
 * Function to indicate that a tramnsmit sequence has 
 * been completely send (reported from DMA).
 *///===============================================
void UARTisr_transmitDMA(int sigInt)
{
    theRS232Class->callbackRef->messageSendComplete();
}
/******************************************************/

