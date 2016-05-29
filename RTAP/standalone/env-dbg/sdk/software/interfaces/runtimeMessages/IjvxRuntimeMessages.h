/* 
 *****************************************************
 * Filename: IjvxRuntimeMessages.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: Interface for the runtime messaging 
 *			communication principle in JVXroc.
 *			Each component exposes this JVX_INTERFACE and 
 *			with this, the member function.
 *			If a component does not support the 
 *			messaging principle, all functions must 
 *			be implemented but simply return false in 
 *			the member function.
 *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#ifndef _RUNTIMEMESSAGE__H__
#define _RUNTIMEMESSAGE__H__

/**
 * Reference between components to enable runtime messaging.
 *
 * For a detailled description, refer to the jvxrocdoc.pdf document.
 *
 *///===========================================================
JVX_INTERFACE IjvxRuntimeMessages
{
public:
  virtual JVX_CALLINGCONVENTION ~IjvxRuntimeMessages(){};

	//! Functionality for runtime messaging, receiver callback. This function should be non blocking (copy data field!)
	virtual jvxErrorType JVX_CALLINGCONVENTION sendRuntimeMessage(TjvxRuntimeMessage* specificData) = 0;
};

#endif
