#ifndef _CJVXAUDIONODE_GENPCG_H__
#define _CJVXAUDIONODE_GENPCG_H__

#include "helpers/tools/HjvxReadConfigEntries.h"
#include "helpers/tools/HjvxWriteConfigEntries.h"

class CjvxAudioNode_genpcg
{
public:
	CjvxAudioNode_genpcg(){};

	//=================================================
	// Member Variables
	//=================================================



	// ---- Properties in state active ---
	struct 
	{
		// Audio buffersize
		struct 
		{
			jvxInt32 value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} buffersize;
		// Sample rate Audio device
		struct 
		{
			jvxInt32 value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} samplerate;
		// Processing format
		struct 
		{
			jvxInt16 value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} format;
		// Number of input channels
		struct 
		{
			jvxInt32 value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} numberinputchannels;
		// Number of output channels
		struct 
		{
			jvxInt32 value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} numberoutputchannels;
	} properties_parameters;

	// ---- Properties in state active ---
	struct 
	{
		// Engage Algorithm
		struct 
		{
			jvx_oneSelectionOption_cpp value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} engage;
	} properties_active_higher;

	//=================================================
	// Init functions
	//=================================================

	// ---- Properties in state active ---
	inline void init__properties_parameters()
	{
		// Initialize the struct to neutral defined values.
		properties_parameters.buffersize.value = 0;
		properties_parameters.buffersize.id = -1;
		properties_parameters.buffersize.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_parameters.buffersize.valid = false;
		properties_parameters.buffersize.frozen = false;
		properties_parameters.buffersize.onlySelectionToFile = false;
		properties_parameters.samplerate.value = 0;
		properties_parameters.samplerate.id = -1;
		properties_parameters.samplerate.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_parameters.samplerate.valid = false;
		properties_parameters.samplerate.frozen = false;
		properties_parameters.samplerate.onlySelectionToFile = false;
		properties_parameters.format.value = 0;
		properties_parameters.format.id = -1;
		properties_parameters.format.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_parameters.format.valid = false;
		properties_parameters.format.frozen = false;
		properties_parameters.format.onlySelectionToFile = false;
		properties_parameters.numberinputchannels.value = 0;
		properties_parameters.numberinputchannels.id = -1;
		properties_parameters.numberinputchannels.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_parameters.numberinputchannels.valid = false;
		properties_parameters.numberinputchannels.frozen = false;
		properties_parameters.numberinputchannels.onlySelectionToFile = false;
		properties_parameters.numberoutputchannels.value = 0;
		properties_parameters.numberoutputchannels.id = -1;
		properties_parameters.numberoutputchannels.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_parameters.numberoutputchannels.valid = false;
		properties_parameters.numberoutputchannels.frozen = false;
		properties_parameters.numberoutputchannels.onlySelectionToFile = false;
	};

	// ---- Properties in state active ---
	inline void init__properties_active_higher()
	{
		// Initialize the struct to neutral defined values.
		properties_active_higher.engage.value.selection = 0;
		properties_active_higher.engage.value.exclusive = 0;
		properties_active_higher.engage.value.entries.clear();
		properties_active_higher.engage.id = -1;
		properties_active_higher.engage.valid = false;
		properties_active_higher.engage.frozen = false;
		properties_active_higher.engage.onlySelectionToFile = false;
	};

	//=================================================
	// Allocation functions
	//=================================================

	// ---- Properties in state active ---
	inline void allocate__properties_parameters()
	{
		// Allocate memory and update registration with property handler if desired.
		// Audio buffersize
		properties_parameters.buffersize.value = 1024;
		properties_parameters.buffersize.valid = true;
		properties_parameters.buffersize.frozen = false;
		properties_parameters.buffersize.onlySelectionToFile = false;
		// Sample rate Audio device
		properties_parameters.samplerate.value = 48000;
		properties_parameters.samplerate.valid = true;
		properties_parameters.samplerate.frozen = false;
		properties_parameters.samplerate.onlySelectionToFile = false;
		// Processing format
		properties_parameters.format.value = 1;
		properties_parameters.format.valid = true;
		properties_parameters.format.frozen = false;
		properties_parameters.format.onlySelectionToFile = false;
		// Number of input channels
		properties_parameters.numberinputchannels.value = 0;
		properties_parameters.numberinputchannels.valid = true;
		properties_parameters.numberinputchannels.frozen = false;
		properties_parameters.numberinputchannels.onlySelectionToFile = false;
		// Number of output channels
		properties_parameters.numberoutputchannels.value = 0;
		properties_parameters.numberoutputchannels.valid = true;
		properties_parameters.numberoutputchannels.frozen = false;
		properties_parameters.numberoutputchannels.onlySelectionToFile = false;
	};

