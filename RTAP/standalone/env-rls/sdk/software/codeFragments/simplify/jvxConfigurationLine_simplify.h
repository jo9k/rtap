virtual jvxErrorType JVX_CALLINGCONVENTION number_lines(jvxSize* num_lines )
{
	return(_number_lines(num_lines ));
};

virtual jvxErrorType JVX_CALLINGCONVENTION add_line(jvxComponentType tp_master, const char* description)
{
	return(_add_line(tp_master, description));
};

virtual jvxErrorType JVX_CALLINGCONVENTION add_slave_line(jvxSize idxLine, jvxComponentType tp_slave)
{
	return(_add_slave_line(idxLine, tp_slave));
};

virtual jvxErrorType JVX_CALLINGCONVENTION remove_slave_line(jvxSize idxLine, jvxComponentType tp_slave) 
{
	return(_remove_slave_line(idxLine, tp_slave));
};

virtual jvxErrorType JVX_CALLINGCONVENTION remove_line(jvxSize idxLine)
{
	return(_remove_line(idxLine));
};

virtual jvxErrorType JVX_CALLINGCONVENTION description_line(jvxSize idxLine, jvxSize* num_slaves)
{
	return(_description_line(idxLine, num_slaves));
};

	virtual jvxErrorType JVX_CALLINGCONVENTION number_lines(jvxSize* num_lines ) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_line(jvxComponentType tp_master, const char* description) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION add_slave_line(jvxSize idxLine, jvxComponentType tp_slave) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION add_configuration_property_line(jvxSize idxLine, jvxSize system_property_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_slave_line(jvxSize idxLine, jvxComponentType tp_slave) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_line(jvxSize idxLine, jvxSize* num_slaves) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION add_configuration_property_line(jvxSize idxLine, jvxSize system_property_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_configuration_property_line(jvxSize idxLine, jvxSize system_property_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION description_property_configuration_line(jvxSize idxLine, jvxSize idxEntry, jvxSize* the_prop_id) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION remove_line(jvxSize idxLine) = 0;