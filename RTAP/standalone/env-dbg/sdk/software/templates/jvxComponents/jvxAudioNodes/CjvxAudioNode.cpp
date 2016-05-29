#include "jvx.h"
#include "templates/jvxComponents/jvxAudioNodes/CjvxAudioNode.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

CjvxAudioNode::CjvxAudioNode(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass):
	CjvxObject(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_AUDIO_NODE;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxNode*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);
	_common_spec_proc_set.thisisme = static_cast<IjvxObject*>(this);

	_common_set_audio_params.acceptOnlyExactMatchLinkDataInput = false;
	_common_set_audio_params.acceptOnlyExactMatchLinkDataOutput = false;

	_common_set_audio_params.preferred.buffersize.min = -1;
	_common_set_audio_params.preferred.buffersize.max = -1;
	_common_set_audio_params.preferred.number_input_channels.min = -1;
	_common_set_audio_params.preferred.number_input_channels.max = -1;
	_common_set_audio_params.preferred.number_output_channels.min = -1;
	_common_set_audio_params.preferred.number_output_channels.max = -1;
	_common_set_audio_params.preferred.samplerate.min = -1;
	_common_set_audio_params.preferred.samplerate.max = -1;
	_common_set_audio_params.preferred.format.min = JVX_DATAFORMAT_NONE;
	_common_set_audio_params.preferred.format.max = JVX_DATAFORMAT_NONE;

	_common_set_audio_params.processing.buffersize = -1;
	_common_set_audio_params.processing.number_input_channels = -1;
	_common_set_audio_params.processing.number_output_channels = -1;
	_common_set_audio_params.processing.samplerate = -1;
	_common_set_audio_params.processing.format = JVX_DATAFORMAT_NONE;

}

CjvxAudioNode::~CjvxAudioNode()
{
}

jvxErrorType
CjvxAudioNode::activate()
{
	jvxErrorType res = _activate();
	if(res == JVX_NO_ERROR)
	{
		CjvxAudioNode_genpcg::init__properties_parameters();
		CjvxAudioNode_genpcg::allocate__properties_parameters();
		CjvxAudioNode_genpcg::register__properties_parameters(static_cast<CjvxProperties*>(this));

		CjvxAudioNode_genpcg::init__properties_active_higher();
		CjvxAudioNode_genpcg::allocate__properties_active_higher();
		CjvxAudioNode_genpcg::register__properties_active_higher(static_cast<CjvxProperties*>(this));

	}
	return(res);
};

jvxErrorType
CjvxAudioNode::deactivate()
{
	jvxErrorType res = _deactivate();
	if(res == JVX_NO_ERROR)
	{
		CjvxAudioNode_genpcg::unregister__properties_active_higher(static_cast<CjvxProperties*>(this));
		CjvxAudioNode_genpcg::deallocate__properties_active_higher();

		CjvxAudioNode_genpcg::unregister__properties_parameters(static_cast<CjvxProperties*>(this));
		CjvxAudioNode_genpcg::deallocate__properties_parameters();
	}
	return(res);
};

jvxErrorType
CjvxAudioNode::prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
{
	jvxErrorType res = _prepare( theEndpoint, numEndpoints);
	if(res == JVX_NO_ERROR)
	{
		_common_set_audio_params.processing.buffersize = CjvxAudioNode_genpcg::properties_parameters.buffersize.value;
		_common_set_audio_params.processing.samplerate = CjvxAudioNode_genpcg::properties_parameters.samplerate.value;
		_common_set_audio_params.processing.number_input_channels = CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value;
		_common_set_audio_params.processing.number_output_channels = CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value;
		_common_set_audio_params.processing.format = (jvxDataFormat)CjvxAudioNode_genpcg::properties_parameters.format.value;
	}
	return(res);
}

