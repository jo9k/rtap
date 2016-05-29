#ifndef __CJVXAPPLICATIONCONTROLLERTECHNOLOGY_H__
#define __CJVXAPPLICATIONCONTROLLERTECHNOLOGY_H__

#include "jvx.h"

#include "templates/common/CjvxProperties.h"
#include "templates/common/CjvxTechnology.h"

class CjvxApplicationControllerTechnology: public IjvxTechnology, public CjvxTechnology, public CjvxProperties
{
protected:


public:

	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	// Component class interface member functions
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	JVX_CALLINGCONVENTION CjvxApplicationControllerTechnology(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxApplicationControllerTechnology();
	
	// ======================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();

	// ======================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION request_device(jvxSize idx, IjvxDevice** dev){return(CjvxTechnology::_request_device(idx, dev));};

	virtual jvxErrorType JVX_CALLINGCONVENTION return_device(IjvxDevice* dev){return(CjvxTechnology::_return_device(dev));};


#include "codeFragments/simplify/jvxNoStateHigherThanActive_simplify.h"

#include "codeFragments/simplify/jvxStateMachine_simplify.h"

#include "codeFragments/simplify/jvxTechnologies_simplify.h"

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxProperties_simplify.h"

#include "codeFragments/simplify/jvxHiddenInterfaces_simplify.h"
};

#endif