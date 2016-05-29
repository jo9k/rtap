/* 
 *****************************************************
 * Filename: IjvxHostExtensions.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description:                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __IJVXHOSTEXTENSIONS_H__
#define __IJVXHOSTEXTENSIONS_H__


JVX_INTERFACE IjvxHostExtensions
{
public:
	 virtual JVX_CALLINGCONVENTION ~IjvxHostExtensions(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION supportsFeature(jvxSize feature, jvxBool* yes_supports) = 0;
	
	// Multithreading features of host
	virtual jvxErrorType JVX_CALLINGCONVENTION hardwareSetupChanged() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION softwareSetupChanged() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION startProcessing() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION stopProcessing() = 0;
};

#endif