jvxErrorType
CjvxAudioNode::reportPreferredParameters(jvxPropertyCategoryType cat, jvxInt16 propId)
{
	jvxInt32 tmp32;
	jvxInt16 tmp16;
	jvxSize tmpSz;
	jvxDataFormat format;
	jvxErrorType resL = JVX_NO_ERROR;
	jvxBool requiresCorrection = false;

	/* Use preferred buffersize if possible */
	if(
		(CjvxAudioNode_genpcg::properties_parameters.buffersize.cat == cat) &&
		(CjvxAudioNode_genpcg::properties_parameters.buffersize.id == propId))
	{
		requiresCorrection = false;

		tmp32 = -1;
		if(_common_set_audio_params.preferred.buffersize.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.buffersize.value > _common_set_audio_params.preferred.buffersize.max)
			{
				tmp32 = _common_set_audio_params.preferred.buffersize.max;
				requiresCorrection = true;
			}
		}
		if(_common_set_audio_params.preferred.buffersize.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.buffersize.value < _common_set_audio_params.preferred.buffersize.min)
			{
				tmp32 = _common_set_audio_params.preferred.buffersize.min;
				requiresCorrection = true;
			}
		}

		if(requiresCorrection)
		{
			// indirect set via technology - if possible
			resL = this->_report_take_over_property(&tmp32, 1, JVX_DATAFORMAT_32BIT_LE, 0, true,
				CjvxAudioNode_genpcg::properties_parameters.buffersize.cat,
				CjvxAudioNode_genpcg::properties_parameters.buffersize.id,
				JVX_COMPONENT_AUDIO_DEVICE);
		}
	}

	/* Use preferred samplerate if possible */
	if(
		(CjvxAudioNode_genpcg::properties_parameters.samplerate.cat == cat) &&
		(CjvxAudioNode_genpcg::properties_parameters.samplerate.id == propId))
	{
		requiresCorrection = false;

		tmp32 = -1;
		if(_common_set_audio_params.preferred.samplerate.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.samplerate.value > _common_set_audio_params.preferred.samplerate.max)
			{
				tmp32 = _common_set_audio_params.preferred.samplerate.max;
				requiresCorrection = true;
			}
		}
		if(_common_set_audio_params.preferred.samplerate.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.samplerate.value < _common_set_audio_params.preferred.samplerate.min)
			{
				tmp32 = _common_set_audio_params.preferred.samplerate.min;
				requiresCorrection = true;
			}
		}

		if(requiresCorrection)
		{
			// indirect set via technology - if possible
			resL = this->_report_take_over_property(&tmp32, 1, JVX_DATAFORMAT_32BIT_LE, 0, true,
				CjvxAudioNode_genpcg::properties_parameters.samplerate.cat,
				CjvxAudioNode_genpcg::properties_parameters.samplerate.id,
				JVX_COMPONENT_AUDIO_DEVICE);
		}
	}

	/* Use preferred number input channels if possible */
	if(
		(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.cat == cat) &&
		(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.id == propId))
	{
		requiresCorrection = false;

		tmp32 = -1;
		if(_common_set_audio_params.preferred.number_input_channels.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value > _common_set_audio_params.preferred.number_input_channels.max)
			{
				tmp32 = _common_set_audio_params.preferred.number_input_channels.max;
				requiresCorrection = true;
			}
		}
		if(_common_set_audio_params.preferred.number_input_channels.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value < _common_set_audio_params.preferred.number_input_channels.min)
			{
				tmp32 = _common_set_audio_params.preferred.number_input_channels.min;
				requiresCorrection = true;
			}
		}


		if(requiresCorrection)
		{
			// indirect set via technology - if possible
			resL = this->_report_take_over_property(&tmp32, 1, JVX_DATAFORMAT_32BIT_LE, 0, true,
				CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.cat,
				CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.id,
				JVX_COMPONENT_AUDIO_DEVICE);
		}
	}

	/* Use preferred number input channels if possible */
	if(
		(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.cat == cat) &&
		(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.id == propId))
	{
		requiresCorrection = false;

		tmp32 = -1;
		if(_common_set_audio_params.preferred.number_output_channels.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value > _common_set_audio_params.preferred.number_output_channels.max)
			{
				tmp32 = _common_set_audio_params.preferred.number_output_channels.max;
				requiresCorrection = true;
			}
		}
		if(_common_set_audio_params.preferred.number_output_channels.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value < _common_set_audio_params.preferred.number_output_channels.min)
			{
				tmp32 = _common_set_audio_params.preferred.number_output_channels.min;
				requiresCorrection = true;
			}
		}


		if(requiresCorrection)
		{
			// indirect set via technology - if possible
			resL = this->_report_take_over_property(&tmp32, 1, JVX_DATAFORMAT_32BIT_LE, 0, true,
				CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.cat,
				CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.id,
				JVX_COMPONENT_AUDIO_DEVICE);
		}
	}



	if(
		(CjvxAudioNode_genpcg::properties_parameters.format.cat == cat) &&
		(CjvxAudioNode_genpcg::properties_parameters.format.id == propId))
	{
		requiresCorrection = false;

		tmp16 = JVX_DATAFORMAT_NONE;
		if(_common_set_audio_params.preferred.format.max > JVX_DATAFORMAT_NONE)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.format.value > _common_set_audio_params.preferred.format.max)
			{
				tmp16 = _common_set_audio_params.preferred.format.max;
				requiresCorrection = true;
			}
		}
		if(_common_set_audio_params.preferred.format.min > JVX_DATAFORMAT_NONE)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.format.value < _common_set_audio_params.preferred.format.min)
			{
				tmp16 = _common_set_audio_params.preferred.format.min;
				requiresCorrection = true;
			}
		}

		if(requiresCorrection)
		{
			// indirect set via technology - if possible
			resL = this->_report_take_over_property(&tmp16, 1, JVX_DATAFORMAT_16BIT_LE, 0, true,
				CjvxAudioNode_genpcg::properties_parameters.format.cat,
				CjvxAudioNode_genpcg::properties_parameters.format.id,
				JVX_COMPONENT_AUDIO_DEVICE);
		}
	}
	return(resL);
}

