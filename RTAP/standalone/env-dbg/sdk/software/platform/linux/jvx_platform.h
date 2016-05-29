/*
 *****************************************************
 * Filename: rtp_platform.h
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

/*
 *  platform.h
 *
 */

#ifndef __JVX_PLATFORM_H___LINUX__
#define __JVX_PLATFORM_H___LINUX__

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/poll.h>
#include <errno.h>
#include <dirent.h>
#include <cstring>
#include <string>
#include <dlfcn.h>
#include <pthread.h>
#include <vector>
#include <stdlib.h> // for getenv() under linux
#include <limits.h>
#include <float.h>
#include <inttypes.h>
#include <cmath>

#define OUTPUT_DEBUG

// Define the operating system
#define JVX_OS_LINUX
//#define OS_SUPPORTS_RS232
//#define OS_SUPPORTS_BTH

//==============================================================
// Datatype definitions
//==============================================================
#include "jvx_dsp_base_types__os.h"

#ifndef __LP64__
#define JVX_DECLARE_QT_METATYPES \
Q_DECLARE_METATYPE(jvxSize)\
Q_DECLARE_METATYPE(jvxInt16)\
Q_DECLARE_METATYPE(jvxUInt64)\
Q_DECLARE_METATYPE(jvxReportPriority)\
Q_DECLARE_METATYPE(jvxComponentType)\
Q_DECLARE_METATYPE(IjvxObject*)\
Q_DECLARE_METATYPE(jvxPropertyCategoryType)\
Q_DECLARE_METATYPE(bool)\
Q_DECLARE_METATYPE(jvxPropertyCallPurpose)
#else // __LP64__
#define JVX_DECLARE_QT_METATYPES \
Q_DECLARE_METATYPE(jvxInt16)\
Q_DECLARE_METATYPE(jvxUInt64)\
Q_DECLARE_METATYPE(jvxReportPriority)\
Q_DECLARE_METATYPE(jvxComponentType)\
Q_DECLARE_METATYPE(IjvxObject*)\
Q_DECLARE_METATYPE(jvxPropertyCategoryType)\
Q_DECLARE_METATYPE(bool)\
Q_DECLARE_METATYPE(jvxPropertyCallPurpose)
#endif // __LP64__
#define JVX_REGISTER_QT_METATYPES \
qRegisterMetaType<jvxSize>("jvxSize");\
qRegisterMetaType<jvxInt16>("jvxInt16");\
qRegisterMetaType<jvxUInt64>("jvxUInt64");\
qRegisterMetaType<jvxReportPriority>("jvxReportPriority");\
qRegisterMetaType<jvxComponentType>("jvxComponentType");\
qRegisterMetaType<IjvxObject*>("IjvxObject*");\
qRegisterMetaType<jvxPropertyCategoryType>("jvxPropertyCategoryType");\
qRegisterMetaType<bool>("bool");\
qRegisterMetaType<jvxPropertyCallPurpose>("jvxPropertyCallPurpose");


// Some additional datatypes (timeval)
#define JVX_TIME_USEC suseconds_t
#define JVX_TIME_SEC time_t

#ifdef __LP64__
#define JVX_PRINTF_CAST_INT64 "%ld"
#else
#define JVX_PRINTF_CAST_INT64 "%lld"
#endif


#define JVX_SYSTEM_DESCRIPTOR_PLATFORM JVX_SYSTEM_LINUX

// Define the interface type, in Mac, interface keyword is unknown
#define JVX_INTERFACE class

#define JVX_ISINF(value) (std::isinf(value) == 1)

#define JVX_ISNEGINF(value) (std::isinf(value) == -1)

#define JVX_INFINITE 1000000

#define byte char

#define JVX_DLL_EXPORT __attribute__((visibility("default")))

//! Define the overall calling conventions for all C++ functions!
#define JVX_CALLINGCONVENTION

#define JVX_HMODULE void*

