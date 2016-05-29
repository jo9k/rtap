/* 
 *****************************************************
 * Filename: IjvxWindowManager.h
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

#ifndef __IJVXWINDOWMANAGER_H__
#define __IJVXWINDOWMANAGER_H__

JVX_INTERFACE IjvxWindowManager: public IjvxObject, public IjvxHiddenInterface
{
	
public:

	virtual JVX_CALLINGCONVENTION ~IjvxWindowManager(){};

	// Add a command line argument to window manager (if required, for QT5 we need this!)
	virtual jvxErrorType JVX_CALLINGCONVENTION addCommandLineArgWindowManager(const char* arg) = 0;

	//! API: Function to initialize the window manager. two additional parameters may extend functionality in the future
	virtual jvxErrorType JVX_CALLINGCONVENTION initializeWindowManager(void* fld, size_t szFld) = 0;

	//! API: Function to terminate the window manager
	virtual jvxErrorType JVX_CALLINGCONVENTION terminateWindowManager() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getStateWindowManager(jvxState* returnedState) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getTypeWindowManager(jvxWindowManagerType* returnedType) = 0;

	//! API: Register a control window object
	virtual jvxErrorType JVX_CALLINGCONVENTION registerControlWindowObject(IjvxControlWindow* object, 
		IjvxObject* jvxObj, const char* nameCW, jvxHandle** hdlControlWindow, 
		IjvxWindowManagerBackward* backRef = NULL, jvxHandle* privateData = NULL) = 0;

	//! API: Register a control window object
	virtual jvxErrorType JVX_CALLINGCONVENTION unregisterControlWindowObject(jvxHandle* hdlControlWindow) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getControlWindowForHandle(IjvxControlWindow** outObj, jvxHandle* handle) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getObjectForHandle(IjvxObject** outObj, jvxHandle* handle) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION getHandleForObject(jvxHandle** outHandle, IjvxObject* inObj) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION winManagerSpecific(jvxHandle* object, jvxInt32 commandId, void* fld, size_t szFld) = 0;

	//! Function to initialize another control window
	virtual jvxErrorType JVX_CALLINGCONVENTION decoupleControlWindow(jvxHandle* object, jvxState oldState, jvxHandle* dataType = NULL, 
		jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION syncControlWindow(jvxHandle* object, jvxState newState, jvxHandle* dataType = NULL, 
		jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION updateMainWindow() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION openControlWindow(jvxHandle* idObject, jvxHandle* dataType = NULL, 
		jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION closeControlWindow(jvxHandle* object, jvxHandle* dataType = NULL, 
		jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION isControlWindowOpen(jvxHandle* object, bool* isOpen,  
		jvxHandle* dataType = NULL, jvxControlWindowDataType tpDataField = JVX_DATATYPE_CONTROL_WINDOW_NONE) = 0;
};

#endif
