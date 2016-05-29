#ifndef __HJVXRS232_H__
#define __HJVXRS232_H__

static void Hjvx_initRs232Default(jvxRs232Config* cfg)
{
	cfg->bRate = 38400;
	cfg->bits4Byte = 8;
	cfg->stopBitsEnum = JVX_RS232_STOPBITS_ONE;
	cfg->parityEnum = JVX_RS232_PARITY_NO;
	cfg->enFlow = JVX_RS232_NO_FLOWCONTROL;
	cfg->boostPrio = false;
}

#endif