jvxErrorType
CjvxAudioNode::is_ready(jvxBool* suc, jvxString** reasonIfNot)
{
	std::string txt;
	jvxErrorType res = _is_ready( suc, reasonIfNot);
	jvxBool Iamready = true;
	jvxBool isAProblem = false;

	if(res == JVX_NO_ERROR)
	{
		/*
		 * CHECK BUFFERSIZE
		 */
		isAProblem = false;
		if(_common_set_audio_params.preferred.buffersize.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.buffersize.value < _common_set_audio_params.preferred.buffersize.min)
			{
				isAProblem = true;
			}
		}
		if(_common_set_audio_params.preferred.buffersize.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.buffersize.value > _common_set_audio_params.preferred.buffersize.max)
			{
				isAProblem = true;
			}
		}
		if(isAProblem)
		{
			Iamready  = false;
			txt = "Buffersize mismatch, constraint: ";
			txt += jvx_int2String(_common_set_audio_params.preferred.buffersize.min);
			txt += "<=";
			txt += jvx_int2String(CjvxAudioNode_genpcg::properties_parameters.buffersize.value);
			txt+= "<=";
			txt += jvx_int2String(_common_set_audio_params.preferred.buffersize.max);
		}

		/*
		 * CHECK SAMPLERATE
		 */
		isAProblem = false;
		if(_common_set_audio_params.preferred.samplerate.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.samplerate.value < _common_set_audio_params.preferred.samplerate.min)
			{
				isAProblem = true;
			}
		}
		if(_common_set_audio_params.preferred.samplerate.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.samplerate.value > _common_set_audio_params.preferred.samplerate.max)
			{
				isAProblem = true;
			}
		}
		if(isAProblem)
		{
			Iamready  = false;
			txt = "Samplerate mismatch, constraint: ";
			txt += jvx_int2String(_common_set_audio_params.preferred.samplerate.min);
			txt += "<=";
			txt += jvx_int2String(CjvxAudioNode_genpcg::properties_parameters.samplerate.value);
			txt+= "<=";
			txt += jvx_int2String(_common_set_audio_params.preferred.samplerate.max);
		}

		/*
		 * CHECK NUMBER INPUT CHANNELS
		 */
		isAProblem = false;
		if(_common_set_audio_params.preferred.number_input_channels.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value < _common_set_audio_params.preferred.number_input_channels.min)
			{
				isAProblem = true;
			}
		}
		if(_common_set_audio_params.preferred.number_input_channels.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value > _common_set_audio_params.preferred.number_input_channels.max)
			{
				isAProblem = true;
			}
		}
		if(isAProblem)
		{
			Iamready  = false;
			txt = "Input Channel mismatch, constraint: ";
			txt += jvx_int2String(_common_set_audio_params.preferred.number_input_channels.min);
			txt += "<=";
			txt += jvx_int2String(CjvxAudioNode_genpcg::properties_parameters.numberinputchannels.value);
			txt+= "<=";
			txt += jvx_int2String(_common_set_audio_params.preferred.number_input_channels.max);
		}


		/*
		 * CHECK NUMBER OUTPUT CHANNELS
		 */
		isAProblem = false;

		if(_common_set_audio_params.preferred.number_output_channels.min >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value < _common_set_audio_params.preferred.number_output_channels.min)
			{
				isAProblem = true;
			}
		}
		if(_common_set_audio_params.preferred.number_output_channels.max >= 0)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value > _common_set_audio_params.preferred.number_output_channels.max)
			{
				isAProblem = true;
			}
		}


		if(isAProblem)
		{
			Iamready  = false;
			txt = "Output Channel mismatch, constraint: ";
			txt += jvx_int2String(_common_set_audio_params.preferred.number_output_channels.min);
			txt += "<=";
			txt += jvx_int2String(CjvxAudioNode_genpcg::properties_parameters.numberoutputchannels.value);
			txt+= "<=";
			txt += jvx_int2String(_common_set_audio_params.preferred.number_output_channels.max);
		}

		/*
		 * CHECK PROCESSING FORMAT
		 */
		isAProblem = false;

		if(_common_set_audio_params.preferred.format.min > JVX_DATAFORMAT_NONE)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.format.value < _common_set_audio_params.preferred.format.min)
			{
				isAProblem = true;
			}
		}
		if(_common_set_audio_params.preferred.format.max > JVX_DATAFORMAT_NONE)
		{
			if(CjvxAudioNode_genpcg::properties_parameters.format.value > _common_set_audio_params.preferred.format.max)
			{
				isAProblem = true;
			}
		}


		if(isAProblem)
		{
			Iamready  = false;
			txt = "Processing Format mismatch, constraint: ";
			txt += jvxDataFormat_txt(_common_set_audio_params.preferred.format.min);
			txt += "<=";
			txt += jvxDataFormat_txt(CjvxAudioNode_genpcg::properties_parameters.format.value);
			txt+= "<=";
			txt += jvxDataFormat_txt(_common_set_audio_params.preferred.format.max);
		}


		if(!txt.empty())
		{
			HjvxObject_produceString(reasonIfNot, txt);
		}
		if(suc)
		{
			*suc = Iamready;
		}
	}

	return(res);
};

