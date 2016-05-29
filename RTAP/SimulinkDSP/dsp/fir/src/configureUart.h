/******************************************************
 * Filename: configureUART.h						  *
 * Description: Configuration file to set RS232 	  *
 *				processing parameters. Only for the   *
 *				expert.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #ifndef __CONFIGURE_UART_H__
#define __CONFIGURE_UART_H__


/********************************************/
// Configure the transmission baudrate
#ifdef USE_UART_FOR_MIDI
#define BAUDRATE 31250 //115200 //19200
#else
#define BAUDRATE 115200
#endif
/********************************************/

/********************************************/
// Configure the transmission parameters
#ifdef USE_UART_FOR_MIDI
#define FLAGS_LINE (UARTWLS8	/* word length 8* / | UARTPEN / * parity enable ODD parity*/|UARTSTB /* Two stop bits */);
#else
#define FLAGS_LINE (UARTWLS8	/* word length 8*/ | UARTPEN /* parity enable ODD parity*/|UARTSTB /* Two stop bits */);
#endif
/********************************************/

/********************************************/
// Configure the UART dispatcher for mode
#define DATATYPE_RS232 UWORD32
#define DATATYPE_LENGTH_RS232 4
#define MAX_NUM_ELEMENTS_DISPATCHER (MAX_NUMBER_ELEMENTS_LOAD_RS232)
#define DATATYPE_RS232_CHAR UWORD32
/********************************************/

/********************************************/
// Configure the UART component for streaming mode
#ifdef USE_UART_FOR_MIDI

// For midi, the user should have control over bytes!
#define MAX_NUM_BYTES_STREAMING 1
#else

//! For non midi, we should send as many bytes as required
#define MAX_NUM_BYTES_STREAMING ((MAX_NUM_ELEMENTS_DISPATCHER+2)*DATATYPE_LENGTH_RS232)
#endif
// Condition: MAX_NUM_BYTES_STREAMING >= (MAX_NUM_ELEMENTS_DISPATCHER*DATATYPE_RS232)
/********************************************/

/********************************************/
// Define the order of bytes from int32/float32
#define N32BIT_TO_CHAR_LSBFIRST
//#define N32BIT_TO_CHAR_MSBFIRST
/********************************************/

/********************************************/
// If this define on, always the number of characters will be written to each message
#define WRITE_ALWAYS_NUMBER_CHARS
/********************************************/

#endif
