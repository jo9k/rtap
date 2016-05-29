#ifndef __JVX_ALLOCATE_H__
#define __JVX_ALLOCATE_H__

#ifdef __cplusplus

#define JVX_DSP_SAFE_ALLOCATE_OBJECT(retPtr, dataType) retPtr = new dataType;
#define JVX_DSP_SAFE_ALLOCATE_FIELD(retPtr, dataType, numElements) retPtr = new dataType[numElements];
#define JVX_DSP_SAFE_DELETE_OBJECT(ptr) { if(ptr) delete(ptr); ptr = nullptr; }
#define JVX_DSP_SAFE_DELETE_FIELD(ptr) { if(ptr) delete[](ptr); ptr = nullptr; }
#define JVX_DSP_SAFE_DELETE_FIELD_TYPE(ptr, type) { if(ptr) delete[]((type*)ptr); ptr = nullptr; }

#else

#include <stdlib.h>
#include <string.h>

#define JVX_DSP_SAFE_ALLOCATE_OBJECT(retPtr, dataType) { retPtr = (dataType*)malloc(sizeof(dataType)); memset(retPtr, 0, sizeof(dataType)); }
#define JVX_DSP_SAFE_ALLOCATE_FIELD(retPtr, dataType, numElements) { retPtr = (dataType*)malloc(sizeof(dataType) * (numElements)); memset(retPtr, 0, sizeof(dataType)* (numElements)); }
#define JVX_DSP_SAFE_DELETE_OBJECT(ptr) { if(ptr) free(ptr); ptr = NULL; }
#define JVX_DSP_SAFE_DELETE_FIELD(ptr) { if(ptr) free(ptr); ptr = NULL; }
#define JVX_DSP_SAFE_DELETE_FIELD_TYPE(ptr, type) { if(ptr) free((type*)ptr); ptr = NULL; }

#endif

#endif