	// ---- Properties in state active ---
	inline void allocate__properties_active_higher()
	{
		// Allocate memory and update registration with property handler if desired.
		// Engage Algorithm
		properties_active_higher.engage.value.selection = 0;
		properties_active_higher.engage.value.exclusive = 0;
		properties_active_higher.engage.value.entries.push_back("Engage");
		properties_active_higher.engage.value.selection |= (1 << 0);
		properties_active_higher.engage.value.exclusive |= (1 << 0);
		properties_active_higher.engage.value.entries.push_back("Do not engage");
		properties_active_higher.engage.value.exclusive |= (1 << 1);
		properties_active_higher.engage.valid = true;
		properties_active_higher.engage.frozen = false;
		properties_active_higher.engage.onlySelectionToFile = false;
	};

	//=================================================
	// Deallocation functions
	//=================================================

	// ---- Properties in state active ---
	inline void deallocate__properties_parameters()
	{
		// If necessary, deallocate.
		// Audio buffersize
		// Sample rate Audio device
		// Processing format
		// Number of input channels
		// Number of output channels
	};

	// ---- Properties in state active ---
	inline void deallocate__properties_active_higher()
	{
		// If necessary, deallocate.
		// Engage Algorithm
	};

	//=================================================
	// Associate functions 
	//=================================================

	//=================================================

	// Deassociate functions 
	//=================================================

	//=================================================
	// Register functions 
	//=================================================

	// ---- Properties in state active ---
	inline void register__properties_parameters(CjvxProperties* theProps)
	{
		// Register properties with property management system.
		theProps->_lock_properties();
		properties_parameters.buffersize.id = 1;
		properties_parameters.buffersize.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_parameters.buffersize.value, 1, JVX_DATAFORMAT_32BIT_LE, properties_parameters.buffersize.cat, &properties_parameters.buffersize.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "buffersize", "Audio buffersize", "JVX_AUDIONODE_BUFFERSIZE", &properties_parameters.buffersize.valid, &properties_parameters.buffersize.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_parameters.samplerate.id = 3;
		properties_parameters.samplerate.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_parameters.samplerate.value, 1, JVX_DATAFORMAT_32BIT_LE, properties_parameters.samplerate.cat, &properties_parameters.samplerate.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "samplerate", "Sample rate Audio device", "JVX_AUDIONODE_SAMPLERATE", &properties_parameters.samplerate.valid, &properties_parameters.samplerate.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_parameters.format.id = 9;
		properties_parameters.format.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_parameters.format.value, 1, JVX_DATAFORMAT_16BIT_LE, properties_parameters.format.cat, &properties_parameters.format.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "format", "Processing format", "JVX_AUDIONODE_FORMAT", &properties_parameters.format.valid, &properties_parameters.format.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_FORMAT_IDX, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_parameters.numberinputchannels.id = 5;
		properties_parameters.numberinputchannels.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_parameters.numberinputchannels.value, 1, JVX_DATAFORMAT_32BIT_LE, properties_parameters.numberinputchannels.cat, &properties_parameters.numberinputchannels.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "numberinputchannels", "Number of input channels", "JVX_AUDIONODE_INPUT_CHANNELS", &properties_parameters.numberinputchannels.valid, &properties_parameters.numberinputchannels.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_parameters.numberoutputchannels.id = 6;
		properties_parameters.numberoutputchannels.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_parameters.numberoutputchannels.value, 1, JVX_DATAFORMAT_32BIT_LE, properties_parameters.numberoutputchannels.cat, &properties_parameters.numberoutputchannels.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "numberoutputchannels", "Number of output channels", "JVX_AUDIONODE_OUTPUT_CHANNELS", &properties_parameters.numberoutputchannels.valid, &properties_parameters.numberoutputchannels.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		theProps->_unlock_properties();

	};

