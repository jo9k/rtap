#include "jvx.h"

#include "templates/jvxHosts/CjvxHost.h"
#include <iostream>

// ===================================================================
// Some static helpers
// ===================================================================

/**
 * Check entries in list to detect existing entries
 */
static bool
isInList(std::vector<std::string> lst, std::string entry)
{
	jvxSize i;
	for(i = 0; i < lst.size(); i++)
	{
		if(lst[i] == entry)
		{
			return(true);
		}
	}
	return(false);
}

// ====================================================================
// Member functions: Allocate/deallocate object
// ====================================================================

/**
 * Constructor: Only those commands required on this class derivation level.
 */
CjvxHost::CjvxHost(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
	CjvxObject(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_HOST;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxHost*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);

	_common_set_host.reportOnConfig = NULL;
	_common_set_host.reportUnit = NULL;

	_common_set_sequencer.cb_thread_handler.callback_thread_startup = cb_startup;
	_common_set_sequencer.cb_thread_handler.callback_thread_timer_expired = cb_period_expired;
	_common_set_sequencer.cb_thread_handler.callback_thread_wokeup = cb_wokeup;
	_common_set_sequencer.cb_thread_handler.callback_thread_stopped = cb_stopped;

	reset();


}

/**
 * Destructor: Only terminate call
 */
CjvxHost::~CjvxHost()
{
	this->terminate();
}

/**
 * Reset function to reset all member variable members
 */
void
CjvxHost::reset()
{
	// technologies
//	_common_set_host.scannerTechnologies.theHandle_shortcut_tech = NULL;
//	_common_set_host.scannerTechnologies.theHandle_shortcut_dev = NULL;
//	_common_set_host.lampTechnologies.theHandle_shortcut_tech = NULL;
//	_common_set_host.lampTechnologies.theHandle_shortcut_dev = NULL;
//	_common_set_host.spectrumProcessorTechnologies.theHandle_shortcut_tech = NULL;
//	_common_set_host.spectrumProcessorTechnologies.theHandle_shortcut_dev = NULL;
//	_common_set_host.spectrumProcessorTechnologies.theHandle_shortcut_tech = NULL;
//	_common_set_host.spectrumProcessorTechnologies.theHandle_shortcut_dev = NULL;

	// simple copmponents
//	_common_set_host.ascanEndpoints.theHandle_shortcut = NULL;

	_common_set_host.reportUnit = NULL;
	_common_set_host.doUnload = false;
}

// ====================================================================
// Member functions: State transitions
// ====================================================================

/*
 * Initialize the current component
 */
jvxErrorType
CjvxHost::initialize(IjvxHiddenInterface* theOtherhost)
{
	jvxErrorType res = _initialize(theOtherhost);
	if(res == JVX_NO_ERROR)
	{
		_common_set_host.threadId_registered = JVX_GET_CURRENT_THREAD_ID();

		reset();
	}
	return(res);
}

jvxErrorType
CjvxHost::select()
{
	jvxErrorType res = _select();
	if(res == JVX_NO_ERROR)
	{
		this->allocate__properties_selected();
		this->register__properties_selected(static_cast<CjvxProperties*>(this));
	}
	return(res);
}

/*
 * Activate host component
 */
jvxErrorType
CjvxHost::activate()
{
	jvxErrorType res = _activate();
	if(res == JVX_NO_ERROR)
	{
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
		this->loadAllComponents();
#endif
	}

	return(res);
}

