#include "templates/jvxComponents/jvxAudioTechnologies/CjvxAudioDevice.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

CjvxAudioDevice::CjvxAudioDevice(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
	CjvxObject(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_AUDIO_DEVICE;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxDevice*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);
	this->_common_set_audio_device.referene_audio_node = NULL;
}

CjvxAudioDevice::~CjvxAudioDevice()
{
	this->terminate();
}

jvxErrorType
CjvxAudioDevice::activate()
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;
	res = _activate();
	if(res == JVX_NO_ERROR)
	{
		init__properties_active();
		allocate__properties_active();

/*
		associate__properties_active(static_cast<CjvxProperties*>(this),
			&_common_set_audio_device.inputchannels, &_common_set_audio_device.outputchannels);
*/
		register__properties_active(static_cast<CjvxProperties*>(this));

		//_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY, JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.format.id);

		for(i = 0; i < _common_set_audio_device.formats.size(); i++)
		{
			CjvxAudioDevice::properties_active.formatselection.value.entries.push_back(jvxDataFormat_txt(_common_set_audio_device.formats[i]));
		}
		if(	_common_set_audio_device.formats.size() > 0)
		{
			CjvxAudioDevice::properties_active.format.value = (jvxInt16) _common_set_audio_device.formats[0];
			CjvxAudioDevice::properties_active.formatselection.value.selection = ((jvxBitField)1 << 0);
			CjvxAudioDevice::properties_active.formatselection.value.exclusive = (jvxBitField) -1;
		}
		else
		{
			CjvxAudioDevice::properties_active.format.value = (jvxInt16) JVX_DATAFORMAT_NONE;
		}

		this->updateDependentVariables_lock(-1, JVX_PROPERTY_CATEGORY_PREDEFINED, true);
	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::activate_lock()
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;
	res = _activate_lock();
	if(res == JVX_NO_ERROR)
	{
		init__properties_active();
		allocate__properties_active();
		register__properties_active(static_cast<CjvxProperties*>(this));

		//_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY, JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.format.id);

		for(i = 0; i < _common_set_audio_device.formats.size(); i++)
		{
			CjvxAudioDevice::properties_active.formatselection.value.entries.push_back(jvxDataFormat_txt(_common_set_audio_device.formats[i]));
		}
		if(	_common_set_audio_device.formats.size() > 0)
		{
			CjvxAudioDevice::properties_active.format.value = (jvxInt16) _common_set_audio_device.formats[0];
			CjvxAudioDevice::properties_active.formatselection.value.selection = ((jvxBitField)1 << 0);
			CjvxAudioDevice::properties_active.formatselection.value.exclusive = (jvxBitField) -1;
		}
		else
		{
			CjvxAudioDevice::properties_active.format.value = (jvxInt16) JVX_DATAFORMAT_NONE;
		}
		this->updateDependentVariables_lock(-1, JVX_PROPERTY_CATEGORY_PREDEFINED, true );
	}
	return(res);
}


