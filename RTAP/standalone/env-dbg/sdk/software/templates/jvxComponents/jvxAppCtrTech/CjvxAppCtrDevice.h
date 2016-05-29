#ifndef __CJVXAPPLICATIONCONTROLLERDEVICE_H__
#define __CJVXAPPLICATIONCONTROLLERDEVICE_H__

#include "jvx.h"
#include "templates/common/CjvxObject.h"
#include "templates/common/CjvxDeallocate.h"
#include "templates/common/CjvxProperties.h"
#include "templates/common/CjvxSequencerControl.h"

#include "pcg_CjvxAppCtrDevice_pcg.h"

class CjvxApplicationControllerDevice: public IjvxDevice, public CjvxObject, public CjvxDeallocate, public IjvxProperties, public CjvxProperties, public IjvxSequencerControl, 
	public CjvxSequencerControl, public IjvxConfiguration, public CjvxApplicationControllerDevice_genpcg
{
protected:

	struct
	{
		bool is_connected;
	} _common_set_external_controller;

public:

	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	// Component class interface member functions
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	JVX_CALLINGCONVENTION CjvxApplicationControllerDevice(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass);

	virtual JVX_CALLINGCONVENTION ~CjvxApplicationControllerDevice();
	
	virtual jvxErrorType JVX_CALLINGCONVENTION activate();

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot){return(_is_ready(suc, reasonIfNot));};

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints){return(_prepare( theEndpoint, numEndpoints));};

	virtual jvxErrorType JVX_CALLINGCONVENTION start(){return(_start());};

	virtual jvxErrorType JVX_CALLINGCONVENTION stop(){return(_stop());};

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess(){return(_postprocess());};

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate();


	virtual jvxErrorType JVX_CALLINGCONVENTION  put_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionToContainAllSubsectionsForMe, const char* filename, jvxInt32 lineno ){return(JVX_ERROR_UNSUPPORTED);};

	virtual jvxErrorType JVX_CALLINGCONVENTION  get_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionWhereToAddAllSubsections){return(JVX_ERROR_UNSUPPORTED);};


#include "codeFragments/simplify/jvxStateMachine_simplify.h"

#include "codeFragments/simplify/jvxObjects_simplify.h"

#include "codeFragments/simplify/jvxDeallocate_simplify.h"

#include "codeFragments/simplify/jvxProperties_simplify.h"

#include "codeFragments/simplify/jvxSequencerControl_simplify.h"

#define JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
#define JVX_INTERFACE_SUPPORT_PROPERTIES
#define JVX_INTERFACE_SUPPORT_CONFIGURATION
#include "codeFragments/simplify/jvxHiddenInterface_simplify.h"
#undef JVX_INTERFACE_SUPPORT_SEQUENCER_CONTROL
#undef JVX_INTERFACE_SUPPORT_PROPERTIES
#undef JVX_INTERFACE_SUPPORT_CONFIGURATION
};

#endif