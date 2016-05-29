#ifndef __CJVXREALTIMEVIEWER_H__
#define __CJVXREALTIMEVIEWER_H__

#define JVX_RTV_MAIN_SECTION_NAME "rtv_section_main_name"
#define JVX_RTV_SUBSECTION_NAME "rtv_sub_section_name"
#define JVX_RTV_SUBSECTION_VIEW_TYPE "rtv_sub_section_viewtype"
#define JVX_RTV_ONE_SUBSECTION_PREFIX "rtv_sub_section_"
#define JVX_RTV_ONE_SUBGROUP_PREFIX "rtv_sub_group_"
#define JVX_RTV_GROUP_TYPE "rtv_sub_section_grouptype"
#define JVX_RTV_GROUP_DATA_DECODE_ID "rtv_sub_section_group_data_decode_id"
#define JVX_RTV_GROUP_DESCRIPTION "rtv_sub_section_group_description"
#define JVX_RTV_PROP_DESCRIPTIONS "rtv_item_prop_description"
#define JVX_RTV_ITEM_DESCRIPTIONS "rtv_item_item_description"
#define JVX_RTV_CATEGORIES "rtv_item_categories"
#define JVX_RTV_UPDATES "rtv_item_updates"
#define JVX_RTV_DECODE_IDS "rtv_item_decode_ids"
#define JVX_RTV_COMPONENT_TYPES "rtv_item_component_types"

#include "helpers/HjvxHostsAndProperties.h"
#include "helpers/tools/HjvxReadConfigEntries.h"
#include "helpers/tools/HjvxWriteConfigEntries.h"

#define ALLOW_TRIGGER_IN_STATE_ACTIVE

typedef enum
{
	JVX_REALTIME_VIEWER_VIEW_NONE = 0,
	JVX_REALTIME_VIEWER_VIEW_PROPERTIES = 1,
	JVX_REALTIME_VIEWER_VIEW_PLOTS = 2
} jvxRealtimeViewerType;

typedef enum
{
	JVX_REALTIME_VIEWER_GROUP_NONE = 0,
	JVX_REALTIME_VIEWER_GROUP_PROPERTY = 1, 
	JVX_REALTIME_VIEWER_GROUP_LINEAR_PLOT = 2,
	JVX_REALTIME_VIEWER_GROUP_CIRC_PLOT = 3,
	JVX_REALTIME_VIEWER_GROUP_POLAR_PLOT = 4
} jvxRealtimeViewerGroupType;

typedef enum
{
	JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE = 0,
	JVX_REALTIME_PRIVATE_MEMORY_LINEAR_PLOT_GROUP = 1,
	JVX_REALTIME_PRIVATE_MEMORY_LINEAR_PLOT_ITEM = 2,
	JVX_REALTIME_PRIVATE_MEMORY_LINEAR_SPLOT_SINGLE_DIAG_ITEM = 3,
	JVX_REALTIME_PRIVATE_MEMORY_PROPERTY_ITEM = 4
} jvxRealtimePrivateMemoryDecoderEnum;


typedef jvxErrorType (*getConfigurationPrivateData)(IjvxConfigProcessor* theReader, jvxConfigData* writeEntriesTo, jvxHandle* privateDataItem, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum typeHint, jvxInt16 itemId);
typedef jvxErrorType (*putConfigurationPrivateData)(IjvxConfigProcessor* theReader, jvxConfigData* readEntriesFrom, jvxHandle* privateDataItem, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum typeHint, jvxInt16 itemId);

typedef jvxErrorType (*copyHandler)(jvxHandle* copyTo, jvxHandle* copyFrom, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decId);
typedef jvxErrorType (*allocateHandler)(jvxHandle** privateData, jvxSize* sz, jvxRealtimePrivateMemoryDecoderEnum decodeHint);
typedef jvxErrorType (*deallocateHandler)(jvxHandle* privateData, jvxSize sz, jvxRealtimePrivateMemoryDecoderEnum decodeHint);

// ============================================================================

typedef jvxErrorType (*requestUpdateOnePropertyPlotItem)(jvxSize sectionId, jvxSize groupId, jvxSize itemId, jvxHandle* privateData);
typedef struct
{
	requestUpdateOnePropertyPlotItem callback_item;
	jvxHandle* privData;
} callbackStruct_section;

// ================================================================================

typedef jvxErrorType (*triggerUpdateOnePropertyPlotGroup)(jvxSize sectionId, jvxSize groupId, jvxBool callOnStart, jvxHandle* privateData);

typedef struct
{
	callbackStruct_section callback_section;
	triggerUpdateOnePropertyPlotGroup callback_group;
} callbackStruct_group;

// ================================================================================
class CjvxRealtimeViewer
{
protected:

	typedef struct
	{
		jvxComponentType tp;
		jvxSize inPropId;
		bool update;
		std::string description;
		struct
		{
			jvxHandle* data;
			jvxSize sz;
			jvxRealtimePrivateMemoryDecoderEnum decodeId;
		} prop;
	} oneEntryViewerItem;

