/* 
 *****************************************************
 * Filename: IjvxWindowManagerBackward.h
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

#ifndef __IJVXWINDOWMANAGERBACKWARD_H__

JVX_INTERFACE IjvxWindowManagerBackward
{
public:
  virtual JVX_CALLINGCONVENTION ~IjvxWindowManagerBackward(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION reportWindowStatusChanged(jvxWindowStatusChangedType tp, jvxHandle* privateData) = 0;
};

#endif
