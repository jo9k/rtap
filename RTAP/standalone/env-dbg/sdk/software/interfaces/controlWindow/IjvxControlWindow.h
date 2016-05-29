/* 
 *****************************************************
 * Filename: IjvxControlWindow.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description: Interface for non-generic GUIs in all components
 *			in JVXroc.                                     *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __ICONTROLWINDOW__H__
#define __ICONTROLWINDOW__H__

/**
 *
 *////===================================================================================
JVX_INTERFACE IjvxControlWindow
{
public:
  virtual JVX_CALLINGCONVENTION ~IjvxControlWindow(){};

	//! Return the windows type for the plugin (only if there is one!)
	virtual jvxErrorType JVX_CALLINGCONVENTION canControlWindowType(jvxWindowManagerType winType, bool* result) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION decoupleControlWindow(jvxState state, jvxWindowManagerType winType, jvxHandle* dataType = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	//! Initialize the control window management. This must be done from the central application
	virtual jvxErrorType JVX_CALLINGCONVENTION syncControlWindow(jvxState newState, jvxWindowManagerType winType, jvxHandle* dataHdl = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	//! Function to open the plugin dialog
	virtual jvxErrorType JVX_CALLINGCONVENTION openControlWindow(jvxWindowManagerType winType,jvxHandle* dataHdl = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;
	
	//! Function to close the plugin dialog
	virtual jvxErrorType JVX_CALLINGCONVENTION closeControlWindow(jvxWindowManagerType winType, jvxHandle* dataHdl = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	//! Return whether window is open
	virtual jvxErrorType JVX_CALLINGCONVENTION isControlWindowOpen(jvxWindowManagerType winType, bool* result, jvxHandle* dataHdl = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION winManagerSpecific(jvxWindowManagerType winType, jvxInt32 commandId, void* fld, size_t szFld) = 0;


};

#endif
