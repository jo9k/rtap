#include "jvx.h"

#include "templates/jvxHosts/CjvxHost.h"

static jvxErrorType static_local_select(IjvxHiddenInterface* theHinterface,
#ifdef QT_ALLOW_QT_WIDGETS
										IjvxStateMachine* theObjectSm, IjvxQtWidgetHost* theWidgetHost)
#else
										IjvxStateMachine* theObjectSm)
#endif
{
	jvxErrorType res = JVX_NO_ERROR;
	assert(theObjectSm);
	assert(theHinterface);

#ifdef QT_ALLOW_QT_WIDGETS
	bool qtWaySuccess = false;

	if(theWidgetHost)
	{
		IjvxQtWidgetUi* theObjectQt = NULL;
		res = theHinterface->request_hidden_interface(JVX_INTERFACE_QT_WIDGET_UI, reinterpret_cast<jvxHandle**>(&theObjectQt));
		if((res == JVX_NO_ERROR) && theObjectQt)
		{
			res = theWidgetHost->select_component_qt_thread(theObjectQt);
			if(res == JVX_NO_ERROR)
			{
				qtWaySuccess = true;
			}
			theHinterface->return_hidden_interface(JVX_INTERFACE_QT_WIDGET_UI, reinterpret_cast<jvxHandle*>(theObjectQt));
		}

	}

	// The other case..
	if(!qtWaySuccess)
	{
		res = theObjectSm->select();
	}
#else
	res = theObjectSm->select();
#endif

	return(res);
}

static jvxErrorType static_local_unselect(IjvxHiddenInterface* theHinterface,
#ifdef QT_ALLOW_QT_WIDGETS
										IjvxStateMachine* theObjectSm, IjvxQtWidgetHost* theWidgetHost)
#else
										IjvxStateMachine* theObjectSm)
#endif
{
	jvxErrorType res = JVX_NO_ERROR;
	assert(theObjectSm);
	assert(theHinterface);

#ifdef QT_ALLOW_QT_WIDGETS
	bool qtWaySuccess = false;

	if(theWidgetHost)
	{
		IjvxQtWidgetUi* theObjectQt = NULL;
		res = theHinterface->request_hidden_interface(JVX_INTERFACE_QT_WIDGET_UI, reinterpret_cast<jvxHandle**>(&theObjectQt));
		if((res == JVX_NO_ERROR) && theObjectQt)
		{
			res = theWidgetHost->unselect_component_qt_thread(theObjectQt);
			if(res == JVX_NO_ERROR)
			{
				qtWaySuccess = true;
			}
			theHinterface->return_hidden_interface(JVX_INTERFACE_QT_WIDGET_UI, reinterpret_cast<jvxHandle*>(theObjectQt));
		}

	}

	// The other case..
	if(!qtWaySuccess)
	{
		res = theObjectSm->unselect();
	}
#else
	res = theObjectSm->unselect();
#endif

	return(res);
}

// ================================================================
// Control sub components
// ================================================================

jvxErrorType
CjvxHost::number_components(jvxComponentType tp, jvxSize* num)
{
	if(num)
	{
		bool foundit = false;
		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			*num = elmIt_tech->technologyInstances.availableTechnologies.size();
			foundit = true;
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
				{
					elmIt_dev->technologyInstances.theHandle_shortcut_tech->number_devices(num);
				}
				else
				{
					*num = 0;
				}
				foundit = true;
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					*num = elmIt_ep->nodeInstances.availableEndpoints.size();
					foundit = true;
				}
			}
		}

		if(!foundit)
		{
			*num = 0;
		}

		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_INVALID_ARGUMENT);
}