jvxErrorType
CjvxHost::is_ready(jvxBool* suc, jvxString** reasonIfNot)
{
	jvxBool canStart = true;
	jvxBool goLocal = true;
	jvxState stat = JVX_STATE_NONE;
	jvxString* reasonLoc = NULL;
	std::string errReason;

	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
#if 0
		// ===============================================
		// Check with scanner technology
		// ===============================================
		goLocal = false;
		if(_common_set_host.scannerTechnologies.theHandle_shortcut_tech)
		{
			stat = JVX_STATE_NONE;
			_common_set_host.scannerTechnologies.theHandle_shortcut_tech->state(&stat);
			if(stat == JVX_STATE_ACTIVE)
			{
				if(_common_set_host.scannerTechnologies.theHandle_shortcut_dev)
				{
					stat = JVX_STATE_NONE;
					_common_set_host.scannerTechnologies.theHandle_shortcut_dev->state(&stat);
					if(stat == JVX_STATE_ACTIVE)
					{
						reasonLoc = NULL;
						_common_set_host.scannerTechnologies.theHandle_shortcut_dev->is_ready(&goLocal, &reasonLoc);
						if(reasonLoc)
						{
							errReason = reasonLoc->bString;
							static_cast<IjvxObject*>(_common_set_host.scannerTechnologies.theHandle_shortcut_dev)->deallocate(reasonLoc);
						}
						reasonLoc = NULL;

						if(!goLocal)
						{
							errReason = "Submodule scanner device not ready, reason: " + errReason;
						}
					}
					else
					{
						errReason = "Submodule scanner device not in active state.";
					}
				}
				else
				{
					errReason = "No submodule scanner device selected.";
				}
			}
			else
			{
				errReason = "Submodule scanner technology not in active state.";
			}
		}
		else
		{
			errReason = "No submodule scanner technology selected.";
		}
		canStart = canStart && goLocal;
#endif

		// ===============================================
	}
	else
	{
		errReason = "Host is not in the right state.";
		canStart = false;
	}

	if(suc)
	{
		*suc = canStart;
	}
	return(JVX_NO_ERROR);
}


/*
jvxErrorType
CjvxHost::process()
{
	if(_common_set.theState == JVX_PROCESSING)
	{
		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_WRONG_STATE);
}
*/

/*
 * Deactivate host component
 */
jvxErrorType
CjvxHost::deactivate()
{
	jvxSize i;
	jvxErrorType res = JVX_ERROR_WRONG_STATE;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		for(i = 0; i < _common_set_types.registeredTechnologyTypes.size(); i++)
		{
			this->unselect_component(_common_set_types.registeredTechnologyTypes[i].selector[1]);
			this->unselect_component(_common_set_types.registeredTechnologyTypes[i].selector[0]);
		}

		for(i = 0; i < _common_set_types.registeredNodeTypes.size(); i++)
		{
			this->unselect_component(_common_set_types.registeredNodeTypes[i].selector[0]);
		}

		res = _deactivate();
		assert(res == JVX_NO_ERROR);

		// unload all Dlls
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
		this->unloadAllComponents();
#endif
	}

	return(res);
}

jvxErrorType
CjvxHost::unselect()
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		unregister__properties_selected(static_cast<CjvxProperties*>(this));
		deallocate__properties_selected();

		res = _unselect();
		assert(res == JVX_NO_ERROR);
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

/*
 * Terminate host component
 */
jvxErrorType
CjvxHost::terminate()
{
	jvxErrorType res = JVX_NO_ERROR;
	res = _terminate();
	return(res);
}

// ====================================================================
// Member functions: Configuration
// ====================================================================

/*
 * Function to add an external component of a specific type (manually)
 */
