#ifndef __HJVXCONFIGPROPERTIES_H__
#define __HJVXCONFIGPROPERTIES_H__

static bool jvx_findPropertyDescriptor(const std::string& token, jvxSize* id, jvxDataFormat* form, jvxInt32* numElements, jvxPropertyDecoderHintType* decHt)
{
	bool foundIt = false;
	int idx = 0;
	while(1)
	{
		if(jvxProperties[idx].id < 0)
		{
			break;
		}
		if(jvxProperties[idx].nameProperty == token)
		{
			if(id)
			{
				*id = jvxProperties[idx].id;
			}
			if(form)
			{
				*form = jvxProperties[idx].format;
			}
			if(numElements)
			{
				*numElements = jvxProperties[idx].numElements;
			}
			if(decHt)
			{
				*decHt = jvxProperties[idx].hintTp;
			}
			foundIt = true;
			break;
		}
		idx++;
	}
	return(foundIt);
}

static bool jvx_findPropertyDescriptorId(jvxSize id, jvxDataFormat* form, jvxInt32* numElements, jvxPropertyDecoderHintType* decHt)
{
	bool foundIt = false;
	int idx = 0;
	while(1)
	{
		if(jvxProperties[idx].id < 0)
		{
			break;
		}
		if(jvxProperties[idx].id == id)
		{
			if(form)
			{
				*form = jvxProperties[idx].format;
			}
			if(numElements)
			{
				*numElements = jvxProperties[idx].numElements;
			}
			if(decHt)
			{
				*decHt = jvxProperties[idx].hintTp;
			}
			foundIt = true;
			break;
		}
		idx++;
	}
	return(foundIt);
}

static bool jvx_findPropertyNameId(jvxSize id, std::string& name)
{
	bool foundIt = false;
	int idx = 0;
	while(1)
	{
		if(jvxProperties[idx].id < 0)
		{
			break;
		}
		if(jvxProperties[idx].id == id)
		{
			name = jvxProperties[idx].nameProperty;
			foundIt = true;
			break;
		}
		idx++;
	}
	return(foundIt);
}

static jvxErrorType
jvx_pushPropertyStringList(std::vector<std::string>& entries, IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxStringList* newList = NULL;

	HjvxObject_produceStringList(&newList, entries);
	if(newList)
	{
		res = prop->set_property((jvxHandle*)newList, 1, JVX_DATAFORMAT_STRING_LIST, id, cat, 0, false);
		HjvxObject_deallocate(newList);
	}
	else
	{
		res = JVX_ERROR_UNEXPECTED;
	}
	return(res);
}
static jvxErrorType
jvx_pushPullPropertyStringList(std::vector<std::string>& entries, IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat, jvxBool* isValid)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxStringList* newList = NULL;
	jvxSize i;

	HjvxObject_produceStringList(&newList, entries);
	if(newList)
	{
		res = prop->set_property((jvxHandle*)newList, 1, JVX_DATAFORMAT_STRING_LIST, id, cat, 0, false);
		HjvxObject_deallocate(newList);
		if(res == JVX_NO_ERROR)
		{
			newList = NULL;
			res = prop->get_property((jvxHandle*)&newList, 1, JVX_DATAFORMAT_STRING_LIST, id, cat, 0, false, isValid, NULL);
			if(res == JVX_NO_ERROR)
			{
				if(newList)
				{
					entries.clear();
					for(i = 0; i < newList->lStrings; i++)
					{
						entries.push_back(newList->bStrings[i].bString);
					}
					obj->deallocate(newList);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			// else skippeed since error code is already set by subcomponent
		}
		// else skippeed since error code is already set by subcomponent
	}
	else
	{
		res = JVX_ERROR_UNEXPECTED;
	}
	return(res);
}

static jvxErrorType
jvx_pullPropertyStringList(std::vector<std::string>& entries,  IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat, jvxBool* isValid)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxStringList* newList = NULL;
	jvxSize i;

	newList = NULL;
	res = prop->get_property((jvxHandle*)&newList, 1, JVX_DATAFORMAT_STRING_LIST, id, cat, 0, false, isValid, NULL);
	if(res == JVX_NO_ERROR)
	{
		if(newList)
		{
			entries.clear();
			for(i = 0; i < newList->lStrings; i++)
			{
				entries.push_back(newList->bStrings[i].bString);
			}
			obj->deallocate(newList);
		}
		else
		{
			res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
		}
	}
	// else skippeed since error code is already set by subcomponent
	return(res);
}