jvxErrorType
CjvxAudioDevice::deactivate()
{
	jvxErrorType res = JVX_NO_ERROR;
	res = _deactivate();
	if(res == JVX_NO_ERROR)
	{

		unregister__properties_active(static_cast<CjvxProperties*>(this));
		//deassociate__properties_active(static_cast<CjvxProperties*>(this));

		deallocate__properties_active();

		CjvxAudioDevice::properties_active.inputchannelselection.value.entries.clear();
		CjvxAudioDevice::properties_active.inputchannelselection.value.selection = 0;
		CjvxAudioDevice::properties_active.inputchannelselection.value.exclusive = 0;

		CjvxAudioDevice::properties_active.outputchannelselection.value.entries.clear();
		CjvxAudioDevice::properties_active.outputchannelselection.value.selection = 0;
		CjvxAudioDevice::properties_active.outputchannelselection.value.exclusive = 0;

		CjvxAudioDevice::properties_active.buffersize.value = 0;
		CjvxAudioDevice::properties_active.format.value = (jvxInt16)JVX_DATAFORMAT_NONE;

		CjvxAudioDevice::properties_active.formatselection.value.entries.clear();
		CjvxAudioDevice::properties_active.formatselection.value.selection = 0;
		CjvxAudioDevice::properties_active.formatselection.value.exclusive = 0;
		CjvxAudioDevice::properties_active.format.value = (jvxInt16) JVX_DATAFORMAT_NONE;

		CjvxAudioDevice::properties_active.numberinputchannels.value = 0;
		CjvxAudioDevice::properties_active.numberoutputchannels.value = 0;

		CjvxAudioDevice::properties_active.samplerate.value = 0;

	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::deactivate_lock()
{
	jvxErrorType res = JVX_NO_ERROR;
	res = _deactivate_lock();
	if(res == JVX_NO_ERROR)
	{

		unregister__properties_active(static_cast<CjvxProperties*>(this));
		//deassociate__properties_active(static_cast<CjvxProperties*>(this));

		deallocate__properties_active();

		CjvxAudioDevice::properties_active.inputchannelselection.value.entries.clear();
		CjvxAudioDevice::properties_active.inputchannelselection.value.selection = 0;
		CjvxAudioDevice::properties_active.inputchannelselection.value.exclusive = 0;

		CjvxAudioDevice::properties_active.outputchannelselection.value.entries.clear();
		CjvxAudioDevice::properties_active.outputchannelselection.value.selection = 0;
		CjvxAudioDevice::properties_active.outputchannelselection.value.exclusive = 0;

		CjvxAudioDevice::properties_active.buffersize.value = 0;
		CjvxAudioDevice::properties_active.format.value = (jvxInt16)JVX_DATAFORMAT_NONE;

		CjvxAudioDevice::properties_active.formatselection.value.entries.clear();
		CjvxAudioDevice::properties_active.formatselection.value.selection = 0;
		CjvxAudioDevice::properties_active.formatselection.value.exclusive = 0;
		CjvxAudioDevice::properties_active.format.value = (jvxInt16) JVX_DATAFORMAT_NONE;

		CjvxAudioDevice::properties_active.numberinputchannels.value = 0;
		CjvxAudioDevice::properties_active.numberoutputchannels.value = 0;

		CjvxAudioDevice::properties_active.samplerate.value = 0;
	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::set_property(jvxHandle* fld, jvxSize numElements, jvxDataFormat format, jvxSize propId, jvxPropertyCategoryType category, jvxSize offsetStart, jvxBool contentOnly, jvxPropertyCallPurpose callPurpose)
{
	jvxBool report_update = false;
	jvxSize i;
	jvxErrorType res = _set_property(fld, numElements, format, propId, category, offsetStart, contentOnly, callPurpose,_common_set.thisisme);
	if(res == JVX_NO_ERROR)
	{
		this->updateDependentVariables_lock(propId, category, false, callPurpose);
	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::put_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionToContainAllSubsectionsForMe, const char* filename, jvxInt32 lineno )
{
	jvxErrorType res = JVX_NO_ERROR;
	std::vector<std::string> warnings;

	if(this->_common_set.theState == JVX_STATE_ACTIVE)
	{
		// Read all parameters from this class
		CjvxAudioDevice::put_configuration__properties_active(processor, sectionToContainAllSubsectionsForMe, warnings);
		if(!warnings.empty())
		{
			jvxSize i;
			std::string txt;
			for(i = 0; i < warnings.size(); i++)
			{
				txt = "Failed to read property " + warnings[i];
				_report_text_message(txt.c_str(), JVX_REPORT_PRIORITY_WARNING);
			}


			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		// Update all dependant props
		this->updateDependentVariables_lock(-1, JVX_PROPERTY_CATEGORY_PREDEFINED, true);

	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::get_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionWhereToAddAllSubsections)
{

	// Write all parameters from this class
	CjvxAudioDevice::get_configuration__properties_active(processor, sectionWhereToAddAllSubsections);

	return(JVX_NO_ERROR);
}

void
CjvxAudioDevice::updateDependentVariables_lock(jvxSize propId, jvxPropertyCategoryType category, jvxBool updateAll, jvxPropertyCallPurpose callPurpose)
{
	jvxSize i;
	jvxPropertyCategoryType catTp;
	jvxInt32 newValue = 0;

	if(category == JVX_PROPERTY_CATEGORY_PREDEFINED)
	{
		if(
			(propId == CjvxAudioDevice::properties_active.inputchannelselection.id) ||
			(propId == CjvxAudioDevice::properties_active.numberinputchannels.id) ||
			updateAll)
		{
			_lock_properties();
			for(i = 0; i < (int)CjvxAudioDevice::properties_active.inputchannelselection.value.entries.size(); i++)
			{
				if(CjvxAudioDevice::properties_active.inputchannelselection.value.selection & ((jvxUInt64)1 << i))
				{
					newValue++;
				}
			}
			CjvxAudioDevice::properties_active.numberinputchannels.value = newValue;

			_unlock_properties();
			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.inputchannelselection.id,
				true, JVX_COMPONENT_UNKNOWN, callPurpose);

			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.numberinputchannels.id,
				true, JVX_COMPONENT_UNKNOWN, callPurpose);
		}

		// =================================================================================================

		if(
			(propId == CjvxAudioDevice::properties_active.outputchannelselection.id) ||
			(propId == CjvxAudioDevice::properties_active.numberoutputchannels.id) ||
			updateAll)
		{
			_lock_properties();
			newValue = 0;
			for(i = 0; i < (int)CjvxAudioDevice::properties_active.outputchannelselection.value.entries.size(); i++)
			{
				if(CjvxAudioDevice::properties_active.outputchannelselection.value.selection & ((jvxUInt64)1 << i))
				{
					newValue++;
				}
			}
			CjvxAudioDevice::properties_active.numberoutputchannels.value = newValue;
			_unlock_properties();

			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED,
				CjvxAudioDevice::properties_active.outputchannelselection.id,
				true, JVX_COMPONENT_UNKNOWN, callPurpose);

			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED,
				CjvxAudioDevice::properties_active.numberoutputchannels.id,
				true, JVX_COMPONENT_UNKNOWN, callPurpose);

		}

		// =================================================================================================

		jvxBool formatFromSelectionList = false;
		if(
			(propId == CjvxAudioDevice::properties_active.format.id) && (!updateAll))
		{
			_lock_properties();

			std::string selStr = jvxDataFormat_txt(CjvxAudioDevice::properties_active.format.value);
			jvxInt32 idxSel = -1;

			for(i = 0; i < CjvxAudioDevice::properties_active.formatselection.value.entries.size(); i++)
			{
				if(CjvxAudioDevice::properties_active.formatselection.value.entries[i] == selStr)
				{
					idxSel = (jvxInt32)i;
					break;
				}
			}

			if(idxSel >= 0)
			{
				CjvxAudioDevice::properties_active.formatselection.value.selection = (jvxBitField)1 << idxSel;
			}
			else
			{
				formatFromSelectionList = true;
			}

			_unlock_properties();
			if(!formatFromSelectionList)
			{
				this->_report_property_has_changed(
					JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.formatselection.id, true,
					JVX_COMPONENT_UNKNOWN, callPurpose);
				this->_report_property_has_changed(
					JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.format.id, true,
					JVX_COMPONENT_UNKNOWN, callPurpose);
			}
		}

		if(
			(propId == CjvxAudioDevice::properties_active.formatselection.id) ||
			updateAll || formatFromSelectionList)
		{
			_lock_properties();
			std::string selStr = jvxDataFormat_txt(JVX_DATAFORMAT_NONE);
			for(i = 0; i < CjvxAudioDevice::properties_active.formatselection.value.entries.size(); i++)
			{
				if(i < _common_set_audio_device.formats.size())
				{
					if(CjvxAudioDevice::properties_active.formatselection.value.selection & ((jvxUInt64)1 << i))
					{
						selStr = CjvxAudioDevice::properties_active.formatselection.value.entries[i];
						break;
					}
				}
			}
			CjvxAudioDevice::properties_active.format.value = jvxDataFormat_decode(selStr.c_str());
			_unlock_properties();
			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.formatselection.id, true,
				JVX_COMPONENT_UNKNOWN, callPurpose);
			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.format.id, true,
				JVX_COMPONENT_UNKNOWN, callPurpose);
		}

		if(
			(propId == CjvxAudioDevice::properties_active.buffersize.id) || updateAll)
		{
			// inform all other devices that property has changed
			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.buffersize.id, true, JVX_COMPONENT_UNKNOWN,
				callPurpose);
		}

		if(
			(propId == CjvxAudioDevice::properties_active.samplerate.id) || updateAll)
		{
			// inform all other devices that property has changed
			this->_report_property_has_changed(
				JVX_PROPERTY_CATEGORY_PREDEFINED, CjvxAudioDevice::properties_active.samplerate.id, true, JVX_COMPONENT_UNKNOWN,
				callPurpose);
		}
	}
}

