/*============================================================
 * FILE: IgenericRuntime.h
 *============================================================
 * PURPOSE: Interface for the generic runtime configuration principle.
 *			Each component exposes this interface and with this, two member functions.
 *			If a component does not support the generic principle, all
 *			functions must be implemented but simply return false in the member function.
 *============================================================
 * COPYRIGHT/AUTHOR:	IND, RWTH-Aachen, Hauke Krueger, hauke@ind.rwth-aachen.de 
 *============================================================
 * For bugs, write an email to audiodev@ind.rwth-aachen.de
 *============================================================*/ 

#ifndef __IRTPGENERICRUNTIME_H__
#define __IRTPGENERICRUNTIME_H__


/**
 * Abstract base class for generic runtime messaging. The communication with this type of object
 * is based on the access to internal memory space. The class therefore returns a pointer to that memory
 * space in one member function and accepts that the returned memory space is modified.
 * In a second member function the exposed memory is passed to the component and the 
 * modifications in the memory space are transferred to the right position in the component internally.
 * The two function in general are called from within a different thread than the main audio processing thread.
 * The exposed memory should never be used internally directly!!
 *
 * For a detailled description, refer to the rtprocdoc.pdf document.
 *
 *///======================================================


#ifdef __DSP_MODE__


//================================================
// DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP DSP
//================================================

 class IrtpGenericRuntime
{
public:
	//! Generate a pointer to a template for the runtime struct. This template may be considered as shared mem
    virtual bool getGenericConfigureTemplate(rtpGenericConfigure_DSP** ptrTemplateOnReturn) =  0;
    
    //! Set the value for generic runtime parameters. The pointer passed must be the template returned previously
    virtual bool exchangeDataConfigure(rtpGenericConfigure_DSP* ptrConfigure) = 0;
};

#else // __DSP_MODE__ is not defined

//==========================================
// PC PC PC PC PC PC PC PC PC PC PC PC PC PC
//==========================================

#include <windows.h>

 interface IrtpGenericRuntime
{
public:
	//! Generate a pointer to a template for the runtime struct. This template may be considered as shared mem
	virtual bool RTPROC_CALLINGCONVENTION getGenericConfigureTemplate(TrtpGenericRuntime** ptrTemplateOnReturn) = 0;

	//! Set the value for generic runtime parameters. The pointer passed must be the template returned previously
	virtual bool RTPROC_CALLINGCONVENTION exchangeDataConfigure(TrtpGenericRuntime* ptrConfigure) = 0;
};

#endif // __IRTPGENERICRUNTIME_H__



#endif // __DSP_MODE__ is not defined