jvxErrorType
CjvxAudioNode::start()
{
	return(_start());
};

jvxErrorType
CjvxAudioNode::stop()
{
	return(_stop());
};

jvxErrorType
CjvxAudioNode::postprocess()
{
	return(_postprocess());
};

jvxErrorType
CjvxAudioNode::reference_object(IjvxObject** refObject)
{
	return(_reference_object(refObject));
};

jvxErrorType
CjvxAudioNode::act_as_component(jvxComponentType* iActAsComponent)
{
	return(_act_as_component(iActAsComponent));
};

jvxErrorType
CjvxAudioNode::prepare_sender_to_receiver(jvxLinkDataDescriptor* theData)
{
	jvxErrorType res = JVX_ERROR_WRONG_STATE;
	if(_common_set.theState == JVX_STATE_PREPARED)
	{
		if(_common_set_audio_params.acceptOnlyExactMatchLinkDataInput)
		{
			// Setup has been set in the properties - let us make sure that it really matches
			if(
				!(
				(_common_set_audio_params.processing.buffersize == theData->sender_to_receiver.buffersize) &&
				(_common_set_audio_params.processing.number_input_channels == theData->sender_to_receiver.number_channels) &&
				(_common_set_audio_params.processing.samplerate == theData->sender_to_receiver.rate) &&
				(_common_set_audio_params.processing.format == theData->sender_to_receiver.format)))
			{
				res = JVX_ERROR_INVALID_SETTING;
			}
			else
			{
				// Allocate memory
				res = _init_data_link(theData, true, true);
			}
		}
		else
		{
			res = _init_data_link(theData, true, true);
		}
	}
	return(res);
};

