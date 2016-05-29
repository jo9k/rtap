#ifndef __IRTPEXTENSIONS_H__
#define __IRTPEXTENSIONS_H__


#ifdef __DSP_MODE__
class IrtpExtensions
{
	//! Gateway function to get references to extended interfaces
	virtual bool requestInterface(void** returnedInterface, UWORD32 interfaceIdentifier) = 0;
	
	//! If interface is not required anymore, return the used handle
	virtual bool returnInterface(void* returnedInterface, UWORD32 interfaceIdentifier) = 0;
};
#else // __DSP_MODE__ is not defined

#include <windows.h>

interface IrtpExtensions
{
	//! Gateway function to get references to extended interfaces
	virtual bool RTPROC_CALLINGCONVENTION requestInterface(rtpHandle** returnedInterface, rtpIdx interfaceIdentifier) = 0;
	
	//! If interface is not required anymore, return the used handle
	virtual bool RTPROC_CALLINGCONVENTION returnInterface(rtpHandle* returnedInterface, rtpIdx interfaceIdentifier) = 0;
};

#endif // __DSP_MODE__
#endif // __IRTPEXTENSIONS_H__
