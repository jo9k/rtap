/* 
 *****************************************************
 * Filename: TjvxControlWindow.h
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

#ifndef __TJVXWINDOWCONFIGURATION_H__
#define __TJVXWINDOWCONFIGURATION_H__

JVX_INTERFACE IjvxWindowManager;

typedef enum
{
	JVX_WINDOW_MANAGER_STATUS_CHANGE_NONE = -1,
	JVX_WINDOW_MANAGER_STATUS_CHANGE_CLOSE = 0,
	JVX_WINDOW_MANAGER_STATUS_CHANGE_OPEN = 1
} jvxWindowStatusChangedType;

//! Operation types, to specify to open a widget or a dialog
typedef enum
{
	JVX_UNSPECIFIED = -1,
	JVX_OPEN_DIALOG = 0,
	JVX_CLOSE_DIALOG = 1,
	JVX_OPEN_WIDGET = 2,
	JVX_CLOSE_WIDGET = 3
} jvxControlWindowOperationType;

typedef enum
{
	JVX_PREFERS_DIALOG = 0,
	JVX_PREFERS_WIDGET = 1
} jvxControlPreferredWindowType;

typedef enum
{
	JVX_DATATYPE_CONTROL_WINDOW_NONE = -1,
	JVX_DATATYPE_CONTROL_WINDOW_OPEN_CLOSE = 0 // TjvxControlWindow_openclose
} jvxControlWindowDataType;

typedef enum
{
	JVX_QT_BLOCK_PENDING_EVENTS_ON = 0,
	JVX_QT_BLOCK_PENDING_EVENTS_OFF = 1,
	JVX_QT_UPDATE_WINDOW_CONTENT_PROCESSING_PERIODIC = 2
} jvxWindowSpecificCommandIds;

//! Struct to pass information to/from the window manager to each new dialog/widget
typedef struct
{
	struct
	{
		//! Size of the field. This can be larger than sizeof(TjvxWindowConfiguration)
		size_t in_szField;

		//! Pass the window manager on initialization
		IjvxWindowManager* in_winManager;

		//! Parent widget for the creation of the new widget
		jvxHandle* in_parentalWindowReference;

		//! Preferred operation
		jvxControlPreferredWindowType in_preferredWindowType;

		//! For future use: self reference to address object in window manager
		jvxHandle* in_selfReferenceHandle;
	
		//! Text field to be displayed in window header (for identification)
		const char* in_ptrTxtTitle;

		bool in_registerWithWinManager;

		// Output
		jvxControlWindowOperationType out_operationPerformed;

		//! Pointer to the newly created widget
		jvxHandle* out_ptrHandleOnReturn;
	
	} common;

} TjvxControlWindow_openclose;

#endif