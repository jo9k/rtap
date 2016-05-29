#ifndef _GENSIMPLE_NODE_H__
#define _GENSIMPLE_NODE_H__

#include "helpers/tools/HjvxReadConfigEntries.h"
#include "helpers/tools/HjvxWriteConfigEntries.h"

class genSimple_node
{
public:
	genSimple_node(){};

	//=================================================
	// Member Variables
	//=================================================



	// ---- Properties in state active ---
	struct 
	{
		// Volume for all channels
		struct 
		{
			jvxData value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} volume;
		// Angle for left-right panning
		struct 
		{
			jvxData value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} ANGLE;
		// Factor for left-right panning depth
		struct 
		{
			jvxData value;
			jvxInt32 id;
			jvxPropertyCategoryType cat;
			jvxBool valid;
			jvxBool frozen;
			jvxBool onlySelectionToFile;
		} CFAC;
	} properties_active;

	//=================================================
	// Init functions
	//=================================================

	// ---- Properties in state active ---
	inline void init__properties_active()
	{
		// Initialize the struct to neutral defined values.
		properties_active.volume.value = 0;
		properties_active.volume.id = -1;
		properties_active.volume.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_active.volume.valid = false;
		properties_active.volume.frozen = false;
		properties_active.volume.onlySelectionToFile = false;
		properties_active.ANGLE.value = 0;
		properties_active.ANGLE.id = -1;
		properties_active.ANGLE.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_active.ANGLE.valid = false;
		properties_active.ANGLE.frozen = false;
		properties_active.ANGLE.onlySelectionToFile = false;
		properties_active.CFAC.value = 0;
		properties_active.CFAC.id = -1;
		properties_active.CFAC.cat = JVX_PROPERTY_CATEGORY_UNKNOWN;
		properties_active.CFAC.valid = false;
		properties_active.CFAC.frozen = false;
		properties_active.CFAC.onlySelectionToFile = false;
	};

	//=================================================
	// Allocation functions
	//=================================================

	// ---- Properties in state active ---
	inline void allocate__properties_active()
	{
		// Allocate memory and update registration with property handler if desired.
		// Volume for all channels
		properties_active.volume.value = 1;
		properties_active.volume.valid = true;
		properties_active.volume.frozen = false;
		properties_active.volume.onlySelectionToFile = false;
		// Angle for left-right panning
		properties_active.ANGLE.value = 90;
		properties_active.ANGLE.valid = true;
		properties_active.ANGLE.frozen = false;
		properties_active.ANGLE.onlySelectionToFile = false;
		// Factor for left-right panning depth
		properties_active.CFAC.value = 1;
		properties_active.CFAC.valid = true;
		properties_active.CFAC.frozen = false;
		properties_active.CFAC.onlySelectionToFile = false;
	};

	//=================================================
	// Deallocation functions
	//=================================================

	// ---- Properties in state active ---
	inline void deallocate__properties_active()
	{
		// If necessary, deallocate.
		// Volume for all channels
		// Angle for left-right panning
		// Factor for left-right panning depth
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
	inline void register__properties_active(CjvxProperties* theProps)
	{
		// Register properties with property management system.
		theProps->_lock_properties();
		properties_active.volume.id = 0;
		properties_active.volume.cat = JVX_PROPERTY_CATEGORY_UNSPECIFIC;
		theProps->_register_property(&properties_active.volume.value, 1, JVX_DATAFORMAT_DATA, properties_active.volume.cat, &properties_active.volume.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "volume", "Volume for all channels", "volume", &properties_active.volume.valid, &properties_active.volume.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_active.ANGLE.id = -1;
		properties_active.ANGLE.cat = JVX_PROPERTY_CATEGORY_UNSPECIFIC;
		theProps->_register_property(&properties_active.ANGLE.value, 1, JVX_DATAFORMAT_DATA, properties_active.ANGLE.cat, &properties_active.ANGLE.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "ANGLE", "Angle for left-right panning", "ANGLE", &properties_active.ANGLE.valid, &properties_active.ANGLE.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		properties_active.CFAC.id = -1;
		properties_active.CFAC.cat = JVX_PROPERTY_CATEGORY_UNSPECIFIC;
		theProps->_register_property(&properties_active.CFAC.value, 1, JVX_DATAFORMAT_DATA, properties_active.CFAC.cat, &properties_active.CFAC.id, 0x1C, 0xFFFFFFFFFFFFFFFF, "CFAC", "Factor for left-right panning depth", "CFAC", &properties_active.CFAC.valid, &properties_active.CFAC.frozen, JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE, JVX_PROPERTY_DECODER_NONE, JVX_PROPERTY_CONTEXT_UNKNOWN);
		theProps->_unlock_properties();

	};

	//=================================================
	// Unregister functions 
	//=================================================

	// ---- Properties in state active ---
	inline void unregister__properties_active(CjvxProperties* theProps)
	{
		// Unregister properties with property management system.
		theProps->_lock_properties();
		theProps->_unregister_property(properties_active.volume.cat , properties_active.volume.id);
		theProps->_unregister_property(properties_active.ANGLE.cat , properties_active.ANGLE.id);
		theProps->_unregister_property(properties_active.CFAC.cat , properties_active.CFAC.id);
		theProps->_unlock_properties();

	};

	//=================================================
	// Config file read functions 
	//=================================================

	// ---- Properties in state active ---
	inline void put_configuration__properties_active(IjvxConfigProcessor* theReader, jvxHandle* ir, std::vector<std::string>& warnings_properties)
	{
		// Put configuration to initialize the properties.
		if(HjvxConfigProcessor_readEntry_assignmentData(theReader, ir, "properties_active__volume", &properties_active.volume.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_active__volume");
		}
		if(HjvxConfigProcessor_readEntry_assignmentData(theReader, ir, "properties_active__ANGLE", &properties_active.ANGLE.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_active__ANGLE");
		}
		if(HjvxConfigProcessor_readEntry_assignmentData(theReader, ir, "properties_active__CFAC", &properties_active.CFAC.value) != JVX_NO_ERROR)
		{
			warnings_properties.push_back("properties_active__CFAC");
		}
	};

	//=================================================
	// Config file write functions 
	//=================================================

	// ---- Properties in state active ---
	inline void get_configuration__properties_active(IjvxConfigProcessor* theReader, jvxHandle* ir)
	{
		// Get configuration to store the property values.
		HjvxConfigProcessor_writeEntry_assignmentData(theReader, ir, "properties_active__volume", &properties_active.volume.value);
		HjvxConfigProcessor_writeEntry_assignmentData(theReader, ir, "properties_active__ANGLE", &properties_active.ANGLE.value);
		HjvxConfigProcessor_writeEntry_assignmentData(theReader, ir, "properties_active__CFAC", &properties_active.CFAC.value);
	};

};
#endif