/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::description_component(jvxComponentType tp, jvxSize idx, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(idx < elmIt_tech->technologyInstances.availableTechnologies.size())
		{
			elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle->description(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.availableTechnologies[idx].common.hObject->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->description_device(idx, &strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					static_cast<IjvxObject*>(elmIt_dev->technologyInstances.theHandle_shortcut_tech)->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;
				if(idx < elmIt_ep->nodeInstances.availableEndpoints.size())
				{
					elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle->description(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.availableEndpoints[idx].common.hObject->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}

/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::descriptor_component(jvxComponentType tp, jvxSize idx, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(idx < elmIt_tech->technologyInstances.availableTechnologies.size())
		{
			elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle->descriptor(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.availableTechnologies[idx].common.hObject->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->descriptor_device(idx, &strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					static_cast<IjvxObject*>(elmIt_dev->technologyInstances.theHandle_shortcut_tech)->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;
				if(idx < elmIt_ep->nodeInstances.availableEndpoints.size())
				{
					elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle->descriptor(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.availableEndpoints[idx].common.hObject->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}

/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::module_name_component(jvxComponentType tp, jvxSize idx, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(idx < elmIt_tech->technologyInstances.availableTechnologies.size())
		{
			elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle->module_name(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.availableTechnologies[idx].common.hObject->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->module_name_device(idx, &strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					static_cast<IjvxObject*>(elmIt_dev->technologyInstances.theHandle_shortcut_tech)->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;
				if(idx < elmIt_ep->nodeInstances.availableEndpoints.size())
				{
					elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle->module_name(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.availableEndpoints[idx].common.hObject->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}

/*
 * Return feature class identifier
 */
jvxErrorType
CjvxHost::feature_class_component(jvxComponentType tp, jvxSize idx, jvxBitField* ft)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize num = 0;
	bool foundit = false;
	jvxState stat = JVX_STATE_NONE;
	IjvxDevice* dev = NULL;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(idx < elmIt_tech->technologyInstances.availableTechnologies.size())
		{
			elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle->feature_class(ft);
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
			{
				elmIt_dev->technologyInstances.theHandle_shortcut_tech->state(&stat);
				if(stat >= JVX_STATE_ACTIVE)
				{
					res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->number_devices(&num);
					if(idx < num)
					{
						res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->request_device(idx, &dev);
						if((res == JVX_NO_ERROR) && dev)
						{
							res = dev->feature_class(ft);
						}
						else
						{
							res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
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
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;
				if(idx < elmIt_ep->nodeInstances.availableEndpoints.size())
				{
					elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle->feature_class(ft);
				}
				else
				{
					res = JVX_ERROR_ID_OUT_OF_BOUNDS;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	return(res);
}


/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::module_name_component(jvxComponentType tp, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
		{
			elmIt_tech->technologyInstances.theHandle_shortcut_tech->module_name(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->module_name(&strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				if(elmIt_ep->nodeInstances.theHandle_shortcut)
				{
					elmIt_ep->nodeInstances.theHandle_shortcut->module_name(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.theHandle_shortcut->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}


/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::description_component(jvxComponentType tp, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
		{
			elmIt_tech->technologyInstances.theHandle_shortcut_tech->description(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->description(&strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				if(elmIt_ep->nodeInstances.theHandle_shortcut)
				{
					elmIt_ep->nodeInstances.theHandle_shortcut->description(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.theHandle_shortcut->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}


/*
 * Return the names of components of a specific type
 */
jvxErrorType
CjvxHost::descriptor_component(jvxComponentType tp, jvxString** str)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
		{
			elmIt_tech->technologyInstances.theHandle_shortcut_tech->descriptor(&strLoc);
			if(strLoc)
			{
				theDescr = strLoc->bString;
				elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(strLoc);
			}
			else
			{
				res = JVX_ERROR_INTERNAL;
			}
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->descriptor(&strLoc);
				if((res == JVX_NO_ERROR) && (strLoc))
				{
					theDescr = strLoc->bString;
					elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(strLoc);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
				strLoc = NULL;
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				if(elmIt_ep->nodeInstances.theHandle_shortcut)
				{
					elmIt_ep->nodeInstances.theHandle_shortcut->descriptor(&strLoc);
					if(strLoc)
					{
						theDescr = strLoc->bString;
						elmIt_ep->nodeInstances.theHandle_shortcut->deallocate(strLoc);
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
					}
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(res == JVX_NO_ERROR)
	{
		if(str)
		{
			HjvxObject_produceString(str, theDescr);
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	return(res);
}

/*
 * Return feature class identifier
 */
jvxErrorType
CjvxHost::feature_class_component(jvxComponentType tp, jvxBitField* ft)
{
	std::string theDescr;
	jvxString*strLoc = NULL;
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize num = 0;
	bool foundit = false;
	jvxState stat = JVX_STATE_NONE;
	IjvxDevice* dev = NULL;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
		{
			res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->feature_class(ft);
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
			{
				res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->feature_class(ft);
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep->nodeInstances.theHandle_shortcut)
			{
				res = elmIt_ep->nodeInstances.theHandle_shortcut->feature_class(ft);
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE;
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	return(res);
}



/*
 * Select a component of a specific type
 */
jvxErrorType
CjvxHost::select_component(jvxComponentType tp, jvxSize idx)
{
	// Check that calling thread is correct
	JVX_HBDX1_CHECK_CALLING_THREAD_ID

	// Check that control has not been passed to sequencer
	JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState >= JVX_STATE_ACTIVE)
	{

		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;

			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech == NULL)
			{
				if(idx < elmIt_tech->technologyInstances.availableTechnologies.size())
				{
					res = elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle->initialize(this);
					if(res == JVX_NO_ERROR)
					{
						IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle);
						IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle);

#ifdef QT_ALLOW_QT_WIDGETS
						res = static_local_select(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
						res = static_local_select(theHinterface, theObjectSm);
#endif

						if(res == JVX_NO_ERROR)
						{
							elmIt_tech->technologyInstances.theHandle_shortcut_tech = elmIt_tech->technologyInstances.availableTechnologies[idx].theHandle;
						}
						else
						{
							res = JVX_ERROR_INTERNAL;
						}
					}
					else
					{
						res = JVX_ERROR_INTERNAL;
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
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				IjvxDevice* theNewDevice = NULL;
				foundit = true;

				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev == NULL)
				{
					if(elmIt_dev->technologyInstances.theHandle_shortcut_tech)
					{
						res = elmIt_dev->technologyInstances.theHandle_shortcut_tech->request_device(idx, &theNewDevice);
						if((res == JVX_NO_ERROR) && (theNewDevice))
						{
							res = theNewDevice->initialize(this);
							if(res == JVX_NO_ERROR)
							{
								IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(theNewDevice);
								IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(theNewDevice);

#ifdef QT_ALLOW_QT_WIDGETS
								res = static_local_select(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
								res = static_local_select(theHinterface, theObjectSm);
#endif

							}

							if(res == JVX_NO_ERROR)
							{
								elmIt_dev->technologyInstances.theHandle_shortcut_dev = theNewDevice;
							}
						}
						else
						{
							res = JVX_ERROR_INTERNAL;
						}
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					foundit = true;
					if(elmIt_ep->nodeInstances.theHandle_shortcut == NULL)
					{
						if(idx < elmIt_ep->nodeInstances.availableEndpoints.size())
						{
							res = elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle->initialize(this);
							if(res == JVX_NO_ERROR)
							{
								IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle);
								IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle);

#ifdef QT_ALLOW_QT_WIDGETS
								res = static_local_select(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
								res = static_local_select(theHinterface, theObjectSm);
#endif

								if(res == JVX_NO_ERROR)
								{
									elmIt_ep->nodeInstances.theHandle_shortcut = elmIt_ep->nodeInstances.availableEndpoints[idx].theHandle;
								}
								else
								{
									res = JVX_ERROR_INTERNAL;
								}
							}
							else
							{
								res = JVX_ERROR_INTERNAL;
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
				}
			}
		}

		if(!foundit)
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}

	return(res);
}

/**
 * Return the idx to identify the currently selected component
 */
jvxErrorType
CjvxHost::selection_component(jvxComponentType tp, jvxInt16* idRet)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize i;
	jvxInt16 id = -1;
	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;

		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech != NULL)
		{
			for(i = 0; i < elmIt_tech->technologyInstances.availableTechnologies.size(); i++)
			{
				if(elmIt_tech->technologyInstances.availableTechnologies[i].theHandle == elmIt_tech->technologyInstances.theHandle_shortcut_tech)
				{
					id = (jvxInt16)i;
					break;
				}
			}
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;

			if(elmIt_dev->technologyInstances.theHandle_shortcut_tech != NULL)
			{
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev != NULL)
				{
					jvxSize num = 0;
					elmIt_dev->technologyInstances.theHandle_shortcut_tech->number_devices(&num);
					for(i = 0; i < num; i++)
					{
						IjvxDevice* ptr = NULL;
						elmIt_dev->technologyInstances.theHandle_shortcut_tech->request_device(i, &ptr);
						if(ptr == elmIt_dev->technologyInstances.theHandle_shortcut_dev)
						{
							id = (jvxInt16)i;
							elmIt_dev->technologyInstances.theHandle_shortcut_tech->return_device(ptr);
							break;
						}
						else
						{
							elmIt_dev->technologyInstances.theHandle_shortcut_tech->return_device(ptr);
						}
					}
				}
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;

				if(elmIt_ep->nodeInstances.theHandle_shortcut != NULL)
				{
					for(i = 0; i < elmIt_ep->nodeInstances.availableEndpoints.size(); i++)
					{
						if(elmIt_ep->nodeInstances.availableEndpoints[i].theHandle == elmIt_ep->nodeInstances.theHandle_shortcut)
						{
							id = (jvxInt16)i;
							break;
						}
					}
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}

	if(idRet)
	{
		*idRet = id;
	}
	return(res);
}

/**
 * Activate a component of a specific type as currently selected.
 */
jvxErrorType
CjvxHost::activate_component(jvxComponentType tp)
{
	// Check that calling thread is correct
	JVX_HBDX1_CHECK_CALLING_THREAD_ID

	// Check that control has not been passed to sequencer
	JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState >= JVX_STATE_ACTIVE)
	{
		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->activate();
				if(res != JVX_NO_ERROR)
				{
					jvxError* theErr = NULL;
					jvxString* theName = NULL;
					elmIt_tech->technologyInstances.theHandle_shortcut_tech->description(&theName);
					elmIt_tech->technologyInstances.theHandle_shortcut_tech->lasterror(&theErr);

					if(theName && theErr)
					{
						this->_common_set.theErrordescr = (std::string)"Failed to activate technology type <" + elmIt_tech->description + ">, technology <" + theName->bString + ">, reason: " + theErr->errorDescription->bString;
					}
					else
					{
						if(theName)
						{
							this->_common_set.theErrordescr = (std::string)"Failed to activate technology type <" + elmIt_tech->description + ">, technology <" + theName->bString + ">, unknown reason.";
						}
						else
						{
							this->_common_set.theErrordescr = (std::string)"Failed to activate  type <" + elmIt_tech->description + ">, unknown reason.";
						}
					}
					if(	theName)
					{
						elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(theName);
					}
					if(theErr)
					{
						elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(theErr);
					}
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				foundit = true;
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->activate();
					if(res != JVX_NO_ERROR)
					{
						jvxError* theErr = NULL;
						jvxString* theName = NULL;
						elmIt_dev->technologyInstances.theHandle_shortcut_dev->description(&theName);
						elmIt_dev->technologyInstances.theHandle_shortcut_dev->lasterror(&theErr);

						if(theName && theErr)
						{
							this->_common_set.theErrordescr = (std::string)"Failed to activate device type <" + elmIt_dev->description + ">, device <" + theName->bString + ">, reason: " + theErr->errorDescription->bString;
						}
						else
						{
							if(theName)
							{
								this->_common_set.theErrordescr = (std::string)"Failed to activate device type <" + elmIt_dev->description + ">, device <" + theName->bString + ">, unknown reason.";
							}
							else
							{
								this->_common_set.theErrordescr = (std::string)"Failed to activate device type <" + elmIt_dev->description + ">, unknown reason.";
							}
						}
						if(	theName)
						{
							elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(theName);
						}
						if(theErr)
						{
							elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(theErr);
						}
					}
					/*
					else
					{
						_updateAllAfterStateChanged(tp, JVX_STATE_ACTIVE, reinterpret_cast<IjvxHost*>(this));
					}*/
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE_SUBMODULE;
				}
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					foundit = true;
					if(elmIt_ep->nodeInstances.theHandle_shortcut)
					{
						res = elmIt_ep->nodeInstances.theHandle_shortcut->activate();
						if(res != JVX_NO_ERROR)
						{
							jvxError* theErr = NULL;
							jvxString* theName = NULL;
							elmIt_ep->nodeInstances.theHandle_shortcut->description(&theName);
							elmIt_ep->nodeInstances.theHandle_shortcut->lasterror(&theErr);

							if(theName && theErr)
							{
								this->_common_set.theErrordescr = (std::string)"Failed to activate node type <" + elmIt_ep->description + ">, node <" + theName->bString + ">, reason: " + theErr->errorDescription->bString;
							}
							else
							{
								if(theName)
								{
									this->_common_set.theErrordescr = (std::string)"Failed to activate node type <" + elmIt_ep->description + ">, node <" + theName->bString + ">, unknown reason.";
								}
								else
								{
									this->_common_set.theErrordescr = (std::string)"Failed to activate node type <" + elmIt_ep->description + ">, unknown reason.";
								}
							}
							if(	theName)
							{
								elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(theName);
							}
							if(theErr)
							{
								elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(theErr);
							}
						}
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}


		if(!foundit)
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

jvxErrorType
CjvxHost::is_ready_component(jvxComponentType tp, jvxBool* ready, jvxString** reasonIfNot)
{
	jvxString * fldStr = NULL;
	std::string reason;
	// Check that calling thread is correct
	JVX_HBDX1_CHECK_CALLING_THREAD_ID

	// Check that control has not been passed to sequencer
	JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState >= JVX_STATE_ACTIVE)
	{
		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->is_ready(ready, &fldStr);
				assert(res == JVX_NO_ERROR);
				if(fldStr)
				{
					reason = fldStr->bString;
					elmIt_tech->technologyInstances.theHandle_shortcut_tech->deallocate(fldStr);
				}
			}
			else
			{
				if(ready)
				{
					*ready = false;
				}
				reason = "No technology selected!";
				//res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				foundit = true;
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->is_ready(ready, &fldStr);
					assert(res == JVX_NO_ERROR);
					if(fldStr)
					{
						reason = fldStr->bString;
						elmIt_dev->technologyInstances.theHandle_shortcut_dev->deallocate(fldStr);
					}
				}
				else
				{
					if(ready)
					{
						*ready = false;
					}
					//reason = "No device selected!";
					res = JVX_ERROR_WRONG_STATE_SUBMODULE;
				}
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					foundit = true;
					if(elmIt_ep->nodeInstances.theHandle_shortcut)
					{
						res = elmIt_ep->nodeInstances.theHandle_shortcut->is_ready(ready, &fldStr);
						assert(res == JVX_NO_ERROR);
						if(fldStr)
						{
							reason = fldStr->bString;
							elmIt_ep->nodeInstances.theHandle_shortcut->deallocate(fldStr);
						}
					}
					else
					{
						if(ready)
						{
							*ready = false;
						}
						reason = "No node selected!";
						//res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}


		if(!foundit)
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}

	if(!reason.empty())
	{
		HjvxObject_produceString(reasonIfNot, reason);
	}
	return(res);
}

/**
 * Return the state of the component currently selected.
 */
jvxErrorType
CjvxHost::state_component(jvxComponentType tp, jvxState* st)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxState stat = JVX_STATE_NONE;

	bool foundit = false;

	std::vector<oneTechnologyType>::iterator elmIt_tech;
	elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
	if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
	{
		foundit = true;
		if(elmIt_tech->technologyInstances.theHandle_shortcut_tech != NULL)
		{
			elmIt_tech->technologyInstances.theHandle_shortcut_tech->state(st);
		}
		else
		{
			if(st)
			{
				*st = JVX_STATE_NONE;
			}
		}
	}
	else
	{
		std::vector<oneTechnologyType>::iterator elmIt_dev;
		elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
		if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_dev->technologyInstances.theHandle_shortcut_dev != NULL)
			{
				elmIt_dev->technologyInstances.theHandle_shortcut_dev->state(st);
			}
			else
			{
				if(st)
				{
					*st = JVX_STATE_NONE;
				}
			}
		}
		else
		{
			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				foundit = true;
				if(elmIt_ep->nodeInstances.theHandle_shortcut != NULL)
				{
					elmIt_ep->nodeInstances.theHandle_shortcut->state(st);
				}
				else
				{
					if(st)
					{
						*st = JVX_STATE_NONE;
					}
				}
			}
		}
	}

	if(!foundit)
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}
	return(res);
}

/**
 * Deactivate a component of a specific type as currently selected.
 */
jvxErrorType
CjvxHost::deactivate_component(jvxComponentType tp)
{
	// Check that calling thread is correct
	JVX_HBDX1_CHECK_CALLING_THREAD_ID

	// Check that control has not been passed to sequencer
	JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{

		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				if(elmIt_tech->technologyInstances.theHandle_shortcut_dev)
				{
					res = this->deactivate_component(elmIt_tech->selector[1]);
					res = this->unselect_component(elmIt_tech->selector[1]);
/*					res = elmIt_tech->technologyInstances.theHandle_shortcut_dev->deactivate();
					if(res == JVX_NO_ERROR)
					{
						res = elmIt_tech->technologyInstances.theHandle_shortcut_dev->unselect();
						if(res == JVX_NO_ERROR)
						{
							res = elmIt_tech->technologyInstances.theHandle_shortcut_dev->terminate();
							if(res == JVX_NO_ERROR)
							{
								res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->return_device(
									elmIt_tech->technologyInstances.theHandle_shortcut_dev);
								elmIt_tech->technologyInstances.theHandle_shortcut_dev = NULL;
							}
						}
					}*/
					if(res != JVX_NO_ERROR)
					{
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
				}

				res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->deactivate();
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				foundit = true;
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->deactivate();
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE_SUBMODULE;
				}
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					foundit = true;
					if(elmIt_ep->nodeInstances.theHandle_shortcut)
					{
						res = elmIt_ep->nodeInstances.theHandle_shortcut->deactivate();
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}


		if(!foundit)
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

/**
 * Unselect a component of a specific type as currently selected.
 */
jvxErrorType
CjvxHost::unselect_component(jvxComponentType tp)
{
		// Check that calling thread is correct
	JVX_HBDX1_CHECK_CALLING_THREAD_ID

	// Check that control has not been passed to sequencer
	JVX_HBDX_CHECK_FREEZE_SEQUENCER_OPERATION

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			foundit = true;
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech != NULL)
			{
				IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);
				IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);

#ifdef QT_ALLOW_QT_WIDGETS
				res = static_local_unselect(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
				res = static_local_unselect(theHinterface, theObjectSm);
#endif

				if(res == JVX_NO_ERROR)
				{
					res = elmIt_tech->technologyInstances.theHandle_shortcut_tech->terminate();
					if(res == JVX_NO_ERROR)
					{
						elmIt_tech->technologyInstances.theHandle_shortcut_tech = NULL;
					}
				}
				if(res != JVX_NO_ERROR)
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_STATE_SUBMODULE;
			}
		}
		else
		{
			std::vector<oneTechnologyType>::iterator elmIt_dev;
			elmIt_dev = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 1);
			if(elmIt_dev != _common_set_types.registeredTechnologyTypes.end())
			{
				foundit = true;
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev != NULL)
				{
					IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);
					IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);

#ifdef QT_ALLOW_QT_WIDGETS
					res = static_local_unselect(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
					res = static_local_unselect(theHinterface, theObjectSm);
#endif

					if(res == JVX_NO_ERROR)
					{
						res = elmIt_dev->technologyInstances.theHandle_shortcut_dev->terminate();
						if(res == JVX_NO_ERROR)
						{
							elmIt_dev->technologyInstances.theHandle_shortcut_dev = NULL;
						}
					}
					if(res != JVX_NO_ERROR)
					{
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
				}
				else
				{
					res = JVX_ERROR_WRONG_STATE_SUBMODULE;
				}
			}
			else
			{
				std::vector<oneNodeType>::iterator elmIt_ep;
				elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

				if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
				{
					foundit = true;
					if(elmIt_ep->nodeInstances.theHandle_shortcut != NULL)
					{
						IjvxHiddenInterface* theHinterface = static_cast<IjvxHiddenInterface*>(elmIt_ep->nodeInstances.theHandle_shortcut);
						IjvxStateMachine* theObjectSm = static_cast<IjvxStateMachine*>(elmIt_ep->nodeInstances.theHandle_shortcut);

#ifdef QT_ALLOW_QT_WIDGETS
						res = static_local_unselect(theHinterface, theObjectSm, _common_set_host.theWidgetHost);
#else
						res = static_local_unselect(theHinterface, theObjectSm);
#endif

						if(res == JVX_NO_ERROR)
						{
							res = elmIt_ep->nodeInstances.theHandle_shortcut->terminate();
							if(res == JVX_NO_ERROR)
							{
								elmIt_ep->nodeInstances.theHandle_shortcut = NULL;
							}
						}
						if(res != JVX_NO_ERROR)
						{
							res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
						}
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}


		if(!foundit)
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}