static jvxErrorType
jvx_pushPropertyString(std::string& entry,  IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxString* newEntry = NULL;
	HjvxObject_produceString(&newEntry, entry);
	if(newEntry)
	{
		res = prop->set_property((jvxHandle*)newEntry, 1, JVX_DATAFORMAT_STRING, id, cat, 0, false);
		HjvxObject_deallocate(newEntry);
	}
	else
	{
		res = JVX_ERROR_UNEXPECTED;
	}
	return(res);
}

static jvxErrorType
jvx_pushPullPropertyString(std::string& entry,  IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat, jvxBool* isValid)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxString* newEntry = NULL;
	HjvxObject_produceString(&newEntry, entry);
	if(newEntry)
	{
		res = prop->set_property((jvxHandle*)newEntry, 1, JVX_DATAFORMAT_STRING, id, cat, 0, false);
		HjvxObject_deallocate(newEntry);
		if(res == JVX_NO_ERROR)
		{
			newEntry = NULL;
			res = prop->get_property((jvxHandle*)&newEntry, 1, JVX_DATAFORMAT_STRING, id, cat, 0, false, isValid, NULL);
			if(res == JVX_NO_ERROR)
			{
				if(newEntry)
				{
					entry.clear();
					entry = newEntry->bString;
					obj->deallocate(newEntry);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			// else skipped, error code is set by submodule
		}
		// else skipped, error code set by submodule
	}
	else
	{
		res = JVX_ERROR_UNEXPECTED;
	}
	return(res);
}

static jvxErrorType
jvx_pullPropertyString(std::string& entry,  IjvxObject* obj, IjvxProperties* prop, jvxSize id, jvxPropertyCategoryType cat, jvxBool* isValid, jvxPropertyAccessType* accessType)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxString* newEntry = NULL;
	res = prop->get_property((jvxHandle*)&newEntry, 1, JVX_DATAFORMAT_STRING, id, cat, 0, false, isValid, accessType);
	if(res == JVX_NO_ERROR)
	{
		if(newEntry)
		{
			entry.clear();
			entry = newEntry->bString;
			obj->deallocate(newEntry);
		}
		else
		{
			res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
		}
	}

	// else skipped, error code is set by submodule
	return(res);
}

static oneMultichannelCircBufferHeader* jvx_allocateCircBuffer(jvxSize bLength, jvxSize16 numChannels, 
															   lock_circ_buffer lockf, try_lock_circ_buffer try_lockf, unlock_circ_buffer unlockf, jvxSize* szFld)
{
	oneMultichannelCircBufferHeader* theNewHeader = NULL;

	jvxSize sz = sizeof(oneMultichannelCircBufferHeader) + sizeof(jvxData) * bLength * numChannels;
	JVX_SAFE_NEW_FLD_CVRT(theNewHeader, jvxByte, sz, oneMultichannelCircBufferHeader*);
	memset(theNewHeader, 0, sizeof(jvxByte) * sz);
	if(szFld)
	{
		*szFld = sz;
	}
	theNewHeader->allowOverwrite = false;
	theNewHeader->bLength = bLength;
	theNewHeader->fHeight = 0;
	theNewHeader->idxRead = 0;
	theNewHeader->numChannels = numChannels;
	theNewHeader->numWraps = 0;

	theNewHeader->safeAccessBuffer.allowRemove = true;
	JVX_MUTEX_HANDLE* mutHdl = NULL;
	JVX_SAFE_NEW_OBJ(mutHdl, JVX_MUTEX_HANDLE);
	JVX_INITIALIZE_MUTEX(*mutHdl);
	theNewHeader->safeAccessBuffer.priv = (jvxHandle*)mutHdl;
	theNewHeader->safeAccessBuffer.lockf = lockf;
	theNewHeader->safeAccessBuffer.unlockf = unlockf;
	theNewHeader->safeAccessBuffer.try_lockf = try_lockf;

	return(theNewHeader);
}

