/*============================================================
 * FILE: IrtpObject.h
 *============================================================
 * PURPOSE: 
 *			For a detailled description, refer to the rtprocdoc.pdf document.
 *
 *============================================================
 * COPYRIGHT/AUTHOR:	IND, RWTH-Aachen, Hauke Krueger, hauke@ind.rwth-aachen.de 
 *============================================================
 * For bugs, write an email to audiodev@ind.rwth-aachen.de
 *============================================================*/ 

/**
 * Interface contains the member function employed to control
 * the generic real-time host.
 *////===================================================================================

#ifndef __IRTPOBJECT_H__
#define __IRTPOBJECT_H__

#ifdef __DSP_MODE__

//================================================
// DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP
//================================================


class IrtpObject: public IrtpGenericRuntime
{
    //! Function to return an objects name
    virtual void getName(char name[STRING_LENGTH]) = 0;
    
    //! Function to return an objects description
    virtual void getDescription(char desc[STRING_LENGTH]) = 0;
    
    //! Function to return an error code if a member function has recently failed.
    virtual void getLastError(unsigned short* errCode, char errDescription[STRING_LENGTH]) = 0;
    
    //! Function to return the coresponding reference to the object
    virtual void getObjectSpecialization(void** link, RTP_OBJECT* type)  = 0;
    
    
    virtual bool sendRuntimeMessage(runtimeMessage_DSP* specificData, unsigned short* errID) = 0;
};

#else // __DSP_MODE__ is not defined

//================================================
// PC PC PC PC PC PC PC PC PC PC PC PC PC PC PC PC
//================================================

#include <windows.h>

interface IrtpObject: public IrtpGenericRuntime
{
public:
	//! Function to return an objects name
	virtual bool RTPROC_CALLINGCONVENTION getName(rtpString** name_argout) = 0;

	//! Function to return an objects description
	virtual bool RTPROC_CALLINGCONVENTION getDescription(rtpString** name_argout) = 0;

	//! Function to return an error code if a member function has recently failed.
	virtual bool RTPROC_CALLINGCONVENTION getLastError(rtpError** err_argout) = 0;

	//! Function to return the coresponding reference to the object
	virtual bool RTPROC_CALLINGCONVENTION getObjectSpecialization(rtpHandle** link, rtpObjectType* type) = 0;

	//! Deallocation function for data type, deallocation of the side of the dynamic library
	virtual void RTPROC_CALLINGCONVENTION deallocate(rtpString* tobeRemoved) = 0;
	
	//! Deallocation function for data type, deallocation of the side of the dynamic library
	virtual void RTPROC_CALLINGCONVENTION deallocate(rtpError* tobeRemoved) = 0;
};

#endif // __DSP_MODE__
#endif // __IRTPOBJECT_H__
