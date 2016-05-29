#ifndef __IJVXCONFIGURATIONLINE_H__
#define __IJVXCONFIGURATIONLINE_H__

/*
JVX_INTERFACE IjvxConfigurationLine_report
{
	virtual JVX_CALLINGCONVENTION ~IjvxConfigurationLine_report(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION report_configuration_changed(IjvxObject* sender) = 0;
};
*/

JVX_INTERFACE IjvxConfigurationLine
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxConfigurationLine(){};


	virtual jvxErrorType JVX_CALLINGCONVENTION number_lines(jvxSize* num_lines ) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_line(jvxComponentType tp_master, const char* description) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_line(jvxSize idxLine, jvxComponentType* tp_master, jvxSize* num_slaves, jvxSize* num_system_props) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION add_slave_line(jvxSize idxLine, jvxComponentType tp_slave) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_slave_line(jvxSize idxLine, jvxComponentType tp_slave) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_slave_line(jvxSize idxLine, jvxSize idxEntry, jvxComponentType* the_prop_id) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION add_configuration_property_line(jvxSize idxLine, jvxSize system_property_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_configuration_property_line(jvxSize idxLine, jvxSize system_property_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_property_configuration_line(jvxSize idxLine, jvxSize idxEntry, jvxSize* the_prop_id) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION report_property_was_set(jvxComponentType tpFrom, jvxPropertyCategoryType cat, jvxSize propIdx, jvxBool onlyContent, jvxComponentType tpTo) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION refresh_all() = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION remove_line(jvxSize idxLine) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_all_lines() = 0;


};

#endif