static void jvx_deallocateCircBuffer(oneMultichannelCircBufferHeader* header)
{
	JVX_MUTEX_HANDLE* mutHdl = (JVX_MUTEX_HANDLE*)header->safeAccessBuffer.priv;
	JVX_TERMINATE_MUTEX(*mutHdl);
	JVX_SAFE_DELETE_OBJ(mutHdl);
	JVX_SAFE_DELETE_FLD_CVRT(header, jvxByte*);
}

static void jvx_valuesToValueInRange(jvxValueInRange& theObj, jvxData minV = 0.0, jvxData maxV = 1.0, jvxData val = 0.0)
{
	theObj.minVal = minV;
	theObj.maxVal = maxV;
	theObj.val = val;
}

static void jvx_static_lock(jvxHandle* priv)
{
	JVX_MUTEX_HANDLE* theHdl = reinterpret_cast<JVX_MUTEX_HANDLE*>(priv);
	assert(theHdl);
	JVX_LOCK_MUTEX(*theHdl);
}

static void jvx_static_unlock(jvxHandle* priv)
{
	JVX_MUTEX_HANDLE* theHdl = reinterpret_cast<JVX_MUTEX_HANDLE*>(priv);
	assert(theHdl);
	JVX_UNLOCK_MUTEX(*theHdl);
}

static jvxErrorType jvx_static_try_lock(jvxHandle* priv)
{
	JVX_MUTEX_HANDLE* theHdl = reinterpret_cast<JVX_MUTEX_HANDLE*>(priv);
	assert(theHdl);
	JVX_TRY_LOCK_MUTEX_RESULT_TYPE res = JVX_TRY_LOCK_MUTEX_SUCCESS;
	JVX_TRY_LOCK_MUTEX(res, *theHdl);
	if(res == JVX_TRY_LOCK_MUTEX_SUCCESS)
	{
		return(JVX_NO_ERROR);
	}
	return(JVX_ERROR_COMPONENT_BUSY);
}

// ==================================================================

static void jvx_initPropertyDescription(jvxPropertyDescriptor& theStr)
{
	theStr.category = JVX_PROPERTY_CATEGORY_UNKNOWN;
	theStr.settype = JVX_THREADING_NONE;
	theStr.stateAccess = JVX_STATE_NONE;
	theStr.format = JVX_DATAFORMAT_NONE;
	theStr.num = 0;
	theStr.accessType = JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE;
	theStr.decTp = JVX_PROPERTY_DECODER_NONE;
	theStr.globalIdx = (jvxSize)-1;
	theStr.ctxt = JVX_PROPERTY_CONTEXT_UNKNOWN;
	theStr.description = NULL;
	theStr.descriptor = NULL;
	theStr.isValid = false;
}

static void jvx_freePropertyDescription(IjvxDeallocate* theDealloc, jvxPropertyDescriptor& theStr)
{
	theDealloc->deallocate(theStr.name);
	theStr.name = NULL;
	theDealloc->deallocate(theStr.description);
	theStr.description = NULL;
	theDealloc->deallocate(theStr.descriptor);
	theStr.descriptor = NULL;
}

static jvxErrorType jvx_getPropertyDescription(IjvxProperties* theProps, jvxPropertyDescriptor& theStr, jvxSize i)
{
	return(theProps->description_property(i, &theStr.category, &theStr.settype, &theStr.stateAccess,
		&theStr.format, &theStr.num, &theStr.accessType, &theStr.decTp,
		&theStr.globalIdx, &theStr.ctxt, &theStr.name, &theStr.description, &theStr.descriptor,
		&theStr.isValid));
}