	typedef struct
	{
		std::vector<oneEntryViewerItem> theItemList;
		std::string description;
		jvxRealtimeViewerGroupType group_tp;
		callbackStruct_group* callback_struct;
		bool update;
		struct
		{
			jvxHandle* data;
			jvxSize sz;
			jvxRealtimePrivateMemoryDecoderEnum decodeId;
		} prop;
	} oneEntryViewerGroup;

	typedef struct
	{
		std::vector<oneEntryViewerGroup> theViewList;
		jvxRealtimeViewerType whatToView;
		std::string description;
		callbackStruct_section* callback_struct;
		jvxBool visible;
	} oneEntryViewerSection;


	struct
	{
		std::vector<oneEntryViewerSection> lstViews;
		jvxState theState;
		allocateHandler theAllocater;
		deallocateHandler theDeallocater;
		copyHandler theCopier;
	} _content;

public:

	CjvxRealtimeViewer(allocateHandler theAllocater, deallocateHandler theDeallocater, copyHandler theCopier)
	{
		_content.theState = JVX_STATE_ACTIVE;
		_content.theAllocater = theAllocater;
		_content.theDeallocater = theDeallocater;
		_content.theCopier = theCopier;
	};

	~CjvxRealtimeViewer()
	{
		while(_content.lstViews.size() > 0)
		{
			_remove_view_section(0);
		}
	};

	jvxErrorType _insert_view_section_after(jvxRealtimeViewerType tp, const char* description, jvxInt32 position)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerSection newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			newItem.description = description;
			newItem.whatToView = tp;
			newItem.callback_struct = NULL;
			newItem.visible = true;

