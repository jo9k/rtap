#ifndef __JVX_THREADS_H__
#define __JVX_THREADS_H__

#include "jvx.h"

typedef struct
{
	jvxErrorType (*callback_thread_startup)(jvxHandle* privateData_thread, jvxInt64 timestamp_us);
	jvxErrorType (*callback_thread_timer_expired)(jvxHandle* privateData_thread, jvxInt64 timestamp_us);
	jvxErrorType (*callback_thread_wokeup)(jvxHandle* privateData_thread, jvxInt64 timestamp_us);
	jvxErrorType (*callback_thread_stopped)(jvxHandle* privateData_thread, jvxInt64 timestamp_us);
} jvx_thread_handler;

jvxErrorType jvx_thread_initialize(jvxHandle** hdlOnReturn, jvx_thread_handler* report_callback, jvxHandle* privData, jvxBool report_timestamp, jvxBool noblocking = false, jvxBool asyncProcessing = false);

jvxErrorType jvx_thread_start(jvxHandle* hdlEnter, jvxInt64 timeout_ms);

// Functions to be executed while thread is active
jvxErrorType  jvx_thread_set_priority(jvxHandle* hdlEnter, JVX_THREAD_PRIORITY prio);
jvxErrorType jvx_thread_reconfigure(jvxHandle* hdlEnter, jvxInt64 timeout_ms);
jvxErrorType jvx_thread_wakeup(jvxHandle* hdlEnter, jvxBool trigger_even_if_busy = true);

jvxErrorType jvx_thread_test_complete(jvxHandle* hdlEnter, jvxBool* ready_for_next_trigger);

jvxErrorType jvx_thread_stop(jvxHandle* hdlEnter);

jvxErrorType jvx_thread_trigger_stop(jvxHandle* hdlEnter);
jvxErrorType jvx_thread_wait_stop(jvxHandle* hdlEnter, jvxInt64 timeout_ms);

jvxErrorType jvx_thread_terminate(jvxHandle* hdlLeave);

#endif
