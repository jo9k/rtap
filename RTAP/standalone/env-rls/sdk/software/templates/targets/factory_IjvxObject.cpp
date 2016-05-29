// Fragment of code to be included by algorithm
#include "jvx_dsp_base.h"

// Definition of used objects: either global OR multiple instances possible
#ifdef _JVX_ALLOW_MULTIPLE_INSTANCES
static std::vector<IjvxObject*> allObjects;
#else

#if defined(FEATURE_CLASS)

#if defined(COMPONENT_DESCRIPTOR)
static COMPONENT_TYPE theObject(COMPONENT_DESCRIPTION, false, JVX_MODULE_NAME, COMPONENT_DESCRIPTOR, FEATURE_CLASS);
#else
static COMPONENT_TYPE theObject(COMPONENT_DESCRIPTION, false, JVX_MODULE_NAME, JVX_MODULE_NAME, FEATURE_CLASS);
#endif

#else

#if defined(COMPONENT_DESCRIPTOR)
static COMPONENT_TYPE theObject(COMPONENT_DESCRIPTION, false, JVX_MODULE_NAME, COMPONENT_DESCRIPTOR, JVX_FEATURE_CLASS_NONE);
#else
static COMPONENT_TYPE theObject(COMPONENT_DESCRIPTION, false, JVX_MODULE_NAME, JVX_MODULE_NAME, JVX_FEATURE_CLASS_NONE);
#endif

#endif
static int refCount = 0;

#endif

// We may overwrite the name of the entry points
#ifndef JVX_OBJECT_INIT_FUNCTION
#define JVX_OBJECT_INIT_FUNCTION jvxInitObject
#endif

#ifndef JVX_OBJECT_TERMINATE_FUNCTION
#define JVX_OBJECT_TERMINATE_FUNCTION jvxTerminateObject
#endif

#ifndef JVX_OBJECT_DLL_PROPERTY_FUNCTION
#define JVX_OBJECT_DLL_PROPERTY_FUNCTION jvxDllProperties
#endif

#ifndef JVX_OBJECT_VERSION
#define JVX_OBJECT_VERSION jvxVersionString
#endif

#ifndef JVX_OBJECT_DLL_PROPERTY_BFIELD
#define JVX_OBJECT_DLL_PROPERTY_BFIELD 0
#endif

static const char* version_tag = JVX_VERSION_STRING;
// Entry functions
extern "C"
{
	//! Access functions for algorithm: Entry-point!
	jvxErrorType JVX_OBJECT_INIT_FUNCTION(IjvxObject** retObject)
	{
		if(retObject)
		{
#ifdef _JVX_ALLOW_MULTIPLE_INSTANCES

#if defined(FEATURE_CLASS)

#if defined( COMPONENT_DESCRIPTOR)
			COMPONENT_TYPE* newComponent = new COMPONENT_TYPE(COMPONENT_DESCRIPTION, true, JVX_MODULE_NAME, COMPONENT_DESCRIPTOR, FEATURE_CLASS);
#else
			COMPONENT_TYPE* newComponent = new COMPONENT_TYPE(COMPONENT_DESCRIPTION, true, JVX_MODULE_NAME, JVX_MODULE_NAME, FEATURE_CLASS);
#endif

#else

#if defined( COMPONENT_DESCRIPTOR)
			COMPONENT_TYPE* newComponent = new COMPONENT_TYPE(COMPONENT_DESCRIPTION, true, JVX_MODULE_NAME, COMPONENT_DESCRIPTOR, JVX_FEATURE_CLASS_NONE);
#else
			COMPONENT_TYPE* newComponent = new COMPONENT_TYPE(COMPONENT_DESCRIPTION, true, JVX_MODULE_NAME, JVX_MODULE_NAME, JVX_FEATURE_CLASS_NONE);
#endif

#endif
			IjvxObject* castedObject = static_cast<IjvxObject*>(newComponent);
			allObjects.push_back(castedObject);
			*retObject = castedObject;
#else
			*retObject = static_cast<IjvxObject*>(&theObject);
			refCount++;
#endif
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_INVALID_ARGUMENT);
	}

	//! Access function for algorithm: Exit-point!
	jvxErrorType JVX_OBJECT_TERMINATE_FUNCTION(IjvxObject* cls)
	{

#ifdef _JVX_ALLOW_MULTIPLE_INSTANCES
		std::vector<IjvxObject*>::iterator elmAlgo = allObjects.begin();
		for(unsigned int i = 0; i < allObjects.size();i++)
		{
			if((void*)cls == (void*)allObjects[i])
			{
				allObjects.erase(elmAlgo);
				delete(cls);
				return(JVX_NO_ERROR);
			}
			elmAlgo++;
		}
#else
		if(cls == static_cast<IjvxObject*>(&theObject))
		{
			refCount--;
			return(JVX_NO_ERROR);
		}
#endif

		return(JVX_ERROR_ELEMENT_NOT_FOUND);
	}

#ifdef JVX_SHARED_LIB
	// We may overwrite the name of the entry points
	jvxErrorType JVX_OBJECT_DLL_PROPERTY_FUNCTION(jvxBitField* btfield)
	{
		if(btfield)
		{
			*btfield = JVX_OBJECT_DLL_PROPERTY_BFIELD;
		}
		return(JVX_NO_ERROR);
	}
	
	const char* JVX_OBJECT_VERSION()
	{
		return(version_tag);
	}
#endif
}
