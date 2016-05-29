#ifndef IJVXREPORT_H__
#define IJVXREPORT_H__

JVX_INTERFACE IjvxReport
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxReport(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION report_simple_text_message(const char* txt, jvxReportPriority prio) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION report_internals_have_changed(jvxComponentType thisismytype, IjvxObject* thisisme,
		jvxPropertyCategoryType cat, jvxInt16 propId, bool onlyContent, jvxComponentType tpTo,
		jvxPropertyCallPurpose callpurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION report_take_over_property(jvxComponentType thisismytype, IjvxObject* thisisme,
		jvxHandle* fld, jvxSize numElements, jvxDataFormat format, jvxSize offsetStart, jvxBool onlyContent,
		jvxPropertyCategoryType cat, jvxInt16 propId, jvxComponentType tpTo, jvxPropertyCallPurpose callpurpose = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION report_command_request(jvxCommandRequestType request) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION report_os_specific(jvxSize commandId, void* context) = 0;

};

#endif