	// ---- Properties in state active ---
	inline void register__properties_active_higher(CjvxProperties* theProps)
	{
		// Register properties with property management system.
		theProps->_lock_properties();
		properties_active_higher.engage.id = 16;
		properties_active_higher.engage.cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		theProps->_register_property(&properties_active_higher.engage.value, 1, JVX_DATAFORMAT_SELECTION_LIST, properties_active_higher.engage.cat, &properties_active_higher.engage.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "engage", "Engage Algorithm", "engage", &properties_active_higher.engage.valid, &properties_active_higher.engage.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_SINGLE_SELECTION, JVX_PROPERTY_CONTEXT_UNKNOWN);
		theProps->_unlock_properties();

	};

	//=================================================
	// Unregister functions 
	//=================================================

	// ---- Properties in state active ---
	inline void unregister__properties_parameters(CjvxProperties* theProps)
	{
		// Unregister properties with property management system.
		theProps->_lock_properties();
		theProps->_unregister_property(properties_parameters.buffersize.cat , properties_parameters.buffersize.id);
		theProps->_unregister_property(properties_parameters.samplerate.cat , properties_parameters.samplerate.id);
		theProps->_unregister_property(properties_parameters.format.cat , properties_parameters.format.id);
		theProps->_unregister_property(properties_parameters.numberinputchannels.cat , properties_parameters.numberinputchannels.id);
		theProps->_unregister_property(properties_parameters.numberoutputchannels.cat , properties_parameters.numberoutputchannels.id);
		theProps->_unlock_properties();

	};

	// ---- Properties in state active ---
	inline void unregister__properties_active_higher(CjvxProperties* theProps)
	{
		// Unregister properties with property management system.
		theProps->_lock_properties();
		theProps->_unregister_property(properties_active_higher.engage.cat , properties_active_higher.engage.id);
		theProps->_unlock_properties();

	};

	//=================================================
	// Config file read functions 
	//=================================================

	// ---- Properties in state active ---
	inline void put_configuration__properties_parameters(IjvxConfigProcessor* theReader, jvxHandle* ir, std::vector<std::string>& warnings_properties)
	{
		// Put configuration to initialize the properties.
		if(HjvxConfigProcessor_readEntry_assignmentInt32(theReader, ir, "properties_parameters__buffersize", &properties_parameters.buffersize.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_parameters__buffersize");
		}
		if(HjvxConfigProcessor_readEntry_assignmentInt32(theReader, ir, "properties_parameters__samplerate", &properties_parameters.samplerate.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_parameters__samplerate");
		}
		if(HjvxConfigProcessor_readEntry_assignmentInt16(theReader, ir, "properties_parameters__format", &properties_parameters.format.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_parameters__format");
		}
	};

	// ---- Properties in state active ---
	inline void put_configuration__properties_active_higher(IjvxConfigProcessor* theReader, jvxHandle* ir, std::vector<std::string>& warnings_properties)
	{
		// Put configuration to initialize the properties.
		if(HjvxConfigProcessor_readEntry_assignmentSelectionList(theReader, ir, "properties_active_higher__engage", &properties_active_higher.engage.value, properties_active_higher.engage.onlySelectionToFile) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_active_higher__engage");
		}
	};

	//=================================================
	// Config file write functions 
	//=================================================

	// ---- Properties in state active ---
	inline void get_configuration__properties_parameters(IjvxConfigProcessor* theReader, jvxHandle* ir)
	{
		// Get configuration to store the property values.
		HjvxConfigProcessor_writeEntry_assignmentInt32(theReader, ir, "properties_parameters__buffersize", &properties_parameters.buffersize.value);
		HjvxConfigProcessor_writeEntry_assignmentInt32(theReader, ir, "properties_parameters__samplerate", &properties_parameters.samplerate.value);
		HjvxConfigProcessor_writeEntry_assignmentInt16(theReader, ir, "properties_parameters__format", &properties_parameters.format.value);
	};

	// ---- Properties in state active ---
	inline void get_configuration__properties_active_higher(IjvxConfigProcessor* theReader, jvxHandle* ir)
	{
		// Get configuration to store the property values.
		HjvxConfigProcessor_writeEntry_assignmentSelectionList(theReader, ir, "properties_active_higher__engage", &properties_active_higher.engage.value, properties_active_higher.engage.onlySelectionToFile);
	};

};
#endif