#define JVX_INVALID_HANDLE_VALUE INVALID_HANDLE_VALUE

#define TCHAR char

#define MAX_PATH 1024

#define JVX_GETCURRENTDIRECTORY(a, b) getcwd(a, b)

#define JVX_SEPARATOR_DIR "/"

#define JVX_SEPARATOR_DIR_char '/'

#define JVX_COMPONENT_DIR "jvxComponents"

#define JVX_MIN(a, b) (a<b ? a:b)
#define JVX_MAX(a, b) (a>b ? a:b)

//===============================================
// Everything for network stuff
//===============================================

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cassert>
#define JVX_SOCKET int
#define JVX_CLOSE_SOCKET close
#define INVALID_SOCKET -1
#define JVX_INVALID_SOCKET_HANDLE -1
#define SOCKET_ERROR   -1
//#define JVX_SOCKET_LEN socklen_t <- setup in Windows platform.h
#define JVX_SOCKADDR sockaddr
#define JVX_SOCKADDR_IN sockaddr_in

#define JVX_POLL_SOCKET_STRUCT struct pollfd
#define JVX_POLL_SOCKET poll
#define JVX_POLL_SOCKET_RDNORM POLLIN
#define JVX_POLL_SOCKET_ERR POLLERR
#define JVX_POLL_SOCKET_HUP POLLHUP
#define JVX_POLL_SOCKET_NVAL POLLNVAL

#define JVX_SHUTDOWN_SOCKET shutdown

#define JVX_SOCKET_SD_RECEIVE 0
#define JVX_SOCKET_SD_SEND 1
#define JVX_SOCKET_SD_BOTH 2

#define JVX_SOCKET_REVENT short

#define JVX_SOCKET_POLL_INIT_DEFAULT POLLIN

static inline void JVX_INIT_LOCALE(const char* newL)
{
  char *oldLocale = setlocale(LC_NUMERIC, NULL);
  std::cout << "Old Locale:" << oldLocale << std::endl;
  setlocale(LC_NUMERIC, newL);
  oldLocale = setlocale(LC_NUMERIC, NULL);
  std::cout << "New Locale:" << oldLocale << std::endl;
}

#define JVX_THREAD_PRIORITY int
#define JVX_THREAD_PRIORITY_REALTIME -19
#define JVX_THREAD_PRIORITY_NORMAL 0
static inline void JVX_SET_THREAD_PRIORITY(pthread_t a, int b)
{
	int policy;
	sched_param parameters;
	pthread_getschedparam(a, &policy, &parameters);
	if (b == JVX_THREAD_PRIORITY_REALTIME)
	{
		parameters.sched_priority = sched_get_priority_max(policy);
	}
	else
	{
		parameters.sched_priority = (sched_get_priority_max(policy) + sched_get_priority_min(policy)) / 2;
	}

	pthread_setschedparam(a, policy, &parameters);
}

static inline JVX_THREAD_PRIORITY JVX_GET_THREAD_PRIORITY(pthread_t a)
{
	int policy;
	sched_param parameters;
	pthread_getschedparam(a, &policy, &parameters);

    return parameters.sched_priority;
}

static inline bool JVX_GETENVIRONMENTVARIABLE(char* var,char* bufRet,int ll)
{
	const char* varP = getenv(var);
	std::string str = "";
	memset(bufRet, 0, sizeof(char)*ll);
	if(varP)
	{
		str = varP;
//		for(int i = 0; i < ll; i++)
//		{
//			bufRet[i] = varP[i];
			//if(varP[i] == 0)
			//	break;
//		}
		memcpy(bufRet, str.c_str(), sizeof(char)*str.size());
		return(true);
	}
	return(false);
}

#define JVX_SETENVIRONMENTVARIABLE(a, b) setenv(a,b, 1)

#define JVX_LOADLIBRARYA(a) dlopen(a, RTLD_LAZY)

