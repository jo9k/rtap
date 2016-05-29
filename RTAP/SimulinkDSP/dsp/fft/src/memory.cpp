/******************************************************
 * Filename: memory.c								  *
 * Description: All functionality required to access  *
 *				the secondary heap (located in SDRAM).*
 * copyright IND, RWTH Aachen, 2008, HK				  *
 ******************************************************/
#include "userDef.h"
#include <stdlib.h>

// USERID must be greater than 0
#define USERID 1

// heapID returned by system function install_heap
// this value is required for any allocation/deallocation
// of memory
int RAM_DEFAULT_SECTION heapID;

asm(".extern ldf_heaq_space;");
asm(".extern ldf_heaq_end;");

/*==================================================
 * Initalization of secondary heap. The start address comes from the
 * ldf file. Note that somehow access to the symbols is only possible in
 * assembler. After the heap is allocated, memory can be allocated
 * from SDRAM which is slow but very big.
 *///===============================================
 void initializeExtHeap()
{
	int heaq_start;
	int heaq_end;
	int heapLength;

	// Allocate space for second heap
	asm(
		"%0 = ldf_heaq_space;": 
		"=d" (heaq_start));
	asm(
		"%0 = ldf_heaq_end;": 
		"=d" (heaq_end));
	heapLength = heaq_end-heaq_start;
	
	// If heapID is -1, an error has occurred
	heapID = heap_install((void *)heaq_start, heaq_end-heaq_start, USERID, -1);
}
