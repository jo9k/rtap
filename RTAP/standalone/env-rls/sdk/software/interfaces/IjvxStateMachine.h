#ifndef __IJVXSTATEMACHINE_H__
#define __IJVXSTATEMACHINE_H__

JVX_INTERFACE IjvxDataProcessor;

JVX_INTERFACE IjvxStateMachine
{

public:

	virtual JVX_CALLINGCONVENTION ~IjvxStateMachine(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION initialize(IjvxHiddenInterface* hostRef) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION select() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION activate() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION is_ready(jvxBool* suc, jvxString** reasonIfNot) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION start() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION stop() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION postprocess() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION deactivate() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION unselect() = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION terminate() = 0;
};

#endif