static inline void* JVX_LOADLIBRARYA_PATH(std::string fName, std::string dir)
{
	if((dir == ""))
	{
		return(dlopen(fName.c_str(), RTLD_LAZY));
	}
	return(dlopen((dir+ JVX_SEPARATOR_DIR +fName).c_str(), RTLD_LAZY));
}

#define JVX_LOADLIBRARY(a) dlopen(a, RTLD_LAZY)

static inline void* JVX_LOADLIBRARY_PATHVEC(std::string fName, std::vector<std::string> PathVec)
{
	for (size_t i = 0; i < PathVec.size(); i++) {
		if((PathVec[i] != ""))
		{
			std::string nmLib = PathVec[i]+ JVX_SEPARATOR_DIR +fName;
#ifdef OUTPUT_DEBUG
			std::cout << "Trying to open library " << nmLib << std::endl;
#endif
			JVX_HMODULE dll_loaded = dlopen((nmLib).c_str(), RTLD_LAZY);
			if ( dll_loaded )
			{
				return( dll_loaded );
			}
			else
			{
#ifdef OUTPUT_DEBUG
				std::cout << "Error trying to open so: " << dlerror() << std::endl;
#endif
			}
		}
	}
	return(dlopen(fName.c_str(), RTLD_LAZY));
}

static inline void* JVX_LOADLIBRARY_PATH(std::string fName, std::string dir)
{
	if((dir == ""))
	{
		return(dlopen(fName.c_str(), RTLD_LAZY));
	}
	return(dlopen((dir+ JVX_SEPARATOR_DIR +fName).c_str(), RTLD_LAZY));
}

#define JVX_GETPROCADDRESS(a, b) dlsym(a,b)

#define JVX_UNLOADLIBRARY(a) dlclose(a)

#define JVX_HANDLE_ void*

#define INVALID_HANDLE_VALUE NULL

#define JVX_MUTEX_HANDLE pthread_mutex_t

#define JVX_LOCK_MUTEX(a) pthread_mutex_lock(&a)

#define JVX_TRY_LOCK_MUTEX(a,b) a = pthread_mutex_trylock(&b)

#define JVX_TRY_LOCK_MUTEX_RESULT_TYPE int

#define JVX_TRY_LOCK_MUTEX_SUCCESS 0

// #define JVX_TRY_LOCK_MUTEX_NO_SUCCESS

#define JVX_UNLOCK_MUTEX(a) pthread_mutex_unlock(&a)

#define JVX_INITIALIZE_MUTEX(a) { pthread_mutexattr_t obj; pthread_mutexattr_init(&obj); pthread_mutexattr_settype(&obj, PTHREAD_MUTEX_RECURSIVE); pthread_mutex_init(&a, &obj); pthread_mutexattr_destroy(&obj);}

#define JVX_TERMINATE_MUTEX(a) pthread_mutex_destroy(&a)

typedef struct
{
	pthread_cond_t cond;
	pthread_mutex_t mutex;
} JVX_NOTIFY_HANDLE;

static inline int JVX_INITIALIZE_NOTIFICATION(JVX_NOTIFY_HANDLE& hdl)
{
	pthread_mutexattr_t obj;
	pthread_mutexattr_init(&obj);
	pthread_mutexattr_settype(&obj, PTHREAD_MUTEX_RECURSIVE);
	int res1 = pthread_mutex_init(&hdl.mutex, &obj);
	pthread_mutexattr_destroy(&obj);
	int res2 = pthread_cond_init(&hdl.cond, NULL);
	return(res1 && res2);
}

static inline int JVX_TERMINATE_NOTIFICATION(JVX_NOTIFY_HANDLE& hdl)
{
	int res1 = pthread_cond_destroy(&hdl.cond);
	int res2 = pthread_mutex_destroy(&hdl.mutex);
	return(res1 && res2);
}

#define JVX_TRY_LOCK_RW_MUTEX_RESULT_TYPE int

#define JVX_TRY_LOCK_RW_MUTEX_SUCCESS 0

