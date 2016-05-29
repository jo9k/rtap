#ifndef __TJVXACCESS_H__
#define __TJVXACCESS_H__

// Entry functions in DLl objects
typedef jvxErrorType JVX_CALLINGCONVENTION (*jvxInitObject_tp)(IjvxObject**);
typedef jvxErrorType JVX_CALLINGCONVENTION (*jvxTerminateObject_tp)(IjvxObject*);
typedef jvxErrorType JVX_CALLINGCONVENTION (*jvxDllProperties_tp)(jvxBitField*);

#define JVX_OPEN_OBJECT_DLL "jvxInitObject"
#define JVX_CLOSE_OBJECT_DLL "jvxTerminateObject"
#define JVX_PROPERTIES_DLL "jvxDllProperties"


#endif