jvxErrorType
CjvxAudioDevice::prepare(IjvxDataProcessor** theEndpoint, jvxSize numEndpoints)
{
    jvxSize i;
	jvxErrorType res = _prepare( theEndpoint, numEndpoints);
	if(res == JVX_NO_ERROR)
	{
		CjvxProperties::_lock_properties();

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.buffersize.cat,
			CjvxAudioDevice_genpcg::properties_active.buffersize.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.format.cat,
			CjvxAudioDevice_genpcg::properties_active.format.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.formatselection.cat,
			CjvxAudioDevice_genpcg::properties_active.formatselection.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.inputchannelselection.cat,
			CjvxAudioDevice_genpcg::properties_active.inputchannelselection.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.numberinputchannels.cat,
			CjvxAudioDevice_genpcg::properties_active.numberinputchannels.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.numberoutputchannels.cat,
			CjvxAudioDevice_genpcg::properties_active.numberoutputchannels.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.outputchannelselection.cat,
			CjvxAudioDevice_genpcg::properties_active.outputchannelselection.id);

		CjvxProperties::_update_property_access_type(JVX_PROPERTY_ACCESS_READ_ONLY,
			CjvxAudioDevice_genpcg::properties_active.samplerate.cat,
			CjvxAudioDevice_genpcg::properties_active.samplerate.id);

		CjvxProperties::_unlock_properties();

        // Determine all relevant processing parameters
        _inproc.samplerate = CjvxAudioDevice::properties_active.samplerate.value;
        _inproc.buffersize = CjvxAudioDevice::properties_active.buffersize.value;
        _inproc.format = (jvxDataFormat)CjvxAudioDevice::properties_active.format.value;

        _inproc.numInputs = 0;
        _inproc.numOutputs = 0;

        _inproc.minChanCntInput = 0;
        _inproc.minChanCntOutput = 0;
        
        _inproc.maxChanCntInput = CjvxAudioDevice::properties_active.inputchannelselection.value.entries.size();
        _inproc.maxChanCntOutput = CjvxAudioDevice::properties_active.outputchannelselection.value.entries.size();
        
        _inproc.inChannelsSelectionMask = CjvxAudioDevice::properties_active.inputchannelselection.value.selection;
        _inproc.outChannelsSelectionMask = CjvxAudioDevice::properties_active.outputchannelselection.value.selection;

        for (i = 0; i < _inproc.maxChanCntInput; i++)
        {
            if (_inproc.inChannelsSelectionMask  & ((jvxBitField) 1 << i))
            {
                _inproc.numInputs++;
                _inproc.minChanCntInput = (jvxInt32)i;
            }
        }

        for (i = 0; i < _inproc.maxChanCntOutput; i++)
        {
            if (_inproc.outChannelsSelectionMask & ((jvxBitField) 1 << i))
            {
                _inproc.numOutputs++;
                _inproc.minChanCntOutput = (jvxInt32)i;
            }
        }

	}
	return(res);
}