#define JVX_RW_MUTEX_HANDLE pthread_rwlock_t

#define JVX_INITIALIZE_RW_MUTEX(a) {pthread_rwlockattr_t obj; pthread_rwlockattr_init(&obj); /*pthread_rwlockattr_settype(&obj, PTHREAD_RWLOCK_RECURSIVE); */pthread_rwlock_init(&a, &obj);pthread_rwlockattr_destroy(&obj);}

#define JVX_TERMINATE_RW_MUTEX(a) pthread_rwlock_destroy(&a)
//TerminateSRWLock(&a)

#define JVX_LOCK_RW_MUTEX_EXCLUSIVE(a) pthread_rwlock_wrlock(&a)

#define JVX_LOCK_RW_MUTEX_SHARED(a) pthread_rwlock_rdlock(&a)

#define JVX_UNLOCK_RW_MUTEX_EXCLUSIVE(a) pthread_rwlock_unlock(&a)

#define JVX_UNLOCK_RW_MUTEX_SHARED(a) pthread_rwlock_unlock(&a)

#define JVX_TRY_LOCK_RW_MUTEX_EXCLUSIVE(a,b) a = pthread_rwlock_trywrlock(&b)

#define JVX_TRY_LOCK_RW_MUTEX_SHARED(a,b) a = pthread_rwlock_tryrdlock(&b)

// ==============================================================
// Some Atomic Operations to - maybe - replace critical sections
// From MSDN: LONG __cdecl InterlockedCompareExchange(  _Inout_  LONG volatile *Destination, _In_     LONG Exchange, _In_     LONG Comparand);
// The function returns the initial value of the Destination parameter.
// ==============================================================

#define JVX_ATOMIC_MUTEX_HANDLE_TYPE volatile LONG

#define JVX_ATOMIC_MUTEX_HANDLE_CREATE(obj) obj = 0

#define JVX_ATOMIC_MUTEX_HANDLE_TRYENTER(obj) InterlockedCompareExchange((LONG*)&obj, 1, 0)

#define JVX_ATOMIC_MUTEX_HANDLE_ENTERSUCCESS 0

#define JVX_ATOMIC_MUTEX_HANDLE_TRYLEAVE(obj) InterlockedCompareExchange((LONG*)&obj, 0, 1)

#define JVX_ATOMIC_MUTEX_HANDLE_LEAVESUCCESS 1

#define JVX_ATOMIC_MUTEX_HANDLE_DELETE(obj)

#define JVX_INTERLOCKED_COMPARE_EXCHANGE_16(dest, exch, comp) __sync_val_compare_and_swap(dest, comp, exch)
// type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)

#define JVX_INTERLOCKED_EXCHANGE_16(target, value) __sync_lock_test_and_set(target, value)
// type __sync_lock_test_and_set (type *ptr, type value, ...)
// ==============================================================



#define JVX_THREAD_HANDLE pthread_t

#define JVX_NULLTHREAD ((pthread_t)0)

#define JVX_THREAD_ID pthread_t

#define JVX_RETURN_THREAD(a) return((void*)((long long)a))

#define JVX_GET_CURRENT_THREAD_ID pthread_self

#define JVX_GET_CURRENT_THREAD_HANDLE pthread_self

#define JVX_THREAD_RETURN_TYPE void*

#define JVX_THREAD_ENTRY_FUNCTION(a,b) static JVX_THREAD_RETURN_TYPE a(void* b)

#define JVX_CREATE_THREAD(a, b, c, d) pthread_create(&a,NULL,b, c)

#define JVX_TERMINATE_THREAD(a, b) pthread_cancel(a);

#define JVX_WAIT_FOR_THREAD_TERMINATE_MS(a,b) pthread_join(a, NULL)

#define JVX_COMPARE_THREADS(a,b) (pthread_equal(a,b) != 0)

typedef struct
{
  timespec start;
} jvxTimeStampData;

