#ifndef __IJVXHOST_H__
#define __IJVXHOST_H__

#include "jvx.h"

JVX_INTERFACE IjvxHost: public IjvxObject, public IjvxHiddenInterface, public IjvxStateMachine
{
	
public:

	virtual JVX_CALLINGCONVENTION ~IjvxHost(){};
	
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
	// Module configuration	
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_report_target(IjvxReport* hdl) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_report_on_config(IjvxReportOnConfig* callbackStruct) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_external_component(IjvxObject*, const char* locationDescription, jvxBool allowMultipleInstance = false, 
		jvxInitObject_tp funcInit = NULL, jvxTerminateObject_tp funcTerm = NULL) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION remove_external_component(IjvxObject* theObj) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_component_load_blacklist(jvxComponentType theTp) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_component_load_blacklist(jvxComponentType theTp) = 0;

	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
	// Control main sub components
	// --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  -- 
	virtual jvxErrorType JVX_CALLINGCONVENTION number_components(jvxComponentType, jvxSize* num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_component(jvxComponentType, jvxSize idx, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_component(jvxComponentType, jvxSize idx, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION module_name_component(jvxComponentType, jvxSize idx, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION feature_class_component(jvxComponentType, jvxSize idx, jvxBitField*) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION select_component(jvxComponentType, jvxSize) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION selection_component(jvxComponentType, jvxInt16*) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION activate_component(jvxComponentType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready_component(jvxComponentType, jvxBool* ready, jvxString** reasonIfNot) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate_component(jvxComponentType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect_component(jvxComponentType) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION state_component(jvxComponentType, jvxState*) = 0;

		
	virtual jvxErrorType JVX_CALLINGCONVENTION description_component(jvxComponentType, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION descriptor_component(jvxComponentType, jvxString**) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION feature_class_component(jvxComponentType, jvxBitField*) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION request_object_component(jvxComponentType, IjvxObject** ) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_object_component(jvxComponentType, IjvxObject* ) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION request_hidden_interface_component(jvxComponentType, jvxInterfaceType tp, jvxHandle** iface) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION return_hidden_interface_component(jvxComponentType, jvxInterfaceType tp, jvxHandle* iface) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION set_external_widget_host(jvxHandle* theWidgetHost, jvxWidgetHostType widgetType) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION clear_configuration_entries() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION set_configuration_entry(const char* entryname, jvxHandle* fld, jvxConfigSectionTypes tp, jvxInt32 id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration_entry(const char* entryname, jvxHandle** fld, jvxConfigSectionTypes* tp, jvxSize id) = 0;
};

#endif
