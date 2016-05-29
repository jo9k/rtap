/******************************************************
 * Filename: memory.h								  *
 * Description: Functionality to use the external heap*
 *			 	memory.								  *
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/

 #ifndef _MEMORY_H__
#define _MEMORY_H__

#include <heapnew>

// Operator new on external memory
#define NEW2 new(heapID)

// Operator new on internal memory
#define NEW1 new

// Operator malloc on external memory
#define MALLOC2(a) heap_malloc(heapID, a)

// Operator malloc on internal memory
#define MALLOC1(a) malloc(a)

// Operator calloc on external memory
#define CALLOC2(a) heap_calloc(heapID, a)

// Operator calloc on internal memory
#define CALLOC1(a) calloc(a)

// Macro to make memory allocation thread safe, should be used since RS232 is already running
// very early
#define MEMORY_THREADSAFE_START asm("#include <def21369.h>");asm("bit clr mode1 IRPTEN;");

// Macro to make memory allocation thread safe, should be used since RS232 is already running
// very early
#define MEMORY_THREADSAFE_STOP  asm("bit set mode1 IRPTEN;");

// Variable to hold the heap ID
extern int heapID;

// Function to setup heap and to get the heap ID
void initializeExtHeap();

#endif
