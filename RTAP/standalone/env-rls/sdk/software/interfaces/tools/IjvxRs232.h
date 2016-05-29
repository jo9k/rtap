#ifndef __IJVXRS232_H__
#define __IJVXRS232_H__

JVX_INTERFACE IjvxRs232_report
{
public:
	~IjvxRs232_report(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION provide_data_and_length(jvxByte**, jvxSize*) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION report_data_and_read(jvxByte*, jvxSize) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION report_event(jvxUInt32 eventMask) = 0;

};

JVX_INTERFACE IjvxRs232: public IjvxObject
{
public:
	virtual ~IjvxRs232(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION initialize() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION availablePorts(jvxStringList** allPorts) = 0;

	// =============================================================
	// Port specific commands
	// =============================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION state_port(jvxSize idPort, jvxState* theState) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION start_port(jvxSize idPort, jvxRs232Config* cfg, IjvxRs232_report* theReport) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION sendMessage_port(jvxSize idPort, jvxByte* fld, jvxSize* szFld) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION stop_port(jvxSize idPort) = 0;

	// =============================================================

	virtual jvxErrorType JVX_CALLINGCONVENTION terminate() = 0;
};

#endif