#ifndef __CJVXHOSTTYPEHANDLER_H__
#define __CJVXHOSTTYPEHANDLER_H__


	typedef struct
	{
		jvxSize id;
		bool isExternalComponent;

		jvxComponentType tp;
		jvxBool allowsMultiObjects;
		IjvxObject* hObject;
		jvxBitField dllProps;

		struct
		{
			std::string description;
		} externalLink;

		struct
		{
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
			JVX_HMODULE sharedObjectEntry;
			std::string dllPath;
#endif
			jvxInitObject_tp funcInit;
			jvxTerminateObject_tp funcTerm;
		} linkage;

	} oneComponentWrap;

	// >>>>>>>>>>>>>>>>>>>>>>>>

	typedef struct
	{
		oneComponentWrap common;
		IjvxTechnology* theHandle;
	} oneTechnology;

	typedef struct
	{
		std::vector<oneTechnology> availableTechnologies;
		IjvxTechnology* theHandle_shortcut_tech;
		IjvxDevice* theHandle_shortcut_dev;
	} technologiesT;

	// >>>>>>>>>>>>>>>>>>>>>>>>

	typedef struct
	{
		oneComponentWrap common;
		IjvxNode* theHandle;
	} oneNode;

	typedef struct
	{
		std::vector<oneNode> availableEndpoints;
		IjvxNode* theHandle_shortcut;
	} nodesT;

	// >>>>>>>>>>>>>>>>>>>>>>>>

	typedef struct
	{
		oneComponentWrap common;
		IjvxObject* theHandle;
		int refCount;
		std::vector<IjvxObject*> leased;
	} oneOtherComponent;

class CjvxHostTypeHandler
{
protected:
	typedef struct
	{
		technologiesT technologyInstances;
		jvxComponentType selector[2];
		std::string description;
		std::string tokenInConfigFile;
	} oneTechnologyType;

	typedef struct
	{
		nodesT nodeInstances;
		jvxComponentType selector[1];
		std::string description;
		std::string tokenInConfigFile;
	} oneNodeType;

	struct
	{
		std::vector<oneTechnologyType> registeredTechnologyTypes;
		std::vector<oneNodeType> registeredNodeTypes;
	} _common_set_types;

public:

	CjvxHostTypeHandler()
	{
	};

	~CjvxHostTypeHandler()
	{
	};

	jvxErrorType _add_type_host(IjvxObject* theObj, jvxComponentType* tp, jvxSize numtp, const char* description, const char* tokenConfig, jvxComponentTypeClass classType)
	{
		jvxErrorType res = JVX_NO_ERROR;
		jvxState theState = JVX_STATE_NONE;
		theObj->state(&theState);
		std::vector<oneTechnologyType>::iterator elm_tech;
		std::vector<oneNodeType>::iterator elm_end;
		oneTechnologyType newElm_tech;
		oneNodeType newElm_end;
		if(theState == JVX_STATE_SELECTED)
		{
			if(tp && (numtp >= 1))
			{
				elm_tech = _common_set_types.registeredTechnologyTypes.begin();
				elm_end = _common_set_types.registeredNodeTypes.begin();

				elm_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp[0], 0);
				elm_end = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp[0], 0);

				if((elm_tech == _common_set_types.registeredTechnologyTypes.end()) && (elm_end == _common_set_types.registeredNodeTypes.end()))
				{
					switch(classType)
					{
					case JVX_COMPONENT_TYPE_TECHNOLOGY:
						if(numtp >= 2)
						{
							newElm_tech.description = description;
							newElm_tech.tokenInConfigFile = tokenConfig;
							newElm_tech.selector[0] = tp[0];
							newElm_tech.selector[1] = tp[1];
							assert(numtp >= 2);
							newElm_tech.technologyInstances.theHandle_shortcut_dev = NULL;
							newElm_tech.technologyInstances.theHandle_shortcut_tech = NULL;
							_common_set_types.registeredTechnologyTypes.push_back(newElm_tech);
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
						break;
					case JVX_COMPONENT_TYPE_NODE:

						newElm_end.description = description;
	  					newElm_end.tokenInConfigFile = tokenConfig;
						newElm_end.selector[0] = tp[0];
						assert(numtp >= 1);
						newElm_end.nodeInstances.theHandle_shortcut = NULL;
						_common_set_types.registeredNodeTypes.push_back(newElm_end);
						break;
					default:
						res = JVX_ERROR_UNSUPPORTED;
					}
				}
				else
				{
					res = JVX_ERROR_DUPLICATE_ENTRY;
				}
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
		return(res);

	};

	jvxErrorType _number_types_host(jvxSize* num, jvxComponentTypeClass classType)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(num)
		{
			switch(classType)
			{
			case JVX_COMPONENT_TYPE_TECHNOLOGY:
				*num = _common_set_types.registeredTechnologyTypes.size();
				break;
			case JVX_COMPONENT_TYPE_NODE:
				*num = _common_set_types.registeredNodeTypes.size();
				break;
			default:
				res = JVX_ERROR_UNSUPPORTED;
			}
		}
		return(res);
	};

	jvxErrorType _description_type_host(jvxSize idx, jvxString** theDescription, jvxString** theTokenInConfig, jvxComponentType* tp, jvxSize numtp, jvxComponentTypeClass classType)
	{
		jvxErrorType res = JVX_NO_ERROR;

		switch(classType)
		{
		case JVX_COMPONENT_TYPE_TECHNOLOGY:
			if(idx < _common_set_types.registeredTechnologyTypes.size())
			{
				HjvxObject_produceString(theDescription, _common_set_types.registeredTechnologyTypes[idx].description);
				HjvxObject_produceString(theTokenInConfig, _common_set_types.registeredTechnologyTypes[idx].tokenInConfigFile);
				if(tp)
				{
					if(numtp >= 1)
					{
						tp[0] = _common_set_types.registeredTechnologyTypes[idx].selector[0];
					}
					if(numtp >= 2)
					{
						tp[1] = _common_set_types.registeredTechnologyTypes[idx].selector[1];
					}
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
			break;
		case JVX_COMPONENT_TYPE_NODE:
			if(idx < _common_set_types.registeredNodeTypes.size())
			{
				HjvxObject_produceString(theDescription, _common_set_types.registeredNodeTypes[idx].description);
				HjvxObject_produceString(theTokenInConfig, _common_set_types.registeredNodeTypes[idx].tokenInConfigFile);
				if(tp)
				{
					if(numtp >= 1)
					{
						tp[0] = _common_set_types.registeredNodeTypes[idx].selector[0];
					}
				}
			}
			else
			{
				res = JVX_ERROR_ID_OUT_OF_BOUNDS;
			}
			break;
		default:
			res = JVX_ERROR_UNSUPPORTED;
		}
		return(res);
	};

	jvxErrorType _remove_all_host_types()
	{
		_common_set_types.registeredTechnologyTypes.clear();
		_common_set_types.registeredNodeTypes.clear();
		return(JVX_NO_ERROR);
	};
};

#endif