			if(jvx_insertListAfterPosition<oneEntryViewerSection>(_content.lstViews, newItem, position) == false)
			{
				_content.lstViews.push_back(newItem);
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _description_section(jvxSize sectionId, jvxRealtimeViewerType& tp, std::string& description)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				tp = _content.lstViews[sectionId].whatToView;
				if(_content.lstViews[sectionId].description.empty())
				{
					switch(tp)
					{
					case JVX_REALTIME_VIEWER_VIEW_PROPERTIES:
						description = "Property View (" + jvx_int2String(sectionId) + ")";//_content.lstViews[sectionId].description;
						break;
					case JVX_REALTIME_VIEWER_VIEW_PLOTS:
						description = "Plot View (" + jvx_int2String(sectionId) + ")";//_content.lstViews[sectionId].description;
						break;
					default:
						description = "Unknown";
					}
				}
				else
				{
					description = _content.lstViews[sectionId].description;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_description_section(jvxSize sectionId, const char*  description)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				_content.lstViews[sectionId].description = description;
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_update_callback_view_section(jvxSize sectionId, callbackStruct_section* callback_str)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				_content.lstViews[sectionId].callback_struct = callback_str;
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_update_callback_view_group_in_section(jvxSize sectionId, jvxSize groupId, callbackStruct_group* callback_str)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					_content.lstViews[sectionId].theViewList[groupId].callback_struct = callback_str;
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _run_viewer()
	{
		jvxErrorType res = JVX_NO_ERROR;
		// Note: It is assumed that all functions are called from within one single thread, therefore no state
		// protection
		if(_content.theState == JVX_STATE_ACTIVE)
		{
			_content.theState = JVX_STATE_PROCESSING;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _trigger_viewer()
	{
		jvxSize i,j,k;
		jvxErrorType res = JVX_NO_ERROR;

		if(
			(_content.theState == JVX_STATE_PROCESSING) 
#ifdef ALLOW_TRIGGER_IN_STATE_ACTIVE
			||(_content.theState == JVX_STATE_ACTIVE) 
#endif
			)
		{
			for(i = 0; i < _content.lstViews.size(); i++)
			{
				if(_content.lstViews[i].visible)
				{
					for(j = 0; j < _content.lstViews[i].theViewList.size(); j++)
					{
						if(_content.lstViews[i].theViewList[j].update)
						{
							if(_content.lstViews[i].theViewList[j].callback_struct)
							{
								if(_content.lstViews[i].theViewList[j].callback_struct->callback_group)
								{
									_content.lstViews[i].theViewList[j].callback_struct->callback_group(i,j,true,_content.lstViews[i].theViewList[j].callback_struct->callback_section.privData);
								}
							}
						}
						for(k = 0; k < _content.lstViews[i].theViewList[j].theItemList.size(); k++)
						{
							if(_content.lstViews[i].theViewList[j].theItemList[k].update)
							{
								if(_content.lstViews[i].theViewList[j].callback_struct)
								{
									if(_content.lstViews[i].theViewList[j].callback_struct->callback_section.callback_item)
									{
										_content.lstViews[i].theViewList[j].callback_struct->callback_section.callback_item(i,j,k,
											_content.lstViews[i].theViewList[j].callback_struct->callback_section.privData);
									}
								}
								else if(_content.lstViews[i].callback_struct)
								{
									if(_content.lstViews[i].callback_struct->callback_item)
									{
										_content.lstViews[i].callback_struct->callback_item(i,j,k,_content.lstViews[i].callback_struct->privData);
									}
								}
							}
						}
						if(_content.lstViews[i].theViewList[j].update)
						{
							if(_content.lstViews[i].theViewList[j].callback_struct)
							{
								if(_content.lstViews[i].theViewList[j].callback_struct->callback_group)
								{
									_content.lstViews[i].theViewList[j].callback_struct->callback_group(i,j,false, _content.lstViews[i].theViewList[j].callback_struct->callback_section.privData);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _stop_viewer()
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState == JVX_STATE_PROCESSING)
		{
			_content.theState = JVX_STATE_ACTIVE;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _number_sections(jvxSize* num)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerSection newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(num)
			{
				*num = _content.lstViews.size();
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _remove_view_section(jvxInt32 position)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerSection newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(position < (jvxInt32)_content.lstViews.size())
			{
				while(_content.lstViews[position].theViewList.size() > 0)
				{
					_remove_group_in_section(position, 0);
				}
				res = jvx_removeItemFromPosition<oneEntryViewerSection>(_content.lstViews, position);
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _insert_group_in_section(jvxSize sectionId, jvxInt32 position, const char* descr, jvxRealtimeViewerGroupType tp, jvxHandle* theProp,
		jvxSize szProp, jvxRealtimePrivateMemoryDecoderEnum decodePropId)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				newItem.group_tp = tp;
				newItem.description = descr;
				newItem.update = true;
				newItem.callback_struct = NULL;
				newItem.prop.data = NULL;
				newItem.prop.sz = 0;
				newItem.prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
				if(theProp && szProp)
				{
					newItem.prop.sz = szProp;
					newItem.prop.decodeId = decodePropId;
					if(_content.theAllocater(&newItem.prop.data, &newItem.prop.sz, newItem.prop.decodeId) == JVX_NO_ERROR)
					{
						assert(newItem.prop.sz == szProp);
						_content.theCopier(newItem.prop.data, theProp, newItem.prop.sz, newItem.prop.decodeId);
					}
					else
					{
						newItem.prop.sz = 0;
						newItem.prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
						newItem.prop.data = NULL;
					}
				}

				// If new item does not fit into the list, add it to end of list
				if(jvx_insertListAfterPosition<oneEntryViewerGroup>(_content.lstViews[sectionId].theViewList, newItem, position) == false)
				{
					_content.lstViews[sectionId].theViewList.push_back(newItem);
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _remove_group_in_section(jvxSize sectionId, jvxSize position)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(position < _content.lstViews[sectionId].theViewList.size())
				{
					// Remove all items in group
					while(_content.lstViews[sectionId].theViewList[position].theItemList.size() > 0)
					{
						_remove_item_in_group_in_section(sectionId, position, 0);
					}

					if(_content.lstViews[sectionId].theViewList[position].prop.data)
					{
						_content.theDeallocater(_content.lstViews[sectionId].theViewList[position].prop.data,
							_content.lstViews[sectionId].theViewList[position].prop.sz,
							_content.lstViews[sectionId].theViewList[position].prop.decodeId);

						_content.lstViews[sectionId].theViewList[position].prop.data = NULL;
						_content.lstViews[sectionId].theViewList[position].prop.sz = 0;
						_content.lstViews[sectionId].theViewList[position].prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
					}

					// Remove the group
					jvx_removeItemFromPosition<oneEntryViewerGroup>(_content.lstViews[sectionId].theViewList, position);
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _number_groups_in_section(jvxSize sectionId, jvxSize* num)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerGroup newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(num)
				{
					*num = _content.lstViews[sectionId].theViewList.size();
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _insert_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxComponentType tp, jvxSize inPropId, jvxInt32 position, 
		std::string& descr, jvxHandle* theProp, jvxSize szProp, jvxRealtimePrivateMemoryDecoderEnum decodePropId)
	{
		jvxErrorType res = JVX_NO_ERROR;
		oneEntryViewerItem newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					newItem.tp = tp;
					newItem.inPropId = inPropId;
					newItem.update = true;
					newItem.description = descr;
					newItem.prop.data = NULL;
					newItem.prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
					newItem.prop.sz = 0;
					if(szProp > 0)
					{
						newItem.prop.sz = szProp;
						newItem.prop.decodeId = decodePropId;
						if(_content.theAllocater(&newItem.prop.data, &newItem.prop.sz, newItem.prop.decodeId) == JVX_NO_ERROR)
						{
							assert(newItem.prop.sz == szProp);
							_content.theCopier(newItem.prop.data, theProp, newItem.prop.sz, newItem.prop.decodeId);
						}
						else
						{
							newItem.prop.data = NULL;
							newItem.prop.sz = 0;
							newItem.prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
						}
					}

					if(res == JVX_NO_ERROR)
					{
						if(jvx_insertListAfterPosition<oneEntryViewerItem>(_content.lstViews[sectionId].theViewList[groupId].theItemList, newItem, position) == false)
						{
							_content.lstViews[sectionId].theViewList[groupId].theItemList.push_back(newItem);
						}
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _description_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize itemId, jvxSize* inPropId, jvxComponentType* tp, std::string& nm)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(itemId < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						if(tp)
						{
							*tp = _content.lstViews[sectionId].theViewList[groupId].theItemList[itemId].tp;
						}
						if(inPropId)
						{
							*inPropId = _content.lstViews[sectionId].theViewList[groupId].theItemList[itemId].inPropId;
						}
						nm = _content.lstViews[sectionId].theViewList[groupId].theItemList[itemId].description;
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _description_group_in_section(jvxSize sectionId, jvxSize groupId, jvxRealtimeViewerGroupType& tp, std::string& description)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(_content.lstViews[sectionId].theViewList[groupId].description.empty())
					{
						switch(_content.lstViews[sectionId].theViewList[groupId].group_tp)
						{
						case JVX_REALTIME_VIEWER_GROUP_LINEAR_PLOT:
							description = "Linear Plot (" + jvx_int2String(groupId) + ")";
							break;
						case JVX_REALTIME_VIEWER_GROUP_POLAR_PLOT:
							description = "Polar Plot (" + jvx_int2String(groupId) + ")";
							break;
						case JVX_REALTIME_VIEWER_GROUP_PROPERTY:
							description = "Property (" + jvx_int2String(groupId) + ")";
							break;
						default:
							description = "Unknwon";
							break;
						}
					}
					else
					{
						description =  _content.lstViews[sectionId].theViewList[groupId].description;
					}
					tp = _content.lstViews[sectionId].theViewList[groupId].group_tp;
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_description_group_in_section(jvxSize sectionId, jvxSize groupId, const char* description)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					_content.lstViews[sectionId].theViewList[groupId].description = description;
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_update_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize itemId, jvxBool update)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(itemId < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						_content.lstViews[sectionId].theViewList[groupId].theItemList[itemId].update = update;
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _update_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize itemId, jvxBool* update)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(itemId < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						if(update)
						{
							*update = _content.lstViews[sectionId].theViewList[groupId].theItemList[itemId].update;
						}
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};


		jvxErrorType _set_update_group_in_section(jvxSize sectionId, jvxSize groupId, jvxBool update)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					_content.lstViews[sectionId].theViewList[groupId].update = update;
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _update_group_in_section(jvxSize sectionId, jvxSize groupId, jvxBool* update)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(update)
					{
						*update = _content.lstViews[sectionId].theViewList[groupId].update;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};



	jvxErrorType _remove_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize position)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(position < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						if(_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.data)
						{
							_content.theDeallocater(_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.data,
								_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.sz,
								_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.decodeId);

							_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.data = NULL;
							_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.sz = 0;
							_content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
						}
						jvx_removeItemFromPosition<oneEntryViewerItem>(_content.lstViews[sectionId].theViewList[groupId].theItemList, position);

					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _request_property_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize position, jvxHandle** prop, jvxSize* szFld, jvxInt16* decodeId)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(position < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						if(prop)
						{
							*prop = _content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.data;
						}
						if(szFld)
						{
							*szFld = _content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.sz;
						}
						if(decodeId)
						{
							*decodeId = _content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.decodeId;
						}
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _request_property_group_in_section(jvxSize sectionId, jvxSize groupId, jvxHandle** prop, jvxSize* szFld, jvxInt16* decodeId)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(prop)
					{
						*prop = _content.lstViews[sectionId].theViewList[groupId].prop.data;
					}
					if(szFld)
					{
						*szFld = _content.lstViews[sectionId].theViewList[groupId].prop.sz;
					}
					if(decodeId)
					{
						*decodeId = _content.lstViews[sectionId].theViewList[groupId].prop.decodeId;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _return_property_item_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize position, jvxHandle* prop)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(position < _content.lstViews[sectionId].theViewList[groupId].theItemList.size())
					{
						if(prop  != _content.lstViews[sectionId].theViewList[groupId].theItemList[position].prop.data)
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
					}
					else
					{
						res = JVX_ERROR_ID_OUT_OF_BOUNDS;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _return_property_group_in_section(jvxSize sectionId, jvxSize groupId, jvxHandle* prop)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(prop != _content.lstViews[sectionId].theViewList[groupId].prop.data)
					{
						res = JVX_ERROR_INVALID_ARGUMENT;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _number_items_in_group_in_section(jvxSize sectionId, jvxSize groupId, jvxSize* num)
	{
		jvxErrorType res = JVX_NO_ERROR;
		//oneEntryViewerItem newItem;

		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(groupId < _content.lstViews[sectionId].theViewList.size())
				{
					if(num)
					{
						*num = _content.lstViews[sectionId].theViewList[groupId].theItemList.size();
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _set_section_visible(jvxSize sectionId, jvxBool isVisible)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				_content.lstViews[sectionId].visible = isVisible;
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _get_section_visible(jvxSize sectionId, jvxBool* isVisible)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(_content.theState >= JVX_STATE_ACTIVE)
		{
			if(sectionId < _content.lstViews.size())
			{
				if(isVisible)
				{
					*isVisible = _content.lstViews[sectionId].visible;
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);
	};

	jvxErrorType _get_configuration(IjvxConfigProcessor* theReader, jvxHandle* ir, IjvxHost* theHost, getConfigurationPrivateData theCallback, std::string postfixMainSection)
	{
		jvxSize i,j,k;
		jvxConfigData* datTmp1 = NULL;
		jvxConfigData* datTmp2 = NULL;
		jvxConfigData* datTmp3 = NULL;
		jvxConfigData* datTmp4 = NULL;
		jvx_propertyReferenceTriple theTriple;
		jvxErrorType res = JVX_NO_ERROR;
		jvxString* fldStr = NULL;
		jvxPropertyCategoryType cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		jvxInt16 val = 0;

		jvx_initPropertyReferenceTriple(&theTriple);

		std::string entry;
		std::vector<jvxInt16> entries_cat;
		std::vector<std::string> entries_item_descrs;
		std::vector<std::string> entries_prop_descrs;

		std::vector<jvxInt16> entries_updates;
		std::vector<jvxInt16> entries_types;
		std::vector<jvxInt16> entries_decode_id;

		if(theReader)
		{
			res = theReader->createEmptySection(&datTmp1, (JVX_RTV_MAIN_SECTION_NAME + (std::string)"_" + postfixMainSection).c_str());
			if((res== JVX_NO_ERROR) && datTmp1)
			{
				for(i = 0; i < _content.lstViews.size(); i++)
				{
					entry = JVX_RTV_ONE_SUBSECTION_PREFIX + jvx_int2String(i);
					res = theReader->createEmptySection(&datTmp2, entry.c_str());
					if((res == JVX_NO_ERROR) && datTmp2)
					{
						val = _content.lstViews[i].whatToView;
						HjvxConfigProcessor_writeEntry_assignmentString(theReader, datTmp2, JVX_RTV_SUBSECTION_NAME, &_content.lstViews[i].description);
						HjvxConfigProcessor_writeEntry_assignmentInt16(theReader, datTmp2, JVX_RTV_SUBSECTION_VIEW_TYPE, &val);

						for(j = 0; j < _content.lstViews[i].theViewList.size(); j++)
						{
							entry = JVX_RTV_ONE_SUBGROUP_PREFIX + jvx_int2String(j);
							res = theReader->createEmptySection(&datTmp3, entry.c_str());
							if((res == JVX_NO_ERROR) && datTmp3)
							{
								val = _content.lstViews[i].theViewList[j].group_tp;

								HjvxConfigProcessor_writeEntry_assignmentInt16(theReader, datTmp3, JVX_RTV_GROUP_TYPE, &val);
								HjvxConfigProcessor_writeEntry_assignmentString(theReader, datTmp3, JVX_RTV_GROUP_DESCRIPTION, &_content.lstViews[i].theViewList[j].description);
								val = _content.lstViews[i].theViewList[j].prop.decodeId;
								if(_content.lstViews[i].theViewList[j].prop.data)
								{
									// Write private content to config file
									if(theCallback)
									{
										theCallback(theReader, datTmp3, _content.lstViews[i].theViewList[j].prop.data, _content.lstViews[i].theViewList[j].prop.sz, _content.lstViews[i].theViewList[j].prop.decodeId, -1);
									}
								}
								HjvxConfigProcessor_writeEntry_assignmentInt16(theReader, datTmp3, JVX_RTV_GROUP_DATA_DECODE_ID, &val);

								entries_cat.clear();
								entries_updates.clear();
								entries_types.clear();
								entries_decode_id.clear();
								entries_item_descrs.clear();
								entries_prop_descrs.clear();

								entry = "";

								for(k = 0; k < _content.lstViews[i].theViewList[j].theItemList.size(); k++)
								{
									res = jvx_getReferencePropertiesObject(theHost, &theTriple, _content.lstViews[i].theViewList[j].theItemList[k].tp);
									if((res == JVX_NO_ERROR)&& theTriple.theProps)
									{
										fldStr = NULL;
										res = theTriple.theProps->description_property(_content.lstViews[i].theViewList[j].theItemList[k].inPropId,
											&cat, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fldStr, NULL);
										if((res == JVX_NO_ERROR) && fldStr)
										{
											entries_cat.push_back(cat);
											entries_prop_descrs.push_back(fldStr->bString);
											entries_item_descrs.push_back(_content.lstViews[i].theViewList[j].theItemList[k].description);
											entries_updates.push_back(_content.lstViews[i].theViewList[j].theItemList[k].update);
											theTriple.theObj->deallocate(fldStr);
											entries_types.push_back(_content.lstViews[i].theViewList[j].theItemList[k].tp);
											if(_content.lstViews[i].theViewList[j].theItemList[k].prop.data)
											{
												entries_decode_id.push_back(_content.lstViews[i].theViewList[j].theItemList[k].prop.decodeId);
											}
											else
											{
												entries_decode_id.push_back(-1);
											}

											if(_content.lstViews[i].theViewList[j].theItemList[k].prop.data)
											{
												if(theCallback)
												{
													theCallback(
														theReader, datTmp3, _content.lstViews[i].theViewList[j].theItemList[k].prop.data,
														_content.lstViews[i].theViewList[j].theItemList[k].prop.sz,
														_content.lstViews[i].theViewList[j].theItemList[k].prop.decodeId,
														k);
												}
											}
										}
										res = jvx_returnReferencePropertiesObject(theHost, &theTriple, _content.lstViews[i].theViewList[j].theItemList[k].tp);
									}
								}

								HjvxConfigProcessor_writeEntry_1dInt16List(theReader, datTmp3, JVX_RTV_COMPONENT_TYPES, entries_types);
								HjvxConfigProcessor_writeEntry_1dInt16List(theReader, datTmp3, JVX_RTV_CATEGORIES, entries_cat);
								HjvxConfigProcessor_writeEntry_1dInt16List(theReader, datTmp3, JVX_RTV_UPDATES, entries_updates);
								HjvxConfigProcessor_writeEntry_1dInt16List(theReader, datTmp3, JVX_RTV_DECODE_IDS, entries_decode_id);
								HjvxConfigProcessor_writeEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_ITEM_DESCRIPTIONS, &entries_item_descrs);
								HjvxConfigProcessor_writeEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_PROP_DESCRIPTIONS, &entries_prop_descrs);

								// Write to config file all entry lists..
								theReader->addSubsectionToSection(datTmp2, datTmp3);
							}
						}
						theReader->addSubsectionToSection(datTmp1, datTmp2);
					}
				}
				theReader->addSubsectionToSection(ir, datTmp1);
			}
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _put_configuration(IjvxConfigProcessor* theReader, jvxHandle* ir, IjvxHost* theHost, const char* fName, int lineno, std::vector<std::string>& warnings, putConfigurationPrivateData putConfig, std::string postfixMainSection)
	{
		jvxSize i,j,k,l;
		jvxConfigData* datTmp1 = NULL;
		jvxConfigData* datTmp2 = NULL;
		jvxConfigData* datTmp3 = NULL;
		jvxConfigData* datTmp4 = NULL;
		jvx_propertyReferenceTriple theTriple;
		jvxErrorType res = JVX_NO_ERROR;
		jvxString* fldStr = NULL;
		jvxPropertyCategoryType cat = JVX_PROPERTY_CATEGORY_PREDEFINED;
		jvxSize numS = 0;
		jvxSize numG = 0;
		jvxSize numP = 0;
		//jvxInt16 val = 0;
		std::string group_descr;
		jvxRealtimeViewerGroupType group_tp;
		jvxRealtimePrivateMemoryDecoderEnum decodeIdGroup = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
		jvxHandle* data = NULL;
		jvxSize sz = 0;
		jvxRealtimePrivateMemoryDecoderEnum decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
		jvxSize idAddS = 0;
		jvxSize idAddG = 0;
		jvx_initPropertyReferenceTriple(&theTriple);

		std::string entry;
		std::vector<jvxInt16> entries_cat;
		std::vector<std::string> entries_item_descrs;
		std::vector<std::string> entries_prop_descrs;
		std::vector<jvxInt16> entries_updates;
		std::vector<jvxInt16> entries_types;
		std::vector<jvxRealtimePrivateMemoryDecoderEnum> entries_decode_id;

		jvxRealtimeViewerType viewerTp = JVX_REALTIME_VIEWER_VIEW_NONE;

		if(theReader && theHost)
		{
			res = theReader->getReferenceSubsectionCurrentSection_name(ir, &datTmp1, (JVX_RTV_MAIN_SECTION_NAME + (std::string)"_" + postfixMainSection).c_str());
			if((res== JVX_NO_ERROR) && datTmp1)
			{
				res = theReader->getNumberSubsectionsCurrentSection(datTmp1, &numS);

				for(i = 0; i < numS; i++)
				{
					datTmp2 = NULL;
					res = theReader->getReferenceSubsectionCurrentSection_id(datTmp1, &datTmp2, i);
					if((res == JVX_NO_ERROR) && datTmp2)
					{
						entry = "No description";
						HjvxConfigProcessor_readEntry_assignmentString(theReader, datTmp2, JVX_RTV_SUBSECTION_NAME, &entry);
						//HjvxConfigProcessor_readEntry_assignmentInt16(theReader, datTmp2, JVX_RTV_SUBSECTION_VIEW_TYPE, &val);
						HjvxConfigProcessor_readEntry_assignmentSpec<jvxRealtimeViewerType>(theReader, datTmp2, JVX_RTV_SUBSECTION_VIEW_TYPE, &viewerTp);
						bool isThereAValidGroup = false;
						if(res == JVX_NO_ERROR)
						{
							res = theReader->getNumberSubsectionsCurrentSection(datTmp2, &numG);
							if(res == JVX_NO_ERROR)
							{
								for(j = 0; j < numG; j++)
								{
									res = theReader->getReferenceSubsectionCurrentSection_id(datTmp2, &datTmp3, j);

									entries_cat.clear();
									entries_updates.clear();
									entries_types.clear();
									entries_item_descrs.clear();
									entries_prop_descrs.clear();
									entries_decode_id.clear();

									HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_COMPONENT_TYPES, entries_types);
									HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_UPDATES, entries_updates);
									HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_CATEGORIES, entries_cat);
									HjvxConfigProcessor_readEntry_1dSpecList<jvxRealtimePrivateMemoryDecoderEnum>(theReader, datTmp3, JVX_RTV_DECODE_IDS, entries_decode_id);
									HjvxConfigProcessor_readEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_ITEM_DESCRIPTIONS, &entries_item_descrs);
									HjvxConfigProcessor_readEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_PROP_DESCRIPTIONS, &entries_prop_descrs);


									jvxSize minVal = JVX_MIN(entries_types.size(), entries_updates.size());
									minVal = JVX_MIN(minVal, entries_cat.size());
									minVal = JVX_MIN(minVal, entries_decode_id.size());

									minVal = JVX_MIN(minVal, entries_item_descrs.size());
									minVal = JVX_MIN(minVal, entries_prop_descrs.size());

									bool isThereAValidEntry = false;

									/* Check wether there is a valid entry within this group */
									for(k = 0; k < minVal; k++)
									{
										jvx_initPropertyReferenceTriple(&theTriple);
										res = jvx_getReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
										if((res == JVX_NO_ERROR) && theTriple.theProps)
										{
											res = theTriple.theProps->number_properties(&numP);
											if(res == JVX_NO_ERROR)
											{
												bool foundit = false;
												for(l = 0; l < numP; l++)
												{
													fldStr = NULL;
													res = theTriple.theProps->description_property(l, &cat, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fldStr, NULL);
													if((res == JVX_NO_ERROR) && fldStr)
													{
														if((cat == entries_cat[k]) &&
															(entries_prop_descrs[k] == fldStr->bString))
														{
															//this->_insert_item_in_group_in_section(i, j, (jvxComponentType)entries_types[k], l, -1, 0);
															foundit = true;
															isThereAValidEntry = true;
															break;
														}
													}
												}
												if(!foundit)
												{
													warnings.push_back("Failed to link property with description <" + entries_prop_descrs[k] + ">, component type " + jvxComponentType_txt( entries_types[k]) + ".");
												}
											}
										}
										else
										{
											warnings.push_back("Reference to properties of component type " + jvxComponentType_txt( entries_types[k]) + " could not be acquired.");
										}
										jvx_returnReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
									}

									if(isThereAValidEntry)
									{
										isThereAValidGroup = true;
									}
									else
									{
										warnings.push_back("No Valid entry for group #" + jvx_int2String(j) + ".");
									}
								}
							}
						}

						if(isThereAValidGroup)
						{
							res = this->_insert_view_section_after(viewerTp, entry.c_str(), -1);
							if(res == JVX_NO_ERROR)
							{
								res = theReader->getNumberSubsectionsCurrentSection(datTmp2, &numG);
								if(res == JVX_NO_ERROR)
								{
									for(j = 0; j < numG; j++)
									{
										res = theReader->getReferenceSubsectionCurrentSection_id(datTmp2, &datTmp3, j);

										group_tp = JVX_REALTIME_VIEWER_GROUP_PROPERTY;
										HjvxConfigProcessor_readEntry_assignmentSpec<jvxRealtimeViewerGroupType>(theReader, datTmp3, JVX_RTV_GROUP_TYPE, &group_tp);

										decodeIdGroup = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;
										HjvxConfigProcessor_readEntry_assignmentSpec<jvxRealtimePrivateMemoryDecoderEnum>(theReader, datTmp3, JVX_RTV_GROUP_DATA_DECODE_ID, &decodeIdGroup);

										HjvxConfigProcessor_readEntry_assignmentString(theReader, datTmp3, JVX_RTV_GROUP_DESCRIPTION, &group_descr);

										entries_cat.clear();
										entries_updates.clear();
										entries_types.clear();
										entries_item_descrs.clear();
										entries_prop_descrs.clear();
										entries_decode_id.clear();

										HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_COMPONENT_TYPES, entries_types);
										HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_UPDATES, entries_updates);
										HjvxConfigProcessor_readEntry_1dInt16List(theReader, datTmp3, JVX_RTV_CATEGORIES, entries_cat);
										HjvxConfigProcessor_readEntry_1dSpecList<jvxRealtimePrivateMemoryDecoderEnum>(theReader, datTmp3, JVX_RTV_DECODE_IDS, entries_decode_id);

										HjvxConfigProcessor_readEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_PROP_DESCRIPTIONS, &entries_prop_descrs);
										HjvxConfigProcessor_readEntry_assignmentStringList(theReader, datTmp3, JVX_RTV_ITEM_DESCRIPTIONS, &entries_item_descrs);

										jvxSize minVal = JVX_MIN(entries_types.size(), entries_updates.size());
										minVal = JVX_MIN(minVal, entries_cat.size());
										minVal = JVX_MIN(minVal, entries_decode_id.size());

										minVal = JVX_MIN(minVal, entries_item_descrs.size());
										minVal = JVX_MIN(minVal, entries_prop_descrs.size());

										bool isThereAValidEntry = false;

										/* Check wether there is a valid entry within this group */
										for(k = 0; k < minVal; k++)
										{
											jvx_initPropertyReferenceTriple(&theTriple);
											res = jvx_getReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
											if((res == JVX_NO_ERROR) && theTriple.theProps)
											{
												res = theTriple.theProps->number_properties(&numP);
												if(res == JVX_NO_ERROR)
												{
													for(l = 0; l < numP; l++)
													{
														fldStr = NULL;
														res = theTriple.theProps->description_property(l, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
														if(res == JVX_NO_ERROR)
														{
															isThereAValidEntry = true;
															break;
														}
													}
												}
												jvx_returnReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
											}
										}

										if(isThereAValidEntry)
										{
											data = NULL;
											sz = 0;
											decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;

											if(decodeIdGroup >= 0)
											{
												decodeId = decodeIdGroup;
												if(_content.theAllocater( &data, &sz, decodeId) == JVX_NO_ERROR)
												{
													if(putConfig)
													{
														putConfig(theReader, datTmp3, data, sz, decodeId, -1);
													}
												}
											}

											this->_number_sections(&idAddS);
											assert(idAddS > 0);
											res = this->_insert_group_in_section(idAddS-1, -1, group_descr.c_str(), group_tp, data, sz, decodeId);

											//
											if(data)
											{
												_content.theDeallocater(data, sz, decodeId);
											}

											for(k = 0; k < minVal; k++)
											{
												jvx_initPropertyReferenceTriple(&theTriple);
												res = jvx_getReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
												if((res == JVX_NO_ERROR) && theTriple.theProps)
												{
													res = theTriple.theProps->number_properties(&numP);
													if(res == JVX_NO_ERROR)
													{
														bool foundit = false;
														for(l = 0; l < numP; l++)
														{
															fldStr = NULL;
															res = theTriple.theProps->description_property(l, &cat, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fldStr, NULL);
															if((res == JVX_NO_ERROR) && fldStr)
															{
																if(
																	(cat == entries_cat[k]) &&
																	(entries_prop_descrs[k] == fldStr->bString))
																{
																	data = NULL;
																	sz = 0;
																	decodeId = JVX_REALTIME_PRIVATE_MEMORY_DECODER_NONE;

																	if(entries_decode_id[k] >= 0)
																	{
																		decodeId = entries_decode_id[k];
																		if(_content.theAllocater(&data, &sz, decodeId) == JVX_NO_ERROR)
																		{
																			if(putConfig)
																			{
																				res = putConfig(theReader, datTmp3, data, sz, decodeId, k);
																			}
																		}
																	}

																	if(res == JVX_NO_ERROR)
																	{
																		_number_groups_in_section(idAddS-1, &idAddG);
																		assert(idAddG > 0);
																		this->_insert_item_in_group_in_section(idAddS-1, /*j*/ idAddG-1, (jvxComponentType)entries_types[k], l, -1, entries_item_descrs[k], data, sz, decodeId); // <- If groups were not added before, we need to get the next valid id
																	}
																	else
																	{
																		warnings.push_back("Failed to add property item with description <" + entries_prop_descrs[k] + ">, component type " + jvxComponentType_txt( entries_types[k]) + ": failed to read configuration.");
																	}
																	if(data)
																	{
																		_content.theDeallocater(data, sz, decodeId);
																	}
																	foundit = true;
																	break;
																}
															}
														}
														if(!foundit)
														{
															warnings.push_back("Failed to link property with description <" + entries_prop_descrs[k] + ">, component type " + jvxComponentType_txt( entries_types[k]) + ".");
														}
													}
													jvx_returnReferencePropertiesObject(theHost, &theTriple, (jvxComponentType)entries_types[k]);
												}
											}
										}
									}
								}
							}
						}//if(isThereAValidGroup)
						else
						{
							warnings.push_back("Section <" + entry + "> not added since real-time reference groups do not contain valid entries.");
						}
					}
				}
			}
		}
		return(JVX_NO_ERROR);
	};
};

#endif
