#ifndef __IJVXSEQUENCERCONTROL_H__
#define __IJVXSEQUENCERCONTROL_H__

JVX_INTERFACE IjvxSequencerControl
{
public:

	virtual JVX_CALLINGCONVENTION ~IjvxSequencerControl(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION number_conditions(jvxSize* num) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION description_condition(jvxSize idx, jvxSize* unqiueId, jvxString** description, jvxUInt64* stateValid) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION status_condition(jvxSize uniqueId, jvxBool* fulfilled) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION number_commands(jvxSize* num) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION description_command(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_command(jvxSize uniqueId) = 0;


	virtual jvxErrorType JVX_CALLINGCONVENTION number_relative_jumps(jvxSize* num) = 0;
	
	virtual jvxErrorType JVX_CALLINGCONVENTION description_relative_jump(jvxSize idx, jvxSize* uniqueId, jvxString** description, jvxUInt64* stateValid) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION trigger_relative_jump(jvxSize uniqueId, jvxSize* pc_increment) = 0;
};

#endif