static inline void JVX_GET_TICKCOUNT_US_SETREF(jvxTimeStampData& refData)
{
  int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &refData.start);
  assert(res == 0);
}

static inline jvxInt64 JVX_GET_TICKCOUNT_US_GET(jvxTimeStampData& refData)
{
	jvxInt64 time_us;
	//timeval tp;
	int deltaSec = 0;
	int deltaNSec = 0;

	timespec tpNew;

	tpNew.tv_sec = 0;
	tpNew.tv_nsec = 0;

	int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &tpNew);
	assert(res == 0);

	deltaSec = tpNew.tv_sec - refData.start.tv_sec;
	deltaNSec = tpNew.tv_nsec - refData.start.tv_nsec;

	time_us  = (jvxInt64) deltaSec * 1000000 + deltaNSec/1000;
	return(time_us);
}

/*
static inline jvxUInt64 JVX_GET_TICKCOUNT_US()
{
	jvxUInt64 time_us;
	//timeval tp;
	timespec tp;

	//gettimeofday(&tp, NULL);
	assert(clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID* /, &tp) == 0);

	//time_us  = tp.tv_sec * 1000000 + tp.tv_usec;
	time_us  = (jvxUInt64) (jvxData)tp.tv_sec * 1000000 + tp.tv_nsec/1000;
	return( time_us );
}
*/

static inline int JVX_WAIT_FOR_NOTIFICATION_S(JVX_NOTIFY_HANDLE& hdl, int timeoutSec)
{
	timespec tt;
	//timeval tp;

	int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &tt);
	//int res =  gettimeofday(&tp, NULL);

	/* Convert from timeval to timespec */
	//tt.tv_sec  = tp.tv_sec;
	//tt.tv_nsec = tp.tv_usec * 1000;
	tt.tv_sec += timeoutSec;


	pthread_mutex_lock(&hdl.mutex);
	res = pthread_cond_timedwait(&hdl.cond, &hdl.mutex, &tt);
	pthread_mutex_unlock(&hdl.mutex);
	return(res);
}

static inline int JVX_WAIT_FOR_NOTIFICATION_MS(JVX_NOTIFY_HANDLE& hdl, int timeoutmsec)
{
	timespec tt;
	//timeval tp;

	int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &tt);
	//int res =  gettimeofday(&tp, NULL);

	jvxInt64 sec = (jvxInt64)tt.tv_sec;
	jvxInt64 nsec = (jvxInt64)tt.tv_nsec + (jvxInt64)1000000 * (jvxInt64)timeoutmsec;

	sec += nsec/1000000000;
	nsec = nsec%1000000000;

	tt.tv_sec  = sec;
	tt.tv_nsec = nsec;


	pthread_mutex_lock(&hdl.mutex);
	res = pthread_cond_timedwait(&hdl.cond, &hdl.mutex, &tt);
	pthread_mutex_unlock(&hdl.mutex);
	return(res);
}

#define JVX_WAIT_FOR_NOTIFICATION_I(hdl) pthread_mutex_lock(&hdl.mutex)

static inline int JVX_WAIT_FOR_NOTIFICATION_II_S(JVX_NOTIFY_HANDLE& hdl, int timeoutSec)
{
	timespec tt;
	//timeval tp;

	int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &tt);
	//int res =  gettimeofday(&tp, NULL);

	/* Convert from timeval to timespec */
	//tt.tv_sec  = tp.tv_sec;
	//tt.tv_nsec = tp.tv_usec * 1000;
	tt.tv_sec += timeoutSec;


	res = pthread_cond_timedwait(&hdl.cond, &hdl.mutex, &tt);
	pthread_mutex_unlock(&hdl.mutex);
	return(res);
}

