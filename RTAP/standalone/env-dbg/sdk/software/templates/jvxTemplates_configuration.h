#ifndef __JVXTEMPLATES_CONFIGURATION_H__
#define __JVXTEMPLATES_CONFIGURATION_H__

/*
 Wrapping typedefs for technologies must be like:

typedef struct
{
	oneComponentWrap common;
	IjvxCameraTechnology* theHandle;
} oneCameraTechnology;

typedef struct
{
	std::vector<oneCameraTechnology> availableCameraTechnologies;
	IjvxCameraTechnology* theHandle_shortcut_tech;
	IjvxCameraDevice* theHandle_shortcut_dev;
} cameraTechnologiesT;

*/

template<typename T1, typename T2> static void put_configuration_technology(T1& theStruct, T2& theList, 
																				   IjvxConfigProcessor* processor, 
																				   jvxHandle* datTmp, 
																				   const char* filename, jvxInt32 lineno, 
																				   std::string tokena,
																				   std::string tokenb,
																				   std::string token1,
																				   std::string token2,
																				   std::string tokenss1,
																				   std::string tokenss2,
																				   jvxComponentType tp1, 
																				   jvxComponentType tp2, 
																				   std::string cType1,
																				   std::string cType2,
																				   IjvxReport* report, 
																				   IjvxReportOnConfig* reportOnConfig, 
																				   IjvxHost* this_pointer)
{
	jvxSize i;
	IjvxConfiguration* config_local = NULL;
	jvxData valD;
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize targetStateIdx = 0;
	jvxString* fldStr = NULL;
	std::string entry;
	std::string desc;
	jvxBool wantToContinue = true;
	jvxInt16 selectme = -1;
	jvxSize num = 0;
	// Step I: Get the desired state from configuration
	targetStateIdx = 0;
	jvxHandle* datTmp_ss = NULL;

	res = HjvxConfigProcessor_readEntry_assignmentData(processor, datTmp, tokena, &valD);
	if(res == JVX_NO_ERROR)
	{
		targetStateIdx = JVX_DATA2INT16(valD);
	}
	else
	{
		REPORT_ERROR(((std::string)"Failed to obtain entry " + token1 + " from config file " + filename + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
		wantToContinue = false;
	}

	if(wantToContinue)
	{
		// Check state to see what needs to be done.
		jvxInt16 idxC = jvx_stateToIndex(JVX_STATE_SELECTED);
		assert(idxC >= 0);
		if(targetStateIdx >= (jvxSize)idxC)
		{
			// if selection is desired, go and get the name of the selected component
			res = HjvxConfigProcessor_readEntry_assignmentString(processor, datTmp, token1, &entry);
			if(res == JVX_NO_ERROR)
			{
				// Find the selection id for the desired component
				selectme = -1;
				for(i = 0; i < theList.size(); i++)
				{
					fldStr = NULL;
					desc = "";
					theList[i].common.hObject->description(&fldStr);
					if(fldStr)
					{
						desc = fldStr->bString;
						theList[i].common.hObject->deallocate(fldStr);
					}

					if(entry == desc)
					{
						selectme = (jvxInt16)i;
						break;
					}
				}

				// Step II: If selection is valid, select technology
				if(selectme >= 0)
				{
					res = this_pointer->select_component(tp1, selectme);
					if( res != JVX_NO_ERROR)
					{
						REPORT_ERROR(((std::string)"Unexpected error when selecting component " + cType1 + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
						wantToContinue = false;
					}
				}
				else
				{
					REPORT_ERROR(((std::string)"Failed to find component with description " + entry + " on target machine.").c_str(), JVX_REPORT_PRIORITY_WARNING);
					wantToContinue = false;
				}
			}
			else
			{
				REPORT_ERROR(((std::string)"Failed to find component description entry for symbol " + token1 + " in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
				wantToContinue = false;
			}
		}
		else
		{
			// No selection is desired
			wantToContinue = false;
		}

		// If selection was successful, next, pass config entries to component to be read in state selected. Afterwards, activate
		if(wantToContinue)
		{
			// Get configuration interface and pass configuration
			datTmp_ss = NULL;
			config_local = NULL;
			theStruct.theHandle_shortcut_tech->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);

			if(config_local)
			{
				res = processor->getReferenceSubsectionCurrentSection_name(datTmp, &datTmp_ss, tokenss1.c_str());
				if((res == JVX_NO_ERROR) && datTmp_ss)
				{
					res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
					if(res != JVX_NO_ERROR)
					{
						if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
						{
							REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType1 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
						else
						{
							REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType1 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
					}
				}
				else
				{
					REPORT_ERROR(((std::string)"Failed to read entry " + tokenss1 + ", object type " + cType1 + " <" + entry + ">, from config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
					datTmp_ss = NULL;
				}
			}
			/* Do not return handle yet, we may need it lateron */

			// Step III: Check state to see what needs to be done.
			jvxSize idxC = jvx_stateToIndex(JVX_STATE_ACTIVE);
			assert(idxC >= 0);
			if(targetStateIdx >= (jvxSize) idxC)
			{
				// Seems that activation is desired
				res = this_pointer->activate_component(tp1);
				if( res != JVX_NO_ERROR)
				{
					REPORT_ERROR(((std::string)"Unexpected error when activating component " + cType1 + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
					wantToContinue = false;
				}
				else
				{
					if(reportOnConfig)
					{
						reportOnConfig->report_component_active_on_config(tp1);
					}
				}
			}
			else
			{
				wantToContinue = false;
			}

			if(wantToContinue)
			{
				// Activation successful, next, configure component specific. Handle should still be "alive"
				if(config_local && datTmp_ss)
				{
					res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
					if(res != JVX_NO_ERROR)
					{
						if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
						{
							REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType1 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
						else
						{
							REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType1 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
					}
				}
			}

			// Return handle for ocmponent configuration
			if(config_local)
			{
				theStruct.theHandle_shortcut_tech->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
			}
			config_local = NULL;
			datTmp_ss = NULL;
		}

		if(wantToContinue)
		{

			// Check the desired target state
			targetStateIdx = 0;
			res = HjvxConfigProcessor_readEntry_assignmentData(processor, datTmp, tokenb, &valD);
			if(res == JVX_NO_ERROR)
			{
				targetStateIdx = JVX_DATA2INT16(valD);
			}
			else
			{
				REPORT_ERROR(((std::string)"Failed to obtain entry " + token2 + " from config file " + filename + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
				wantToContinue = false;
			}
		}
		if(wantToContinue)
		{
			jvxInt16 idxC = jvx_stateToIndex(JVX_STATE_SELECTED);
			assert(idxC >= 0);
			if(targetStateIdx >= (jvxSize)idxC)
			{
				// If selection is desired find out what to select
				res = HjvxConfigProcessor_readEntry_assignmentString(processor, datTmp, token2, &entry);
				if(res == JVX_NO_ERROR)
				{
					// Compare value to all descriptions of devices
					selectme = -1;
					num = 0;
					this_pointer->number_components(tp2, &num);
					for(i = 0; i < num; i++)
					{
						fldStr = NULL;
						desc = "";
						this_pointer->description_component(tp2, i, &fldStr);
						if(fldStr)
						{
							desc = fldStr->bString;
							this_pointer->deallocate(fldStr);
						}

						if(entry == desc)
						{
							selectme = (jvxInt16)i;
							break;
						}
					}

					if(selectme >= 0)
					{
						res = this_pointer->select_component(tp2, selectme);
						if( res != JVX_NO_ERROR)
						{
							REPORT_ERROR(((std::string)"Unexpected error when selecting component " + cType2 + ".").c_str(), JVX_REPORT_PRIORITY_ERROR);
							wantToContinue = false;
						}
					}
					else
					{
						REPORT_ERROR(((std::string)"Failed to find component with description " + entry + " on target machine.").c_str(), JVX_REPORT_PRIORITY_ERROR);
						wantToContinue = false;
					}
				}
				else
				{
					REPORT_ERROR(((std::string)"Failed to find component description entry for symbol " + token2 + " in config file.").c_str(), JVX_REPORT_PRIORITY_ERROR);
					wantToContinue = false;
				}

				if(wantToContinue)
				{
					// Get access to configuration handle for component
					datTmp_ss = NULL;
					config_local = NULL;
					theStruct.theHandle_shortcut_dev->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);
					if(config_local)
					{
						res = processor->getReferenceSubsectionCurrentSection_name(datTmp, &datTmp_ss, tokenss2.c_str());
						if((res == JVX_NO_ERROR) && datTmp_ss)
						{
							res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
							if(res != JVX_NO_ERROR)
							{
								if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
								{
									REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType2 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
								}
								{
									REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType2 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
								}
							}
						}
						else
						{
							REPORT_ERROR(((std::string)"Failed to read entry " + tokenss2 + ", object type " + cType2 + " <" + entry + ">, from config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
							datTmp_ss = NULL;
						}
					}

					// Check wether activation is desired
					jvxInt16 idxC = jvx_stateToIndex(JVX_STATE_ACTIVE);
					assert(idxC >= 0);
					if(targetStateIdx >= (jvxSize) idxC)
					{
						res = this_pointer->activate_component(tp2);
						if(res != JVX_NO_ERROR)
						{
							REPORT_ERROR(((std::string)"Unexpected error when activating component " + cType2 + ".").c_str(), JVX_REPORT_PRIORITY_ERROR);
							wantToContinue = false;
						}
						else
						{
							if(reportOnConfig)
							{
								reportOnConfig->report_component_active_on_config(tp2);
							}
						}
					}
					else
					{ 
						wantToContinue = false;
					}

					if(wantToContinue)
					{
						if(config_local && datTmp_ss)
						{
							res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
							if(res != JVX_NO_ERROR)
							{
								if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
								{
									REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType2 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
								}
								else
								{
									REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType2 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
								}
							}
						}
					}

					// Return configuration handle the 
					if(config_local)
					{
						theStruct.theHandle_shortcut_dev->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
					}
					config_local = NULL;
					datTmp_ss = NULL;
				} // if(goahead)
			} // if(targetStateIdx >= jvx_stateToIndex(JVX_STATE_SELECTED))
		} // if(goahead) (for device)
	}
}

template<typename T> static void get_configuration_technology(T& theStruct, 
																		   IjvxConfigProcessor* processor, 
																		   jvxHandle* datTmp, 
																		   std::string tokena,
																		   std::string tokenb,
																		   std::string token1,
																		   std::string token2, 
																		   std::string tokenss1,
																		   std::string tokenss2)
{
	jvxConfigData* datTmp_add = NULL;
	IjvxConfiguration* config_local = NULL;
	jvxString* fldStr = NULL;
	jvxBool errorDetected = false;
	std::string entry;
	jvxErrorType res = JVX_NO_ERROR;
	jvxData valD = 0;

	if(theStruct.theHandle_shortcut_tech)
	{
		// Write the name of the selected scanner technology
		errorDetected = false;
		fldStr = NULL;
		theStruct.theHandle_shortcut_tech->description(&fldStr);
		if(fldStr)
		{
			entry = fldStr->bString;
			theStruct.theHandle_shortcut_tech->deallocate(fldStr);
			HjvxConfigProcessor_writeEntry_assignmentString(processor, datTmp, token1, &entry);
		}
		else
		{
			errorDetected = true;
		}

		// Write the state of the selected scanner technology
		jvxState stat = JVX_STATE_NONE;
		res = theStruct.theHandle_shortcut_tech->state(&stat);
		if(res == JVX_NO_ERROR)
		{
			valD = (jvxData)jvx_stateToIndex(stat);
			HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokena, &valD);
		}
		else
		{
			errorDetected = true;
		}

		config_local = NULL;
		theStruct.theHandle_shortcut_tech->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);
		if(config_local)
		{
			datTmp_add = NULL;
			processor->createEmptySection(&datTmp_add, tokenss1.c_str());
			config_local->get_configuration(processor, datTmp_add);
			processor->addSubsectionToSection(datTmp, datTmp_add);
		}
		theStruct.theHandle_shortcut_tech->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
	}
	else
	{
		jvxState stat = JVX_STATE_NONE;
		valD = (jvxData)jvx_stateToIndex(stat);
		HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokena, &valD);
	}
	assert(!errorDetected);

	// + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
	// SCANNER DEVICE
	// + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

	if(theStruct.theHandle_shortcut_dev)
	{
		// Write the name of the selected scanner technology
		errorDetected = false;
		fldStr = NULL;
		theStruct.theHandle_shortcut_dev->description(&fldStr);
		if(fldStr)
		{
			entry = fldStr->bString;
			theStruct.theHandle_shortcut_dev->deallocate(fldStr);
			HjvxConfigProcessor_writeEntry_assignmentString(processor, datTmp, token2, &entry);
		}
		else
		{
			errorDetected = true;
		}

		// Write the state of the selected scanner technology
		jvxState stat = JVX_STATE_NONE;
		res =theStruct.theHandle_shortcut_dev->state(&stat);
		if(res == JVX_NO_ERROR)
		{
			valD = (jvxData)jvx_stateToIndex(stat);
			HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokenb, &valD);
		}
		else
		{
			errorDetected = true;
		}

		config_local = NULL;
		theStruct.theHandle_shortcut_dev->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);
		if(config_local)
		{
			datTmp_add = NULL;
			processor->createEmptySection(&datTmp_add, tokenss2.c_str());
			config_local->get_configuration(processor, datTmp_add);
			processor->addSubsectionToSection(datTmp, datTmp_add);
		}
		theStruct.theHandle_shortcut_dev->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
	}
	else
	{
		jvxState stat = JVX_STATE_NONE;
		valD = (jvxData)jvx_stateToIndex(stat);
		HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokenb, &valD);
	}
	assert(!errorDetected);
}

// =============================================================================================
// =============================================================================================
// =============================================================================================

/*
Wrapping typedefs for components must be like:

typedef struct
{
	oneComponentWrap common;
	IjvxAscanEndpoint* theHandle;
} oneAscanEndpoint;

typedef struct
{
	std::vector<oneAscanEndpoint> availableAscanEndpoints;
	IjvxAscanEndpoint* theHandle_shortcut;
} ascanEndpointsT;
*/

template<typename T1, typename T2> static void put_configuration_component(T1& theStruct, T2& theList, 
																		   IjvxConfigProcessor* processor, 
																		   jvxHandle* datTmp, 
																		   const char* filename, jvxInt32 lineno, 
																		   std::string tokena,
																		   std::string token1,
																		   std::string tokenss1,
																		   jvxComponentType tp1, 
																		   std::string cType1,
																		   IjvxReport* report,
																		   IjvxReportOnConfig* reportOnConfig, 
																		   IjvxHost* this_pointer)
{
	jvxSize i;
	IjvxConfiguration* config_local = NULL;
	jvxData valD;
	jvxErrorType res = JVX_NO_ERROR;
	jvxSize targetStateIdx = 0;
	jvxString* fldStr = NULL;
	std::string entry;
	std::string desc;
	jvxBool wantToContinue = true;
	jvxInt16 selectme = -1;
	jvxSize num = 0;
	// Step I: Get the desired state from configuration
	targetStateIdx = 0;
	jvxHandle* datTmp_ss = NULL;

	res = HjvxConfigProcessor_readEntry_assignmentData(processor, datTmp, tokena, &valD);
	if(res == JVX_NO_ERROR)
	{
		targetStateIdx = JVX_DATA2INT16(valD);
	}
	else
	{
		REPORT_ERROR(((std::string)"Failed to obtain entry " + token1 + " from config file " + filename + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
		wantToContinue = false;
	}

	if(wantToContinue)
	{
		// Check state to see what needs to be done.
		jvxSize idxC = jvx_stateToIndex(JVX_STATE_SELECTED);
		assert(idxC >= 0);
		if(targetStateIdx >= (jvxSize) idxC)
		{
			// if selection is desired, go and get the name of the selected component
			res = HjvxConfigProcessor_readEntry_assignmentString(processor, datTmp, token1, &entry);
			if(res == JVX_NO_ERROR)
			{
				// Find the selection id for the desired component
				selectme = -1;
				for(i = 0; i < theList.size(); i++)
				{ 
					fldStr = NULL;
					desc = "";
					theList[i].common.hObject->description(&fldStr);
					if(fldStr)
					{
						desc = fldStr->bString;
						theList[i].common.hObject->deallocate(fldStr);
					}

					if(entry == desc)
					{
						selectme = (jvxInt16)i;
						break;
					}
				}

				// Step II: If selection is valid, select technology
				if(selectme >= 0)
				{
					res = this_pointer->select_component(tp1, selectme);
					if( res != JVX_NO_ERROR)
					{
						REPORT_ERROR(((std::string)"Unexpected error when selecting component " + cType1 + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
						wantToContinue = false;
					}
				}
				else
				{
					REPORT_ERROR(((std::string)"Failed to find component with description " + entry + " on target machine.").c_str(), JVX_REPORT_PRIORITY_WARNING);
					wantToContinue = false;
				}
			}
			else
			{
				REPORT_ERROR(((std::string)"Failed to find component description entry for symbol " + token1 + " in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
				wantToContinue = false;
			}
		}
		else
		{
			// No selection is desired
			wantToContinue = false;
		}

		// If selection was successful, next, pass config entries to component to be read in state selected. Afterwards, activate
		if(wantToContinue)
		{
			// Get configuration interface and pass configuration
			datTmp_ss = NULL;
			config_local = NULL;
			theStruct.theHandle_shortcut->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);
			if(config_local)
			{
				res = processor->getReferenceSubsectionCurrentSection_name(datTmp, &datTmp_ss, tokenss1.c_str());
				if((res == JVX_NO_ERROR) && datTmp_ss)
				{
					res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
					if(res != JVX_NO_ERROR)
					{
						if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
						{
							REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType1 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
						else
						{
							REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType1 + " <" + entry + "> - state selected - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
					}
				}
				else
				{
					REPORT_ERROR(((std::string)"Failed to read entry " + tokenss1 + ", object type " + cType1 + " <" + entry + ">, from config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
					datTmp_ss = NULL;
				}
			}
			/* Do not return handle yet, we may need it lateron */

			// Step III: Check state to see what needs to be done.
			jvxInt16 idxC = jvx_stateToIndex(JVX_STATE_ACTIVE);
			assert(idxC >= 0);
			if(targetStateIdx >= (jvxSize)idxC)
			{
				// Seems that activation is desired
				res = this_pointer->activate_component(tp1);
				if( res != JVX_NO_ERROR)
				{
					REPORT_ERROR(((std::string)"Unexpected error when activating component " + cType1 + ".").c_str(), JVX_REPORT_PRIORITY_WARNING);
					wantToContinue = false;
				}
				else
				{
					if(reportOnConfig)
					{
						reportOnConfig->report_component_active_on_config(tp1);
					}
				}
			}
			else
			{
				wantToContinue = false;
			}

			if(wantToContinue)
			{
				// Activation successful, next, configure component specific. Handle should still be "alive"
				if(config_local && datTmp_ss)
				{
					res = config_local->put_configuration(processor, datTmp_ss, filename, lineno);
					if(res != JVX_NO_ERROR)
					{
						if(res == JVX_ERROR_ELEMENT_NOT_FOUND)
						{
							REPORT_ERROR(((std::string)"--> One or more configuration entries not found in object type " + cType1 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
						else
						{
							REPORT_ERROR(((std::string)"Failed to read configuration in object type " + cType1 + " <" + entry + "> - state active - in config file.").c_str(), JVX_REPORT_PRIORITY_WARNING);
						}
					}
				}
			}

			// Return handle for ocmponent configuration
			if(config_local)
			{
				theStruct.theHandle_shortcut->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
			}
			config_local = NULL;
			datTmp_ss = NULL;
		}
	}
}

template<typename T> static void get_configuration_component(T& theStruct, 
															 IjvxConfigProcessor* processor, 
															 jvxHandle* datTmp, 
															 std::string tokena,
															 std::string token1,
															 std::string token11)
{
	jvxConfigData* datTmp_add = NULL;
	IjvxConfiguration* config_local = NULL;
	jvxString* fldStr = NULL;
	jvxBool errorDetected = false;
	std::string entry;
	jvxErrorType res = JVX_NO_ERROR;
	jvxData valD = 0;

	if(theStruct.theHandle_shortcut)
	{
		// Write the name of the selected scanner technology
		errorDetected = false;
		fldStr = NULL;
		theStruct.theHandle_shortcut->description(&fldStr);
		if(fldStr)
		{
			entry = fldStr->bString;
			theStruct.theHandle_shortcut->deallocate(fldStr);
			HjvxConfigProcessor_writeEntry_assignmentString(processor, datTmp, token1, &entry);
		}
		else
		{
			errorDetected = true;
		}

		// Write the state of the selected scanner technology
		jvxState stat = JVX_STATE_NONE;
		res = theStruct.theHandle_shortcut->state(&stat);
		if(res == JVX_NO_ERROR)
		{
			valD = (jvxData)jvx_stateToIndex(stat);
			HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokena, &valD);
		}
		else
		{
			errorDetected = true;
		}

		config_local = NULL;
		theStruct.theHandle_shortcut->request_hidden_interface(JVX_INTERFACE_CONFIGURATION, (jvxHandle**)&config_local);
		if(config_local)
		{
			datTmp_add = NULL;
			processor->createEmptySection(&datTmp_add, token11.c_str());
			config_local->get_configuration(processor, datTmp_add);
			processor->addSubsectionToSection(datTmp, datTmp_add);
		}
		theStruct.theHandle_shortcut->return_hidden_interface(JVX_INTERFACE_CONFIGURATION, config_local);
	}
	else
	{
		jvxState stat = JVX_STATE_NONE;
		valD = (jvxData)jvx_stateToIndex(stat);
		HjvxConfigProcessor_writeEntry_assignmentData(processor, datTmp, tokena, &valD);
	}
	assert(!errorDetected);
}

#endif