jvxErrorType
CjvxAudioNode::prepare_complete_receiver_to_sender(jvxLinkDataDescriptor* theData)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_PREPARED)
	{
		if(_common_set_audio_params.acceptOnlyExactMatchLinkDataOutput)
		{
			if(
				!(
				(_common_set_audio_params.processing.buffersize == theData->receiver_to_sender.buffersize) &&
				(_common_set_audio_params.processing.number_output_channels == theData->receiver_to_sender.number_channels) &&
				(_common_set_audio_params.processing.samplerate == theData->receiver_to_sender.rate) &&
				(_common_set_audio_params.processing.format == theData->receiver_to_sender.format)))
			{
				res = JVX_ERROR_INVALID_SETTING;
			}
			else
			{
				res = JVX_NO_ERROR;
			}
		}
		else
		{
			res = JVX_NO_ERROR;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
};

jvxErrorType
CjvxAudioNode::supports_multithreading(jvxBool* supports)
{
	return(_supports_multithreading(supports));
};

/* TODO: Add member functions for multithreading negotiations */

jvxErrorType
CjvxAudioNode::process_mt(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender, jvxSize* channelSelect , jvxSize numEntriesChannels, jvxInt32 offset_input, jvxInt32 offset_output, jvxInt32 numEntries)
{
	return(_process_mt(theData, idx_sender_to_receiver, idx_receiver_to_sender, channelSelect, numEntriesChannels, offset_input, offset_output, numEntries));
};

jvxErrorType
CjvxAudioNode::reconfigured_receiver_to_sender(jvxLinkDataDescriptor* theData)
{
	return(JVX_NO_ERROR);
};

jvxErrorType
CjvxAudioNode::process_st(jvxLinkDataDescriptor* theData, jvxSize idx_sender_to_receiver, jvxSize idx_receiver_to_sender)
{
	return(_process_st(theData, idx_sender_to_receiver, idx_receiver_to_sender));
};

jvxErrorType
CjvxAudioNode::last_error_process(char* fld_text, jvxSize fldSize, jvxErrorType* err, jvxInt32* id_error, jvxLinkDataDescriptor* theData)
{
	return(_last_error_process(fld_text, fldSize, err, id_error, theData));
};

jvxErrorType
CjvxAudioNode::postprocess_sender_to_receiver(jvxLinkDataDescriptor* theData)
{
	jvxErrorType res = JVX_ERROR_WRONG_STATE;
	if(_common_set.theState == JVX_STATE_PREPARED)
	{
		// Deallocate memory
		res = _release_data_link(theData, true, true);
		_common_set_audio_params.processing.format = JVX_DATAFORMAT_NONE;
		_common_set_audio_params.processing.number_input_channels = -1;
		_common_set_audio_params.processing.buffersize = -1;
		_common_set_audio_params.processing.samplerate = -1;
	}
	return(res);
};

jvxErrorType
CjvxAudioNode::before_postprocess_receiver_to_sender(jvxLinkDataDescriptor* theData)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_PREPARED)
	{
		/*
		_common_set_audio_params.processing.format = theData->receiver_to_sender.format;
		_common_set_audio_params.processing.number_output_channels = theData->receiver_to_sender.number_channels;
		_common_set_audio_params.processing.buffersize = theData->receiver_to_sender.buffersize;
		_common_set_audio_params.processing.samplerate = theData->receiver_to_sender.rate;
		*/
		_common_set_audio_params.processing.number_output_channels = -1;
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}

	return(res);
};

jvxErrorType
CjvxAudioNode::put_configuration(IjvxConfigProcessor* processor,
				 jvxHandle* sectionToContainAllSubsectionsForMe,
				 const char* filename,
				 jvxInt32 lineno)
{
     return JVX_NO_ERROR;
}

jvxErrorType
CjvxAudioNode::get_configuration(IjvxConfigProcessor* processor,
				 jvxHandle* sectionWhereToAddAllSubsections)
{
     return JVX_NO_ERROR;
}

#ifdef JVX_PROJECT_NAMESPACE
}
#endif
