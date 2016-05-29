/* 
 *****************************************************
 * Filename: IrtpLogfile.h
 *****************************************************
 * Project: RTProc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description:                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __IRTPLOGFILE_H__
#define __IRTPLOGFILE_H__

JVX_INTERFACE IjvxLogToFile
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxLogToFile(){};


	virtual jvxErrorType JVX_CALLINGCONVENTION addEntry_blocking(const char* txt) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION addEntry_nonblocking(const char* txt) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION addEntry_nonblocking_strict(const char* txt) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION produceStackframe_blocking(const char* description) = 0;

};

#endif




