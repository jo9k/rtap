#ifndef __TJVXRS232_H__
#define __TJVXRS232_H__

typedef enum
{
	JVX_RS232_NO_FLOWCONTROL,				// No flowcontrol
	JVX_RS232_CTSRTSFLOWCONTROL,
	JVX_RS232_CTSDTRFLOWCONTROL,
	JVX_RS232_DSRRTSFLOWCONTROL,
	JVX_RS232_DSRDTRFLOWCONTROL,
	JVX_RS232_XONXOFFFLOWCONTROL
} jvxRs232FlowControlEnum;

typedef enum
{
	JVX_RS232_REPORT_NONE = 0,
	JVX_RS232_REPORT_CTS = 0x1,		// Report CTS set/unset
	JVX_RS232_REPORT_RLSD = 0x2,		// Report RLSD set/unset
	JVX_RS232_REPORT_RING = 0x4,		// Report Ring line state changed
	JVX_RS232_REPORT_DSR = 0x8,		// Report DSR state changed
	JVX_RS232_REPORT_ANY = 0xF		// Report all events
} jvxRs232ReportEnum;

typedef enum
{
	JVX_RS232_STOPBITS_ONE,
	JVX_RS232_STOPBITS_ONE5,
	JVX_RS232_STOPBITS_TWO
} jvxRs232StopbitsEnum;

typedef enum
{
	JVX_RS232_PARITY_EVEN,
	JVX_RS232_PARITY_MARK, 
	JVX_RS232_PARITY_ODD, 
	JVX_RS232_PARITY_NO,
	JVX_RS232_PARITY_SPACE
} jvxRs232ParityEnum;

typedef struct
{
	int bRate;
	int bits4Byte;
	jvxRs232StopbitsEnum stopBitsEnum; 
	jvxRs232ParityEnum parityEnum;
	jvxRs232FlowControlEnum enFlow;
	jvxBool boostPrio;
	int reportEnum;
} jvxRs232Config;


#endif
