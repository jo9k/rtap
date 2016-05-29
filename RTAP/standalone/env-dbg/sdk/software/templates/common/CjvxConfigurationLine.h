#ifndef _CJVXCONFIGURATIONLINE_H__
#define _CJVXCONFIGURATIONLINE_H__

#include "helpers/HjvxHostsAndProperties.h"

class CjvxConfigurationLine
{
protected:

	typedef struct
	{
		jvxSize selector;
	} jvxOneConfigurationId;

	typedef struct
	{
		jvxComponentType selector;
	} jvxOneSlaveConfigurationLine;

	typedef struct
	{
		std::string selector;
		jvxComponentType master_tp;
		std::vector<jvxOneSlaveConfigurationLine> slaves_tp;
		std::vector<jvxOneConfigurationId> link_properties;
	} jvxOneConfigurationLine;

	struct
	{
		std::vector<jvxOneConfigurationLine> theLines;
	} _common_configurationline;
public:

	JVX_CALLINGCONVENTION CjvxConfigurationLine(){};

	virtual JVX_CALLINGCONVENTION ~CjvxConfigurationLine(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION _number_lines(jvxSize* num_lines )
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(num_lines)
		{
			*num_lines = _common_configurationline.theLines.size();
		}
		return(res);
	};


	virtual jvxErrorType JVX_CALLINGCONVENTION _add_line(jvxComponentType tp_master, const char* description)
	{
		jvxOneConfigurationLine newItem;
		jvxErrorType res = JVX_NO_ERROR;
		std::string selector = description;

		std::vector<jvxOneConfigurationLine>::iterator elm = _common_configurationline.theLines.begin();
		elm = jvx_findItemSelectorInList<jvxOneConfigurationLine, std::string>(_common_configurationline.theLines, selector);
		if(elm == _common_configurationline.theLines.end())
		{
			newItem.selector = description;
			newItem.master_tp = tp_master;
			_common_configurationline.theLines.push_back(newItem);
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}

		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _add_slave_line(jvxSize idxLine, jvxComponentType tp_slave)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(idxLine < _common_configurationline.theLines.size())
		{
			std::vector<jvxOneSlaveConfigurationLine>::iterator elm = _common_configurationline.theLines[idxLine].slaves_tp.begin();
			elm = jvx_findItemSelectorInList<jvxOneSlaveConfigurationLine, jvxComponentType>(
				_common_configurationline.theLines[idxLine].slaves_tp, tp_slave);
			if(elm == _common_configurationline.theLines[idxLine].slaves_tp.end())
			{
				jvxOneSlaveConfigurationLine newEntry;
				newEntry.selector = tp_slave;
				_common_configurationline.theLines[idxLine].slaves_tp.push_back(newEntry);
			}
			else
			{
				res = JVX_ERROR_DUPLICATE_ENTRY;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _add_configuration_property_line(jvxSize idxLine, jvxSize system_property_id)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(idxLine < _common_configurationline.theLines.size())
		{
			std::vector<jvxOneConfigurationId>::iterator elm = _common_configurationline.theLines[idxLine].link_properties.begin();
			elm = jvx_findItemSelectorInList<jvxOneConfigurationId, jvxSize>(
				_common_configurationline.theLines[idxLine].link_properties, system_property_id);
			if(elm == _common_configurationline.theLines[idxLine].link_properties.end())
			{
				jvxOneConfigurationId newEntry;
				newEntry.selector = system_property_id;
				_common_configurationline.theLines[idxLine].link_properties.push_back(newEntry);
			}
			else
			{
				res = JVX_ERROR_DUPLICATE_ENTRY;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _remove_slave_line(jvxSize idxLine, jvxComponentType tp_slave)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(idxLine < _common_configurationline.theLines.size())
		{
			std::vector<jvxOneSlaveConfigurationLine>::iterator elm = _common_configurationline.theLines[idxLine].slaves_tp.begin();
			elm = jvx_findItemSelectorInList<jvxOneSlaveConfigurationLine, jvxComponentType>(
				_common_configurationline.theLines[idxLine].slaves_tp, tp_slave);
			if(elm != _common_configurationline.theLines[idxLine].slaves_tp.end())
			{
				_common_configurationline.theLines[idxLine].slaves_tp.erase(elm);
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
		return(res);
	};

		virtual jvxErrorType JVX_CALLINGCONVENTION _remove_configuration_property_line(jvxSize idxLine, jvxSize system_property_id)
	{
		jvxErrorType res = JVX_NO_ERROR;

		if(idxLine < _common_configurationline.theLines.size())
		{
			std::vector<jvxOneConfigurationId>::iterator elm = _common_configurationline.theLines[idxLine].link_properties.begin();
			elm = jvx_findItemSelectorInList<jvxOneConfigurationId, jvxSize>(
				_common_configurationline.theLines[idxLine].link_properties, system_property_id);
			if(elm != _common_configurationline.theLines[idxLine].link_properties.end())
			{
				_common_configurationline.theLines[idxLine].link_properties.erase(elm);
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _remove_line(jvxSize idxLine)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<jvxOneConfigurationLine>::iterator elm = _common_configurationline.theLines.begin();
		elm = jvx_findItemIdInList<jvxOneConfigurationLine>(_common_configurationline.theLines, idxLine);
		if(elm != _common_configurationline.theLines.end())
		{
			_common_configurationline.theLines.erase(elm);
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _description_line(jvxSize idxLine, jvxComponentType* master, jvxSize* num_slaves, jvxSize* num_system_properties)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(idxLine < _common_configurationline.theLines.size())
		{
			if(num_slaves)
			{
				*num_slaves = _common_configurationline.theLines[idxLine].slaves_tp.size();
			}
			if(num_system_properties)
			{
				*num_system_properties = _common_configurationline.theLines[idxLine].link_properties.size();
			}
			if(master)
			{
				*master = _common_configurationline.theLines[idxLine].master_tp;
			}
		}
		else
		{
			res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		}
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _description_slave_line(jvxSize idxLine, jvxSize idxEntry, jvxComponentType* the_slave)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(idxLine < _common_configurationline.theLines.size())
		{
			if(idxEntry < _common_configurationline.theLines[idxLine].slaves_tp.size())
			{
				if(the_slave)
				{
					*the_slave = _common_configurationline.theLines[idxLine].slaves_tp[idxEntry].selector;
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
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _description_property_configuration_line(jvxSize idxLine, jvxSize idxEntry, jvxSize* the_prop_id)
	{
		jvxErrorType res = JVX_NO_ERROR;
		if(idxLine < _common_configurationline.theLines.size())
		{
			if(idxEntry < _common_configurationline.theLines[idxLine].link_properties.size())
			{
				if(the_prop_id)
				{
					*the_prop_id = _common_configurationline.theLines[idxLine].link_properties[idxEntry].selector;
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
		return(res);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _report_property_was_set(jvxComponentType tp, jvxPropertyCategoryType cat, jvxSize propIdx, IjvxHost* theHost, jvxBool onlyContent, jvxComponentType tpTo)
	{
		jvxSize i, j, k;
		jvxDataFormat form = JVX_DATAFORMAT_NONE;
		jvxInt32 num = 0;
		jvxInt32 valI32 = 0;
		jvxInt64 valI64 = 0;
		jvxInt16 valI16 = 0;
		jvxInt8 valI8 = 0;
		jvxData valDbl = 0;
		jvxData valFlt = 0;
		jvxSelectionList lst;
		jvxStringList strListLoc;
		lst.strList = NULL;

		jvx_propertyReferenceTriple theTriple_get;
		jvx_propertyReferenceTriple theTriple_set;

		jvxHandle* theDataPtr = NULL;
		jvxErrorType res = JVX_NO_ERROR;

		if(tpTo == JVX_COMPONENT_UNKNOWN)
		{
			for(i = 0; i < _common_configurationline.theLines.size(); i++)
			{
				if(tp == _common_configurationline.theLines[i].master_tp)
				{

					for(j = 0; j < _common_configurationline.theLines[i].link_properties.size(); j++)
					{
						jvxSize thePropId = _common_configurationline.theLines[i].link_properties[j].selector;
						if(thePropId == propIdx)
						{
							if(jvx_findPropertyDescriptorId(thePropId, &form, &num, NULL))
							{
								if(num == 1)
								{
									switch(form)
									{
									case JVX_DATAFORMAT_64BIT_LE:
										theDataPtr = &valI64;
										break;
									case JVX_DATAFORMAT_32BIT_LE:
									case JVX_DATAFORMAT_24BIT_LE:
										theDataPtr = &valI32;
										break;
									case JVX_DATAFORMAT_16BIT_LE:
										theDataPtr = &valI16;
										break;
									case JVX_DATAFORMAT_8BIT:
										theDataPtr = &valI8;
										break;
									case JVX_DATAFORMAT_DATA:
										theDataPtr = &valDbl;
										break;
									case JVX_DATAFORMAT_SELECTION_LIST:
										theDataPtr = &lst;
										break;
									default:
										theDataPtr = NULL;
									}

									if(theHost && theDataPtr)
									{
										jvx_initPropertyReferenceTriple(&theTriple_get);
										res = jvx_getReferencePropertiesObject(theHost, &theTriple_get, tp);
										if((res == JVX_NO_ERROR) && theTriple_get.theProps)
										{
											bool isValid = false;

											switch(form)
											{
											case JVX_DATAFORMAT_SELECTION_LIST:
												lst.strList = NULL;
												if(onlyContent)
												{
													lst.strList = &strListLoc;
													res = theTriple_get.theProps->get_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
													if((res == JVX_NO_ERROR) && isValid && lst.strList)
													{
														for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
														{
															jvx_initPropertyReferenceTriple(&theTriple_set);
															res = jvx_getReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
															if((res == JVX_NO_ERROR) && theTriple_set.theProps)
															{
																res = theTriple_set.theProps->set_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
																jvx_returnReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
															}
														}
													}
												}
												else
												{
													lst.strList = NULL;
													res = theTriple_get.theProps->get_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
													if((res == JVX_NO_ERROR) && isValid && lst.strList)
													{
														for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
														{
															jvx_initPropertyReferenceTriple(&theTriple_set);
															res = jvx_getReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
															if((res == JVX_NO_ERROR) && theTriple_set.theProps)
															{
																res = theTriple_set.theProps->set_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
																jvx_returnReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
															}
														}
														if(lst.strList)
														{
															theTriple_set.theObj->deallocate(lst.strList);
														}
													}
												}
												break;
											default:
												res = theTriple_get.theProps->get_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
												if((res == JVX_NO_ERROR) && isValid)
												{
													for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
													{
														jvx_initPropertyReferenceTriple(&theTriple_set);
														res = jvx_getReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
														if((res == JVX_NO_ERROR) && theTriple_set.theProps)
														{
															res = theTriple_set.theProps->set_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
															jvx_returnReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
														}
													}
												}
												break;
											}
											jvx_returnReferencePropertiesObject(theHost, &theTriple_get, tp); // <- return reference
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if(jvx_findPropertyDescriptorId(propIdx, &form, &num, NULL))
			{
				if(num == 1)
				{
					switch(form)
					{
					case JVX_DATAFORMAT_64BIT_LE:
						theDataPtr = &valI64;
						break;
					case JVX_DATAFORMAT_32BIT_LE:
					case JVX_DATAFORMAT_24BIT_LE:
						theDataPtr = &valI32;
						break;
					case JVX_DATAFORMAT_16BIT_LE:
						theDataPtr = &valI16;
						break;
					case JVX_DATAFORMAT_8BIT:
						theDataPtr = &valI8;
						break;
					case JVX_DATAFORMAT_DATA:
						theDataPtr = &valDbl;
						break;
					case JVX_DATAFORMAT_SELECTION_LIST:
						theDataPtr = &lst;
						break;
					default:
						theDataPtr = NULL;					
					}

					if(theHost && theDataPtr)
					{
						jvx_initPropertyReferenceTriple(&theTriple_get);
						res = jvx_getReferencePropertiesObject(theHost, &theTriple_get, tp);
						if((res == JVX_NO_ERROR) && theTriple_get.theProps)
						{
							bool isValid = false;
							switch(form)
							{
							case JVX_DATAFORMAT_SELECTION_LIST:

								lst.strList = NULL;
								if(onlyContent)
								{
									lst.strList = &strListLoc;

									res = theTriple_get.theProps->get_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
									if((res == JVX_NO_ERROR) && isValid && lst.strList)
									{

										jvx_initPropertyReferenceTriple(&theTriple_set);
										res = jvx_getReferencePropertiesObject(theHost, &theTriple_set,tpTo);
										if((res == JVX_NO_ERROR) && theTriple_set.theProps)
										{
											res = theTriple_set.theProps->set_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
											jvx_returnReferencePropertiesObject(theHost, &theTriple_set, tpTo);
										}
									}
								}
								else
								{
									lst.strList = NULL;
									res = theTriple_get.theProps->get_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
									if((res == JVX_NO_ERROR) && isValid && lst.strList)
									{

										jvx_initPropertyReferenceTriple(&theTriple_set);
										res = jvx_getReferencePropertiesObject(theHost, &theTriple_set,tpTo);
										if((res == JVX_NO_ERROR) && theTriple_set.theProps)
										{
											res = theTriple_set.theProps->set_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
											jvx_returnReferencePropertiesObject(theHost, &theTriple_set, tpTo);
										}
										if(lst.strList)
										{
											theTriple_set.theObj->deallocate(lst.strList);
										}
									}
								}
								break;
							default:
								res = theTriple_get.theProps->get_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent, &isValid, NULL);
								if((res == JVX_NO_ERROR) && isValid)
								{

									jvx_initPropertyReferenceTriple(&theTriple_set);
									res = jvx_getReferencePropertiesObject(theHost, &theTriple_set,tpTo);
									if((res == JVX_NO_ERROR) && theTriple_set.theProps)
									{
										res = theTriple_set.theProps->set_property(theDataPtr, num, form, propIdx, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, onlyContent);
										jvx_returnReferencePropertiesObject(theHost, &theTriple_set, tpTo);
									}
								}
							}
							jvx_returnReferencePropertiesObject(theHost, &theTriple_get, tp); // <- return reference
						}
					}
				}
			}
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _refresh_all(IjvxHost* theHost)
	{
		jvxSize i, j, k;
		jvxDataFormat form = JVX_DATAFORMAT_NONE;
		jvxInt32 num = 0;
		jvxInt32 valI32 = 0;
		jvxInt64 valI64 = 0;
		jvxInt16 valI16 = 0;
		jvxInt8 valI8 = 0;
		jvxData valDbl = 0;
		jvxData valFlt = 0;
		jvxSelectionList lst;
		lst.strList = NULL;

		jvx_propertyReferenceTriple theTriple_get;
		jvx_propertyReferenceTriple theTriple_set;

		jvxHandle* theDataPtr = NULL;
		jvxErrorType res = JVX_NO_ERROR;

		for(i = 0; i < _common_configurationline.theLines.size(); i++)
		{
			jvxComponentType tp = _common_configurationline.theLines[i].master_tp;
				
			for(j = 0; j < _common_configurationline.theLines[i].link_properties.size(); j++)
			{
				jvxSize thePropId = _common_configurationline.theLines[i].link_properties[j].selector;
				if(jvx_findPropertyDescriptorId(thePropId, &form, &num, NULL))
				{
					if(num == 1)
					{
						switch(form)
						{
						case JVX_DATAFORMAT_64BIT_LE:
							theDataPtr = &valI64;
							break;
						case JVX_DATAFORMAT_32BIT_LE:
						case JVX_DATAFORMAT_24BIT_LE:
							theDataPtr = &valI32;
							break;
						case JVX_DATAFORMAT_16BIT_LE:
							theDataPtr = &valI16;
							break;
						case JVX_DATAFORMAT_8BIT:
							theDataPtr = &valI8;
							break;
						case JVX_DATAFORMAT_DATA:
							theDataPtr = &valDbl;
							break;
						case JVX_DATAFORMAT_SELECTION_LIST:
							theDataPtr = &lst;
							break;
						default:
							assert(0);
						}

						if(theHost)
						{
							jvx_initPropertyReferenceTriple(&theTriple_get);
							res = jvx_getReferencePropertiesObject(theHost, &theTriple_get, tp);
							if((res == JVX_NO_ERROR) && theTriple_get.theProps)
							{
								bool isValid = false;
								res = theTriple_get.theProps->get_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, false, &isValid, NULL);
								
								switch(form)
								{
								case  JVX_DATAFORMAT_SELECTION_LIST:
									if((res == JVX_NO_ERROR) && isValid)
									{
										for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
										{
											jvx_initPropertyReferenceTriple(&theTriple_set);
											res = jvx_getReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
											if((res == JVX_NO_ERROR) && theTriple_set.theProps)
											{
												res = theTriple_set.theProps->set_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, false);
												jvx_returnReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
											}
										}									
									}
									if(lst.strList)
									{
										theTriple_get.theObj->deallocate(lst.strList);
									}
									break;
								default:
									if((res == JVX_NO_ERROR) && isValid)
									{
										for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
										{
											jvx_initPropertyReferenceTriple(&theTriple_set);
											res = jvx_getReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
											if((res == JVX_NO_ERROR) && theTriple_set.theProps)
											{
												res = theTriple_set.theProps->set_property(theDataPtr, num, form, thePropId, JVX_PROPERTY_CATEGORY_PREDEFINED, 0, false);
												jvx_returnReferencePropertiesObject(theHost, &theTriple_set, _common_configurationline.theLines[i].slaves_tp[k].selector);
											}
										}									
									}
									break;
								}

								jvx_returnReferencePropertiesObject(theHost, &theTriple_get, tp); // <- return reference

							}
						}
					}
				}
			}
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _updateAllAfterStateChanged(jvxComponentType tp, jvxState newState, IjvxHost* theHost)
	{
		jvxSize i,j,k;
		switch(newState)
		{
		case JVX_STATE_SELECTED:
		case JVX_STATE_ACTIVE:

			for(i = 0; i < _common_configurationline.theLines.size(); i++)
			{
				for(k = 0; k < _common_configurationline.theLines[i].slaves_tp.size(); k++)
				{
					if(tp == _common_configurationline.theLines[i].slaves_tp[k].selector)
					{
						for(j = 0; j < _common_configurationline.theLines[i].link_properties.size(); j++)
						{
							jvxSize thePropId = _common_configurationline.theLines[i].link_properties[j].selector;
							_report_property_was_set(_common_configurationline.theLines[i].master_tp, JVX_PROPERTY_CATEGORY_PREDEFINED,
								thePropId, theHost, true, tp);
						}
					}
				}
			}
			break;
		}
		return(JVX_NO_ERROR);
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _remove_all_lines()
	{
		_common_configurationline.theLines.clear();
		return(JVX_NO_ERROR);
	};
};

#endif