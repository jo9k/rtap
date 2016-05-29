#ifndef __CJVXAUDIOTECHNOLOGY_H__
#define __CJVXAUDIOTECHNOLOGY_H__

#include "jvx.h"

#include "templates/common/CjvxProperties.h"
#include "templates/common/CjvxTechnology.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

class CjvxAudioTechnology: public IjvxTechnology, public CjvxTechnology,
	public IjvxProperties, public CjvxProperties
{
protected:


public:

	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	// Component class interface member functions
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	JVX_CALLINGCONVENTION CjvxAudioTechnology(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxAudioTechnology();
	
	// ======================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	// ======================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION request_device(jvxSize idx, IjvxDevice** dev){return(CjvxTechnology::_request_device(idx, dev));};

	virtual jvxErrorType JVX_CALLINGCONVENTION return_device(IjvxDevice* dev){return(CjvxTechnology::_return_device(dev));};

#include "codeFragments/simplify/jvxProperties_simplify.h"

#include "codeFragments/simplify/jvxNoStateHigherThanActive_simplify.h"

#define JVX_RESET_PROPERTIES_ON_TERMINATE
#include "codeFragments/simplify/jvxStateMachine_simplify.h"
#undef JVX_RESET_PROPERTIES_ON_TERMINATE

#include "codeFragments/simplify/jvxTechnologies_simplify.h"

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxHiddenInterfaces_simplify.h"
};

#ifdef JVX_PROJECT_NAMESPACE
}
#endif

#endif