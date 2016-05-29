/***********************************************************************
 * DESCRIPTION:
 * timerWin32: Timer class using WIN32 WaitForSingleObject functions.
 ***********************************************************************
 * This  class should be derived from whenever implementing a timer in WIN32.
 * There may be however better methodologies to implement an accurate timer.
 *
 * Copyright Hauke Krueger, IND, 2004, 02-06-04
 *********************************************************************/

#include <string>

#include "timerWin32.h"
#include "serialConnectionUseRef.h"

/**
 * Static function to convert an int into a string
 *///====================================================
static
std::string int2String(int num)
{
	char str[128];
	sprintf(str, "%d", num);
	return(str);
}

//! Static variable for timer name, this must be unambiguous
static long timerCounter = 0;

//! Global function for access to timer class whenever starting a new thread, MUST BE PROTECTED!
static DWORD WINAPI
global_TimerThread(void* param)
{
	timerWin32* this_pointer = (timerWin32*) param;
	return(this_pointer->timerThread());
}

/**
 * Do nothing but initialize
 *///=======================================================
timerWin32::timerWin32()
{
	serialRef = NULL;
	tMsec = 0;
	stateProcessing = false;
}

/**
 * Destructor: Do nothing
 *///=================================================
timerWin32::~timerWin32()
{

}

/**
 * Initialize: Setup the events and the timer thread
 *///======================================================
void timerWin32::initialize(serialConnectionUseRef* parRef)
{
	// Use different names whenever starting a timer object (names are critical)
	startEvent = CreateEvent(NULL, true, false, ((std::string)"START_EVENT" + int2String(timerCounter)).c_str());
	stopEvent = CreateEvent(NULL, true, false, ((std::string)"STOP_EVENT" + int2String(timerCounter)).c_str());
	timerCounter++;

	// Intialize the critical section for timer access
	InitializeCriticalSection(&sectionTimerAccess);
	threadTimer = CreateThread(NULL, 0, global_TimerThread, 
		(void*)this, 0, &threadID);
	serialRef = parRef;
}

/**
 * Thread to wait until the start event has been triggered
 *///=======================================================
DWORD
timerWin32::timerThread()
{
	while(1)
	{
		// Set mode to not in processing
		EnterCriticalSection(&sectionTimerAccess);
		stateProcessing = false;
		LeaveCriticalSection(&sectionTimerAccess);

		WaitForSingleObject(startEvent, INFINITE);

		// Start has been triggered, now wait for timeout
		ResetEvent(startEvent);
		if(WaitForSingleObject(stopEvent, tMsec) == WAIT_TIMEOUT)
		{
			// If timeout has been waited for, call the callback function
			this->timeoutProcess();
		}
		ResetEvent(stopEvent);
	}
	return(0);
}

/**
 * Only if not currently in processing state, set the timeout interval
 *///==================================================================
bool
timerWin32::setTimerLength(int msec)
{
	bool res = false;
	EnterCriticalSection(&sectionTimerAccess);

	// Set new values only if not currently in processing
	if(stateProcessing == false)
	{
		tMsec = msec;
		res = true;
	}
	LeaveCriticalSection(&sectionTimerAccess);
	return(res);
}

/**
 * Function to start the timer if not currently in processing
 *///==============================================================
bool
timerWin32::startTimerWin32()
{
	bool state = false;
	EnterCriticalSection(&sectionTimerAccess);
	state = stateProcessing;
	if(state == false)
	{
		stateProcessing = true;
		SetEvent(startEvent);
	}
	LeaveCriticalSection(&sectionTimerAccess);
	return(state == false);
}

/**
 * Stop timer before timer has expired. Avoid calling of callback
 * from outside
 *///=============================================================
bool
timerWin32::stopTimerWin32()
{
	bool state = false;
	EnterCriticalSection(&sectionTimerAccess);
	state = stateProcessing;
	LeaveCriticalSection(&sectionTimerAccess);
	if(state == true)
	{
		SetEvent(stopEvent);
		return(true);
	}
	return(false);
}

/**
 * Callback function used for timer reporting. This function is called whenever timer expires
 *///=========================================================================================
void
timerWin32::timeoutProcess()
{
	if(serialRef)
		serialRef->awakeTimer(this);
}