static jvxErrorType jvx_getPropertyDescription(IjvxHost* theHost, jvxPropertyDescriptor& theStr, jvxComponentType tp, const char* descriptor)
{
	jvxSize i,num;
	jvx_propertyReferenceTriple theTriple;
	jvxErrorType res = JVX_NO_ERROR;
	jvxBool foundit = false;
	jvx_initPropertyReferenceTriple(&theTriple);
	jvx_getReferencePropertiesObject(theHost, &theTriple, tp);
	if(jvx_isValidPropertyReferenceTriple(&theTriple))
	{
		theTriple.theProps->number_properties(&num);
		for(i = 0; i < num;  i++)
		{
			res = theTriple.theProps->description_property(i, &theStr.category, &theStr.settype, &theStr.stateAccess,
				&theStr.format, &theStr.num, &theStr.accessType, &theStr.decTp,
				&theStr.globalIdx, &theStr.ctxt, &theStr.name, &theStr.description, &theStr.descriptor,
				&theStr.isValid);
			assert(res == JVX_NO_ERROR);
			assert(theStr.descriptor);

			std::string descrC = theStr.descriptor->bString;
			if(descrC == descriptor)
			{
				foundit = true;
				break;
			}
			else
			{
				jvx_freePropertyDescription(theTriple.theObj, theStr);
			}
		}
		jvx_returnReferencePropertiesObject(theHost, &theTriple, tp);
	}
	if(foundit)
	{
		res = JVX_NO_ERROR;
	}
	else
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}
	return(res);
}

static jvxErrorType jvx_get_property(IjvxProperties* theProps, jvxHandle* fld, 
	jvxSize offsetStart,
	jvxSize numElements,
	jvxDataFormat format,
	jvxBool contentOnly,
	const char* descriptor)
{
	jvxSize idx;
	jvxPropertyCategoryType cat;
	jvxErrorType res;
	
	res = theProps->description_property(descriptor, NULL, NULL, &idx, &cat);
	if(res == JVX_NO_ERROR)
	{
		return(theProps->get_property(fld,
			     numElements,
			     format,
			     idx,
			     cat,
			     offsetStart,
			     contentOnly));
	}
	return(res);
}

static jvxErrorType jvx_get_property(IjvxHost* theHost, jvxHandle* fld, 
	jvxSize offsetStart,
	jvxSize numElements,
	jvxDataFormat format,
	jvxBool contentOnly,
	jvxComponentType tp,
	const char* descriptor,
	jvxBool* isValid = NULL)
{
	jvxSize idx;
	jvxPropertyCategoryType cat;
	jvxErrorType res = JVX_ERROR_ELEMENT_NOT_FOUND;

	jvx_propertyReferenceTriple theTriple;
	jvx_initPropertyReferenceTriple(&theTriple);
	jvx_getReferencePropertiesObject(theHost, &theTriple, tp);
	if(jvx_isValidPropertyReferenceTriple(&theTriple))
	{
	
		res = theTriple.theProps->description_property(descriptor, NULL, NULL, &idx, &cat);
		if(res == JVX_NO_ERROR)
		{
			return(theTriple.theProps->get_property(fld,
				numElements,
				format,
				idx,
				cat,
				offsetStart,
				contentOnly, 
				isValid));
		}
		jvx_returnReferencePropertiesObject(theHost, &theTriple, tp);
	}
	return(res);
}
	
static jvxErrorType jvx_set_property(IjvxProperties* theProps, jvxHandle* fld, 
	jvxSize offsetStart,
	jvxSize numElements,
	jvxDataFormat format,
	jvxBool contentOnly,
	const char* descriptor)
{
	jvxSize idx;
	jvxPropertyCategoryType cat;
	jvxErrorType res;
	
	res = theProps->description_property(descriptor, NULL, NULL, &idx, &cat);
	if(res == JVX_NO_ERROR)
	{
		return(theProps->set_property(fld,
			     numElements,
			     format,
			     idx,
			     cat,
			     offsetStart,
			     contentOnly));
	}
	return(res);
}

#endif