static inline int JVX_WAIT_FOR_NOTIFICATION_II_MS(JVX_NOTIFY_HANDLE& hdl, int timeoutmsec)
{
	timespec tt;
	timeval tp;

	int res = clock_gettime(CLOCK_REALTIME/*CLOCK_MONOTONIC*//*CLOCK_PROCESS_CPUTIME_ID*/, &tt);

	jvxInt64 sec = (jvxInt64)tt.tv_sec;
	jvxInt64 nsec = (jvxInt64)tt.tv_nsec + (jvxInt64)1000000 * (jvxInt64)timeoutmsec;

	sec += nsec/1000000000;
	nsec = nsec%1000000000;

	/* Convert from timeval to timespec */
	tt.tv_sec  = sec;
	tt.tv_nsec = nsec;

	res = pthread_cond_timedwait(&hdl.cond, &hdl.mutex, &tt);
	pthread_mutex_unlock(&hdl.mutex);
	return(res);
}

#define JVX_WAIT_SUCCESS 0

#define JVX_WAIT_RESULT int

/*
static uint64_t JVX_GET_TICKCOUNT64()
{
    uint64_t value = 0;
    struct timeval tt;
    gettimeofday(&tt,NULL);

    value = tt.tv_sec* 1000;
    value += (uint64_t)(0.001*(jvxData)tt.tv_usec);
    return(value);
}

static uint32_t JVX_GET_TICKCOUNT32()
{
    uint64_t val = JVX_GET_TICKCOUNT64();
    val = val & 0xFFFFFFFF;
    return((uint32_t)val);
}
*/

static void JVX_SLEEP_MS(unsigned int ms)
{
    usleep(ms*1000);
}

static void JVX_SLEEP_S(unsigned int s)
{
    sleep(s);
}

static inline int JVX_SET_NOTIFICATION(JVX_NOTIFY_HANDLE& hdl)
{
	pthread_mutex_lock(&hdl.mutex);
	pthread_cond_signal(&hdl.cond);
	pthread_mutex_unlock(&hdl.mutex);
	return(0);
}

#define JVX_RESET_NOTIFICATION(a)

/*
#define JVX_CRITICAL_SECTION pthread_mutex_t

#define JVX_INITIALIZE_CRITICAL_SECTION(var) pthread_mutex_init(var, NULL)

#define JVX_ENTER_CRITICAL_SECTION(var) pthread_mutex_lock(var)

#define JVX_TRYENTER_CRITICAL_SECTION(var) pthread_mutex_trylock(var)

#define JVX_LEAVE_CRITICAL_SECTION(var) pthread_mutex_unlock(var)

#define JVX_DESTROY_CRITICAL_SECTION(var) pthread_mutex_destroy(var)

#define JVX_NOTIFY_HANDLE pthread_cond_t

#define JVX_NOTIFY_CREATE(var) pthread_cond_init(&var, NULL)

#define JVX_NOTIFY_RESET(var)

#define JVX_WAITFOR_EVENT(evt, timeout) pthread_cond_timedwait(&evt, timeout)

#define JVX_THREAD_CREATE(hdl, entry, arg) pthread_create(&hdl,NULL,entry, arg)

#define JVX_THREAD_HANDLE pthread_t

#define THREAD_ENTRY(name, pParam) void* name(void* pParam)

#define JVX_THREAD_ID int

#define JVX_WAIT_ID int

#define WAIT_OBJECT_0 0
*/
#define JVX_HANDLE_DIR DIR*

#define JVX_DIR_FIND_DATA struct dirent*

#define INIT_SEARCH_DIR_DEFAULT NULL

static inline JVX_HANDLE_DIR
JVX_FINDFIRSTFILE_WC(JVX_HANDLE_DIR dirH, const char* dirName, const char* fNameExt, JVX_DIR_FIND_DATA& fData)
{
	//std::cout << "Dir: " << dirName << std::endl;
	dirH = opendir (dirName);
	fData = NULL;
	if(dirH)
	{
		while(true)
		{
			fData = readdir (dirH);
			if(fData)
			{
				std::string str = fData->d_name;

				//std::cout << str << std::endl;
				size_t posStr = str.rfind(".");
				if(posStr != std::string::npos)
				{
					std::string ext = str.substr(posStr, std::string::npos);
					if(ext == fNameExt)
					{
						break;
					}
				}
			}
			else
			{
				return NULL;
			}
		}
	}
	return(dirH);
}