jvxErrorType
CjvxHost::add_external_component(IjvxObject* theObj, const char* locationDescription, jvxBool allowMultipleInstance, jvxInitObject_tp funcInit, jvxTerminateObject_tp funcTerm)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		if(theObj)
		{
			jvxComponentType tp = JVX_COMPONENT_UNKNOWN;
			jvxComponentType tpRet = JVX_COMPONENT_UNKNOWN;
			jvxBool allowsMultiObjects = false;
			jvxHandle* specComp = NULL;
			theObj->request_specialization(&specComp, &tp, &allowsMultiObjects);
			
			// Overwrite what comes form dll
			if(allowMultipleInstance == false)
			{
				allowsMultiObjects = false;
			}

			std::vector<oneTechnologyType>::iterator elmIt_tech;
			elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);

			std::vector<oneNodeType>::iterator elmIt_ep;
			elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

			if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
			{
				oneTechnology elm;
				res = theObj->request_specialization(reinterpret_cast<jvxHandle**>(&elm.theHandle), &tpRet, NULL);
				if(res == JVX_NO_ERROR)
				{
					if(elm.theHandle)
					{
						if(tp == tpRet)
						{
							elm.common.isExternalComponent = true;
							elm.common.id = (jvxSize)elmIt_tech->technologyInstances.availableTechnologies.size();
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
							elm.common.linkage.dllPath = "NONE";
							elm.common.linkage.sharedObjectEntry = NULL;
#endif
							elm.common.linkage.funcInit = funcInit;
							elm.common.linkage.funcTerm = funcTerm;
							elm.common.tp = tp;
							elm.common.allowsMultiObjects = allowsMultiObjects;
							elm.common.externalLink.description = locationDescription;
							elm.common.hObject = theObj;
							elm.common.dllProps = 0;

							elmIt_tech->technologyInstances.availableTechnologies.push_back(elm);
						}
						else
						{
							res = JVX_ERROR_UNEXPECTED;
						}
					}
					else
					{
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			else if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
			{
				oneNode elm;
				res = theObj->request_specialization(reinterpret_cast<jvxHandle**>(&elm.theHandle), &tpRet, NULL);
				if(res == JVX_NO_ERROR)
				{
					if(elm.theHandle)
					{
						if(tp == tpRet)
						{
							elm.common.isExternalComponent = true;
							elm.common.id = (jvxSize)elmIt_ep->nodeInstances.availableEndpoints.size();
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
							elm.common.linkage.dllPath = "NONE";
							elm.common.linkage.sharedObjectEntry = NULL;
#endif
							elm.common.linkage.funcInit = funcInit;
							elm.common.linkage.funcTerm = funcTerm;
							elm.common.tp = tp;
							elm.common.allowsMultiObjects = allowsMultiObjects;
							elm.common.externalLink.description = locationDescription;
							elm.common.hObject = theObj;
							elm.common.dllProps = 0;
							elmIt_ep->nodeInstances.availableEndpoints.push_back(elm);
						}
						else
						{
							res = JVX_ERROR_UNEXPECTED;
						}
					}
					else
					{
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
				}
			}
			else
			{
				// All other components
				oneOtherComponent elm;

				elm.theHandle = theObj;
				elm.common.isExternalComponent = true;
				elm.common.id = (jvxSize)_common_set_host.otherComponents.availableOtherComponents.size();
#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
				elm.common.linkage.dllPath = "NONE";
				elm.common.linkage.sharedObjectEntry = NULL;
#endif
				elm.common.linkage.funcInit = funcInit;
				elm.common.linkage.funcTerm = funcTerm;
				elm.common.tp = tp;
				elm.common.allowsMultiObjects = allowsMultiObjects;
				elm.common.externalLink.description = locationDescription;
				elm.common.dllProps = 0;
				elm.common.hObject = theObj;

				_common_set_host.otherComponents.availableOtherComponents.push_back(elm);
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
}

jvxErrorType
CjvxHost::remove_external_component(IjvxObject* theObj)
{
	jvxSize i, j;
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		for(i = 0; i < _common_set_types.registeredTechnologyTypes.size(); i++)
		{
			std::vector<oneTechnology>::iterator elm = _common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.begin();
			for(; elm != _common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.end(); elm++)
			{
				if(elm->common.hObject == theObj)
				{
					break;
				}
			}
			if(elm != _common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.end())
			{
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.erase(elm);
			}
		}

		for(i = 0; i < _common_set_types.registeredNodeTypes.size(); i++)
		{
			std::vector<oneNode>::iterator elm = _common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.begin();
			for(; elm != _common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.end(); elm++)
			{
				if(elm->common.hObject == theObj)
				{
					break;
				}
			}
			if(elm != _common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.end())
			{
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.erase(elm);
			}
		}

		std::vector<oneOtherComponent>::iterator elm = this->_common_set_host.otherComponents.availableOtherComponents.begin();
		for(; elm != this->_common_set_host.otherComponents.availableOtherComponents.end(); elm++)
		{
			if(elm->common.hObject == theObj)
			{
				break;
			}
		}
		if(elm != this->_common_set_host.otherComponents.availableOtherComponents.end())
		{
			this->_common_set_host.otherComponents.availableOtherComponents.erase(elm);
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

jvxErrorType
CjvxHost::set_external_report_target(IjvxReport* theHdl)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		_common_set_host.reportUnit = theHdl;
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

jvxErrorType
CjvxHost::set_external_report_on_config(IjvxReportOnConfig* theHdl)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		_common_set_host.reportOnConfig = theHdl;
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}
// ============================================================================================

jvxErrorType
CjvxHost::add_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize i = 0;
	std::vector<oneInterface>::iterator elmIt_ep;

	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		elmIt_ep = jvx_findItemSelectorInList<oneInterface, jvxInterfaceType>(_common_set_host.externalInterfaces, theIFacetype, 0);
		if(elmIt_ep == _common_set_host.externalInterfaces.end())
		{
			oneInterface newI;
			newI.selector[0] = theIFacetype;
			newI.theHdl = theHdl;
			_common_set_host.externalInterfaces.push_back(newI);

		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

jvxErrorType
CjvxHost::remove_external_interface(jvxHandle* theHdl, jvxInterfaceType theIFacetype)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize i = 0;
	std::vector<oneInterface>::iterator elmIt_ep;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		elmIt_ep = jvx_findItemSelectorInList<oneInterface, jvxInterfaceType>(_common_set_host.externalInterfaces, theIFacetype, 0);
		if(elmIt_ep == _common_set_host.externalInterfaces.end())
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;

		}
		else
		{
			if(elmIt_ep->theHdl != theHdl)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			else
			{
				_common_set_host.externalInterfaces.erase(elmIt_ep);
			}
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

// ============================================================================================

jvxErrorType
CjvxHost::add_component_load_blacklist(jvxComponentType theTp)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize i = 0;
	std::vector<oneBlacklistEntry>::iterator elmIt_ep;

	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		elmIt_ep = jvx_findItemSelectorInList<oneBlacklistEntry, jvxComponentType>(_common_set_host.blacklist_load, theTp, 0);
		if(elmIt_ep == _common_set_host.blacklist_load.end())
		{
			oneBlacklistEntry newE;
			newE.selector[0] = theTp;
			_common_set_host.blacklist_load.push_back(newE);

		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

jvxErrorType
CjvxHost::remove_component_load_blacklist(jvxComponentType theTp)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize i = 0;
	std::vector<oneBlacklistEntry>::iterator elmIt_ep;
	if(_common_set.theState == JVX_STATE_SELECTED)
	{
		elmIt_ep = jvx_findItemSelectorInList<oneBlacklistEntry, jvxComponentType>(_common_set_host.blacklist_load, theTp, 0);
		if(elmIt_ep == _common_set_host.blacklist_load.end())
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		else
		{
			_common_set_host.blacklist_load.erase(elmIt_ep);
		}
	}
	else
	{
		res = JVX_ERROR_WRONG_STATE;
	}
	return(res);
}

// ============================================================================================

jvxErrorType
CjvxHost::set_external_widget_host(jvxHandle* theWidgetHost, jvxWidgetHostType widgetType)
{
	jvxErrorType res = JVX_NO_ERROR;
#ifdef QT_ALLOW_QT_WIDGETS
	if(widgetType == JVX_WIDGET_HOST_QT)
	{
		if(_common_set.theState == JVX_STATE_SELECTED)
		{
			_common_set_host.theWidgetHost = (IjvxQtWidgetHost*)theWidgetHost;
		}
		else
		{
			res = JVX_ERROR_WRONG_STATE;
		}
	}
	else
#endif
	{
		res = JVX_ERROR_UNSUPPORTED;
	}
	return(res);
}

// ================================================================================
// Helper function
// ================================================================================

#ifndef JVX_HOST_USE_ONLY_STATIC_OBJECTS
jvxErrorType
CjvxHost::loadAllComponents()
{
	jvxSize i,ii;
	JVX_HANDLE_DIR searchHandle;
	JVX_DIR_FIND_DATA searchResult = INIT_SEARCH_DIR_DEFAULT;;
	JVX_HMODULE dllHandle;
	std::string fileName;
	if(CjvxHost_genpcg::properties_selected.do_unload_dlls.value.selection & 0x1)
	{

		for(i = 0; i < CjvxHost_genpcg::properties_selected.component_path.value.size(); i ++)
		{
			std::string pathName = CjvxHost_genpcg::properties_selected.component_path.value[i];

			// Search for directory
			searchHandle = JVX_FINDFIRSTFILE_WC(searchHandle, pathName.c_str(), JVX_DLL_EXTENSION, searchResult);
			bool result = true;
			bool foundElement = false;

			// If entry exists
			if (searchHandle != INVALID_HANDLE_VALUE )
			{
				// open DLL and test for technology or plugin
				do
				{
					fileName  = pathName;
					fileName += JVX_SEPARATOR_DIR;
					fileName += JVX_GETFILENAMEDIR(searchResult);//searchResult.cFileName;

					result = false;
					foundElement = false;

					std::cout << "found lib "<< fileName << std::endl;

					// Open the DLL
					if ((dllHandle = JVX_LOADLIBRARY( fileName.c_str())))
					{
						IjvxObject* newObj = NULL;
						jvxInitObject_tp funcInit = NULL;
						jvxTerminateObject_tp funcTerm = NULL;
						jvxDllProperties_tp funcDllsProps = NULL;
						jvxComponentType tp = JVX_COMPONENT_UNKNOWN;
						jvxBool allowMultipleObjects = false;
						jvxHandle* specComp = NULL;
						jvxBitField dllProps = 0;

						funcInit = (jvxInitObject_tp)JVX_GETPROCADDRESS(dllHandle, JVX_OPEN_OBJECT_DLL);
						funcTerm = (jvxTerminateObject_tp)JVX_GETPROCADDRESS(dllHandle, JVX_CLOSE_OBJECT_DLL);
						funcDllsProps= (jvxDllProperties_tp)JVX_GETPROCADDRESS(dllHandle, JVX_PROPERTIES_DLL);

						if(funcInit && funcTerm)
						{
							std::cout<<"  loading lib "<<fileName<<std::endl;
							funcInit(&newObj);
							if(newObj)
							{
								funcDllsProps(&dllProps);
								newObj->request_specialization(&specComp, &tp, &allowMultipleObjects);

								bool isOnBlacklist = false;
								for(ii = 0; ii < _common_set_host.blacklist_load.size(); ii++)
								{
									if(tp == _common_set_host.blacklist_load[ii].selector[0])
									{
										isOnBlacklist = true;
										break;
									}
								}

								if(isOnBlacklist)
								{
									std::cout<<"  blacklisted lib "<<fileName<<std::endl;
									// Do not store this component type
									funcTerm(newObj);
									JVX_UNLOADLIBRARY(dllHandle);
								}
								else
								{
									if(specComp)
									{

										std::vector<oneTechnologyType>::iterator elmIt_tech;
										elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
										std::vector<oneNodeType>::iterator elmIt_ep;
										elmIt_ep = jvx_findItemSelectorInList<oneNodeType, jvxComponentType>(_common_set_types.registeredNodeTypes, tp, 0);

										if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
										{
											oneTechnology elm;
											elm.theHandle = reinterpret_cast<IjvxTechnology*>(specComp);
											elm.common.isExternalComponent = false;
											elm.common.id = (jvxSize)elmIt_tech->technologyInstances.availableTechnologies.size();
											elm.common.tp = tp;
											elm.common.allowsMultiObjects = allowMultipleObjects;
											elm.common.hObject = newObj;

											elm.common.linkage.dllPath = fileName;
											elm.common.linkage.funcInit = funcInit;
											elm.common.linkage.funcTerm = funcTerm;
											elm.common.linkage.sharedObjectEntry = dllHandle;

											elm.common.externalLink.description = "NONE";
											elm.common.dllProps = dllProps;

											elmIt_tech->technologyInstances.availableTechnologies.push_back(elm);
										}
										else if(elmIt_ep != _common_set_types.registeredNodeTypes.end())
										{
											oneNode elm;
											elm.theHandle = reinterpret_cast<IjvxNode*>(specComp);
											elm.common.isExternalComponent = false;
											elm.common.id = (jvxSize)elmIt_ep->nodeInstances.availableEndpoints.size();
											elm.common.tp = tp;
											elm.common.allowsMultiObjects = allowMultipleObjects;
											elm.common.hObject = newObj;

											elm.common.linkage.dllPath = fileName;
											elm.common.linkage.funcInit = funcInit;
											elm.common.linkage.funcTerm = funcTerm;
											elm.common.linkage.sharedObjectEntry = dllHandle;

											elm.common.externalLink.description = "NONE";
											elm.common.dllProps = dllProps;
											elmIt_ep->nodeInstances.availableEndpoints.push_back(elm);
										}
										else
										{
											oneOtherComponent elm;

											elm.common.isExternalComponent = false;
											elm.common.id = (jvxSize)_common_set_host.otherComponents.availableOtherComponents.size();
											elm.common.tp = tp;
											elm.common.allowsMultiObjects = allowMultipleObjects;
											elm.common.hObject = newObj;

											elm.common.linkage.dllPath = fileName;
											elm.common.linkage.funcInit = funcInit;
											elm.common.linkage.funcTerm = funcTerm;
											elm.common.linkage.sharedObjectEntry = dllHandle;

											elm.common.externalLink.description = "NONE";
											elm.common.dllProps = dllProps;
											elm.theHandle = newObj;

											_common_set_host.otherComponents.availableOtherComponents.push_back(elm);
										}
										std::cout << "  added lib " << fileName << std::endl;
									}//if(specComp)
								} // if(isOnBlacklist) else

							}//if(newObj)
						}//if(funcInit && funcTerm)
						else
						{
							std::cout << "! invalid lib " << fileName << std::endl;
						}
					} //if ((dllHandle = JVX_LOADLIBRARY( fileName.c_str())))
					else
					{
						std::cout << "! error opening lib " << fileName << std::endl;
					}
				} while (JVX_FINDNEXTFILE(searchHandle, searchResult, JVX_DLL_EXTENSION));//FindNextFile( searchHandle, &searchResult )
				JVX_FINDCLOSE(searchHandle);//FindClose( searchHandle );
			}//if (searchHandle != INVALID_HANDLE_VALUE )
		}
	}
	return(JVX_NO_ERROR);
}

jvxErrorType
CjvxHost::unloadAllComponents()
{
	jvxSize i, j;

	for(i = 0; i < _common_set_types.registeredTechnologyTypes.size(); i++)
	{
		std::vector<oneTechnology> remainListT;
		for(j = 0; j < _common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.size(); j++)
		{

			if(_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.isExternalComponent)
			{
				remainListT.push_back(_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j]);
			}
			else
			{
				// In all other cases, unload library and delete object
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.linkage.funcTerm(
					_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.hObject);
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.hObject = NULL;
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].theHandle = NULL;
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.linkage.funcInit = NULL;
				_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.linkage.funcTerm = NULL;
				if(!(_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.dllProps & JVX_BITFIELD_DLL_PROPERTY_DO_NOT_UNLOAD))
				{
					JVX_UNLOADLIBRARY(_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies[j].common.linkage.sharedObjectEntry);
				}
			}
		}
		_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies.clear();
		_common_set_types.registeredTechnologyTypes[i].technologyInstances.availableTechnologies = remainListT;
	}

	for(i = 0; i < _common_set_types.registeredNodeTypes.size(); i++)
	{
		std::vector<oneNode> remainListN;
		for(j = 0; j < _common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.size(); j++)
		{

			if(_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.isExternalComponent)
			{
				remainListN.push_back(_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j]);
			}
			else
			{
				// In all other cases, unload library and delete object
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.linkage.funcTerm(
					_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.hObject);
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.hObject = NULL;
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].theHandle = NULL;
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.linkage.funcInit = NULL;
				_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.linkage.funcTerm = NULL;
				if(!(_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.dllProps & JVX_BITFIELD_DLL_PROPERTY_DO_NOT_UNLOAD))
				{
					JVX_UNLOADLIBRARY(_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints[j].common.linkage.sharedObjectEntry);
				}
			}
		}
		_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints.clear();
		_common_set_types.registeredNodeTypes[i].nodeInstances.availableEndpoints = remainListN;
	}

	std::vector<oneOtherComponent> remainListO;
	for(i = 0; i < this->_common_set_host.otherComponents.availableOtherComponents.size();i++)
	{
		if(this->_common_set_host.otherComponents.availableOtherComponents[i].common.isExternalComponent)
		{
			remainListO.push_back(this->_common_set_host.otherComponents.availableOtherComponents[i]);
		}
		else
		{
			// In all other cases, unload library and delete object
			this->_common_set_host.otherComponents.availableOtherComponents[i].common.linkage.funcTerm(
				this->_common_set_host.otherComponents.availableOtherComponents[i].common.hObject);
			this->_common_set_host.otherComponents.availableOtherComponents[i].common.hObject = NULL;
			this->_common_set_host.otherComponents.availableOtherComponents[i].theHandle = NULL;
			this->_common_set_host.otherComponents.availableOtherComponents[i].common.linkage.funcInit = NULL;
			this->_common_set_host.otherComponents.availableOtherComponents[i].common.linkage.funcTerm = NULL;
			if(!(this->_common_set_host.otherComponents.availableOtherComponents[i].common.dllProps & JVX_BITFIELD_DLL_PROPERTY_DO_NOT_UNLOAD))
			{
				JVX_UNLOADLIBRARY(this->_common_set_host.otherComponents.availableOtherComponents[i].common.linkage.sharedObjectEntry);
			}
		}
	}
	this->_common_set_host.otherComponents.availableOtherComponents.clear();
	this->_common_set_host.otherComponents.availableOtherComponents = remainListO;

	return(JVX_NO_ERROR);
}
#endif


jvxErrorType
CjvxHost::request_hidden_interface(jvxInterfaceType tp, jvxHandle** hdl)
{
	jvxErrorType res = JVX_NO_ERROR;

	std::vector<oneInterface>::iterator elmIt_ep;
	elmIt_ep = jvx_findItemSelectorInList<oneInterface, jvxInterfaceType>(_common_set_host.externalInterfaces, tp, 0);
	if(elmIt_ep != _common_set_host.externalInterfaces.end())
	{
		if(hdl)
		{
			*hdl = elmIt_ep->theHdl;
		}
	}
	else
	{
		switch(tp)
		{
		case JVX_INTERFACE_REPORT:
			if(hdl)
			{
				*hdl = reinterpret_cast<jvxHandle*>(_common_set_host.reportUnit);
			}
			break;
		case JVX_INTERFACE_PROPERTIES:
			if(hdl)
			{
				*hdl = reinterpret_cast<jvxHandle*>(static_cast<IjvxProperties*>(this));
			}
			break;
		case JVX_INTERFACE_SEQUENCER:
			if(hdl)
			{
				*hdl = static_cast<IjvxSequencer*>(this);
			}
			break;
		case JVX_INTERFACE_HOSTTYPEHANDLER:
			if(hdl)
			{
				*hdl = static_cast<IjvxHostTypeHandler*>(this);
			}
			break;

#ifdef QT_ALLOW_QT_WIDGETS
		case JVX_INTERFACE_QT_WIDGET_HOST:
			if(_common_set_host.theWidgetHost)
			{
				if(hdl)
				{
					*hdl = _common_set_host.theWidgetHost;
				}
			}
			else
			{
				res = JVX_ERROR_UNSUPPORTED;
			}
			break;
#endif
		default:

			res = _request_hidden_interface(tp, hdl);
		}
	}
	return(res);
}

jvxErrorType
CjvxHost::return_hidden_interface(jvxInterfaceType tp, jvxHandle* hdl)
{
	jvxErrorType res = JVX_NO_ERROR;

	std::vector<oneInterface>::iterator elmIt_ep;
	elmIt_ep = jvx_findItemSelectorInList<oneInterface, jvxInterfaceType>(_common_set_host.externalInterfaces, tp, 0);
	if(elmIt_ep != _common_set_host.externalInterfaces.end())
	{
		if(hdl == elmIt_ep->theHdl)
		{
			res = JVX_NO_ERROR;
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
	}
	else
	{
		switch(tp)
		{
		case JVX_INTERFACE_REPORT:
			if(hdl == _common_set_host.reportUnit)
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
		case JVX_INTERFACE_PROPERTIES:
			if(hdl == reinterpret_cast<jvxHandle*>(static_cast<IjvxProperties*>(this)))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;
		case JVX_INTERFACE_SEQUENCER:
			if(hdl == static_cast<IjvxSequencer*>(this))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;

		case JVX_INTERFACE_HOSTTYPEHANDLER:
			if(hdl == static_cast<IjvxHostTypeHandler*>(this))
			{
				res = JVX_NO_ERROR;
			}
			else
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			break;

#ifdef QT_ALLOW_QT_WIDGETS
		case JVX_INTERFACE_QT_WIDGET_HOST:
			if(_common_set_host.theWidgetHost)
			{
				if(hdl == _common_set_host.theWidgetHost)
				{
					res = JVX_NO_ERROR;
				}
				else
				{
					res = JVX_ERROR_INVALID_ARGUMENT;
				}
			}
			else
			{
				res = JVX_ERROR_UNSUPPORTED;
			}
			break;
#endif

		default:
			res = _return_hidden_interface(tp, hdl);
		}
	}
	return(res);
}

jvxErrorType
CjvxHost::request_hidden_interface_component(jvxComponentType tp, jvxInterfaceType ifTp, jvxHandle** iface)
{
	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		IjvxHiddenInterface* theIface = NULL;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				theIface = static_cast<IjvxHiddenInterface*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);
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
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					theIface = static_cast<IjvxHiddenInterface*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);
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
						theIface = static_cast<IjvxHiddenInterface*>(elmIt_ep->nodeInstances.theHandle_shortcut);
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}

		if((res == JVX_NO_ERROR) && theIface)
		{
			res = theIface->request_hidden_interface(ifTp, iface);
			if(res != JVX_NO_ERROR)
			{
				res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
			}
		}
		else
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
CjvxHost::return_hidden_interface_component(jvxComponentType tp, jvxInterfaceType ifTp, jvxHandle* iface)
{

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		IjvxHiddenInterface* theIface = NULL;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				theIface = static_cast<IjvxHiddenInterface*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);
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
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					theIface = static_cast<IjvxHiddenInterface*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);
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
						theIface = static_cast<IjvxHiddenInterface*>(elmIt_ep->nodeInstances.theHandle_shortcut);
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}

		if((res == JVX_NO_ERROR) && theIface)
		{
			res = theIface->return_hidden_interface(ifTp, iface);
			if(res != JVX_NO_ERROR)
			{
				res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
			}
		}
		else
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
CjvxHost::request_object_component(jvxComponentType tp, IjvxObject** theObj)
{

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		bool foundit = false;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				if(theObj)
				{
					*theObj = static_cast<IjvxObject*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);
				}
				foundit = true;
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
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					if(theObj)
					{
						*theObj = static_cast<IjvxObject*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);
					}
					foundit = true;
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
						if(theObj)
						{
							*theObj = static_cast<IjvxObject*>(elmIt_ep->nodeInstances.theHandle_shortcut);
						}
						foundit = true;
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
CjvxHost::return_object_component(jvxComponentType tp, IjvxObject* theObj)
{

	jvxErrorType res = JVX_NO_ERROR;
	if(_common_set.theState == JVX_STATE_ACTIVE)
	{
		IjvxObject* theObjComp = NULL;

		std::vector<oneTechnologyType>::iterator elmIt_tech;
		elmIt_tech = jvx_findItemSelectorInList<oneTechnologyType, jvxComponentType>(_common_set_types.registeredTechnologyTypes, tp, 0);
		if(elmIt_tech != _common_set_types.registeredTechnologyTypes.end())
		{
			if(elmIt_tech->technologyInstances.theHandle_shortcut_tech)
			{
				theObjComp = static_cast<IjvxObject*>(elmIt_tech->technologyInstances.theHandle_shortcut_tech);
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
				if(elmIt_dev->technologyInstances.theHandle_shortcut_dev)
				{
					theObjComp = static_cast<IjvxObject*>(elmIt_dev->technologyInstances.theHandle_shortcut_dev);
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
						theObjComp = static_cast<IjvxObject*>(elmIt_ep->nodeInstances.theHandle_shortcut);
					}
					else
					{
						res = JVX_ERROR_WRONG_STATE_SUBMODULE;
					}
				}
			}
		}

		if((res == JVX_NO_ERROR) && theObjComp)
		{
			if(theObjComp != theObj)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
		}
		else
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
