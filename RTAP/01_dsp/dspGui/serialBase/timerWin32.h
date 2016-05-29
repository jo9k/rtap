// timerWin32.h: Schnittstelle für die Klasse timerWin32.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TIMERWIN32_H__
#define _TIMERWIN32_H__

#include <windows.h>

class serialConnectionUseRef;

/**
 * Class for the implementation of a timer functionality using WIN32
 * functions. This class should not use the methods as it is done.
 * There should be other timer functions available in WIndows
 *///=========================================================
class timerWin32  
{
private:

	//! Source of the timer
	serialConnectionUseRef* serialRef;

	//! Event to indicate starting point
	HANDLE startEvent;

	//! Event to indicate stopping time
	HANDLE stopEvent;

	//! State: Is timer currently in processing?
	bool stateProcessing;

	//! Timer timeout
	int tMsec;

	//! Critical section to change state of the timer (protect)
	CRITICAL_SECTION sectionTimerAccess;

	//! Thread for waiting
	HANDLE threadTimer;

	//! ID of thread for waiting
	DWORD threadID;

public:
	//! Constructor
	timerWin32();

	//! Destructor
	virtual ~timerWin32();

	//! Class internal timer thread fct.
	DWORD timerThread();

	//! Initialize the timer
	void initialize(serialConnectionUseRef* ref);

	//! Set the length of the waiting period
	bool setTimerLength(int msec);

	//! Start to wait
	bool startTimerWin32();

	//! Stop the timer
	bool stopTimerWin32();

	//! Callback in case that timer has been stopped
	virtual void timeoutProcess();
};

#endif // !defined(AFX_TIMERWIN32_H__FA74EFE3_17A5_4F7A_BD20_55FA6A8AEB2C__INCLUDED_)