static inline JVX_HANDLE_DIR
JVX_FINDFIRSTFILE_S(const char* dirName, JVX_DIR_FIND_DATA& fData)
{
	//std::cout << "Dir: " << dirName << std::endl;
        JVX_HANDLE_DIR dirH = opendir (dirName);
	fData = NULL;
	if(dirH)
	{
		while(true)
		{
			fData = readdir (dirH);
			if(!fData)
			{
				return NULL;
			}
		}
	}
	return(dirH);
}

static inline bool
JVX_FINDNEXTFILE(DIR*& dirH, JVX_DIR_FIND_DATA& fData, const char* fNameExt)
{
	fData = NULL;

	bool found = false;
	while(!found)
	{
		if(dirH)
		{
			fData = readdir (dirH);
			if(fData)
			{
				std::string str = fData->d_name;

//				std::cout << str << std::endl;
				size_t posStr = str.rfind(".");
				std::string ext = str.substr(posStr, std::string::npos);
				if(ext == fNameExt)
				{
					found = true;
					break;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return found;
}

#define JVX_GETFILENAMEDIR(a) searchResult->d_name

#define JVX_FINDCLOSE( a) closedir(a)

#define JVX_CHDIR(a) chdir(a)

#include <stdio.h>
#include <assert.h>

#define JVX_FOPEN(fHdl, fName, howtoopen) fopen_s(&fHdl, fName, howtoopen)
#define JVX_FCLOSE(fHdl) fclose(fHdl)

static inline void fopen_s(FILE **f, const char *name, const char *mode) {
    assert(f);
    *f = fopen(name, mode);
}

static inline std::string
JVX_DISPLAY_LAST_ERROR_LOAD(std::string fName)
{
	std::string errMess = strerror (errno);
	return errMess;
}

#define JVX_DLL_EXTENSION ".so"

#define JVX_MEX_EXTENSION ".mexmaci64"

#define JVX_DYN_LIB_PATH_VARIABLE "LD_LIBRARY_PATH"

#define JVX_PASTE_ENV_VARIABLE_ATT(a, b, c) (std::string)a + SEPARATOR_DIR + c + ":" + b;

#define JVX_PASTE_ENV_VARIABLE_NEW(a,b) (std::string)a + JVX_SEPARATOR_DIR + b;

#define JVX_LOAD_DLL_DIR ((std::string)JVX_SEPARATOR_DIR "loadDLLs")

static char cptime[50];
static inline std::string JVX_DATE()
{
	// At August 14 2014, this function should output Aug_14_2014
	time_t now = time(NULL);
	strftime(cptime, 50, "%b/%d/%Y", localtime(&now)); //uses short month name
	std::string strTime = cptime;
	return strTime;
}

static inline std::string JVX_TIME()
{
	// At 03:21:59, this function should return 03_21_59
	time_t now = time(NULL);
	strftime(cptime, 50, "%T", localtime(&now));
	std::string strTime = cptime;
	return strTime;
}

// Expression for "INFINITE" in maths
#define JVX_MATH_INFINITE INFINITY
//std::numeric_limits<jvxData>::infinity()

// Expression for "NOT A NUMBER" in maths
#define JVX_MATH_NAN NAN
//std::numeric_limits<jvxData>::quiet_NaN()

#define JVX_GUID void

#define JVX_FSEEK fseeko64

#define JVX_FTELL ftello64

// Compile for MATLAB version MATLAB 2007 A plus
// 100: 2007A
// 200: 2008A
// 250: 2008b
// 300: 2009A
// 350: 2009B
// 400: 2010A
// 450: 2010B
// 500: 2011A
#define _MATLAB_MEXVERSION 500

#endif