jvxErrorType
CjvxAudioDevice::postprocess()
{
	jvxErrorType res = _postprocess();
	if(res == JVX_NO_ERROR)
	{
		CjvxProperties::_lock_properties();

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.buffersize.cat,
			CjvxAudioDevice_genpcg::properties_active.buffersize.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.format.cat,
			CjvxAudioDevice_genpcg::properties_active.format.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.formatselection.cat,
			CjvxAudioDevice_genpcg::properties_active.formatselection.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.inputchannelselection.cat,
			CjvxAudioDevice_genpcg::properties_active.inputchannelselection.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.numberinputchannels.cat,
			CjvxAudioDevice_genpcg::properties_active.numberinputchannels.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.numberoutputchannels.cat,
			CjvxAudioDevice_genpcg::properties_active.numberoutputchannels.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.outputchannelselection.cat,
			CjvxAudioDevice_genpcg::properties_active.outputchannelselection.id);

		CjvxProperties::_undo_update_property_access_type(
			CjvxAudioDevice_genpcg::properties_active.samplerate.cat,
			CjvxAudioDevice_genpcg::properties_active.samplerate.id);

		CjvxProperties::_unlock_properties();

		_inproc.samplerate = -1;
        _inproc.buffersize = -1;
        _inproc.format = JVX_DATAFORMAT_NONE;

        _inproc.numInputs = 0;
        _inproc.numOutputs = 0;

	}
	return(res);
}

#ifdef JVX_PROJECT_NAMESPACE
}
#endif