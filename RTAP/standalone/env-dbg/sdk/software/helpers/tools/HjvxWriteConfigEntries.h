#ifndef __HJVXWRITECONFIGENTRIES_H__
#define __HJVXWRITECONFIGENTRIES_H__

/**********************************************************************************
 * Write single entries in config file
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentData(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxData* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), *entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentInt8(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt8* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), *entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentInt16(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt16* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), *entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentInt32(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt32* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), *entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentInt64(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt64* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), (jvxData)*entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentBitField(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxBitField* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentValue(&datTmp, nmToken.c_str(), (jvxData)*entry);
		if(datTmp)
		{
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Write array numeric entries in config file
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dDataList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxData* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < numValues; i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt8List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt8* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < numValues; i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt16List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt16* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < numValues; i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt32List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt32* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < numValues; i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt64ist(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt64* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < numValues; i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, (jvxData)entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Write array numeric entries in config file with STL
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dDataList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxData>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries.size(); i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}


inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt8List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt8>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries.size(); i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt16List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt16>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries.size(); i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt32List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt32>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries.size(); i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
inline jvxErrorType
HjvxConfigProcessor_writeEntry_1dInt64List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt64>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->createAssignmentValueList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries.size(); i++)
			{
				theReader->addAssignmentValueToList(datTmp, 0, (jvxData)entries[i]);
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Write string / string list entries to config file
 *********************************************************************************/
inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentStringList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<std::string>* entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection && entries)
	{
		theReader->createAssignmentStringList(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			for(i = 0; i < entries->size(); i++)
			{
				theReader->addAssignmentStringToList(datTmp, (*entries)[i].c_str());
			}
			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentSelectionList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvx_oneSelectionOption_cpp* entries, bool onlySelectionToFile = false)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL, * datTmp1 = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection && entries)
	{
		theReader->createEmptySection(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			if(!onlySelectionToFile)
			{
				HjvxConfigProcessor_writeEntry_assignmentStringList(theReader, datTmp, JVX_TOKEN_CONFIG_SELECTIONLIST_NAMES, &entries->entries);
				HjvxConfigProcessor_writeEntry_assignmentBitField(theReader, datTmp, JVX_TOKEN_CONFIG_EXCLUSIVE_VALUE, (jvxBitField*)&entries->exclusive);
			}
			HjvxConfigProcessor_writeEntry_assignmentBitField(theReader, datTmp, JVX_TOKEN_CONFIG_SELECTION_VALUE, (jvxBitField*)&entries->selection);


			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentString(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::string* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;

	if(theReader && theSection && entry)
	{
		theReader->createAssignmentString(&datTmp, nmToken.c_str(), entry->c_str());
		theReader->addSubsectionToSection(theSection, datTmp);
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_writeEntry_assignmentValueInRange(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxValueInRange* entries, bool onlySelectionToFile = false)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL, * datTmp1 = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection && entries)
	{
		theReader->createEmptySection(&datTmp, nmToken.c_str());
		if(datTmp)
		{
			if(!onlySelectionToFile)
			{
				HjvxConfigProcessor_writeEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE_MIN, &entries->minVal);
				HjvxConfigProcessor_writeEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE_MAX, &entries->maxVal);
			}
			HjvxConfigProcessor_writeEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE, &entries->val);

			theReader->addSubsectionToSection(theSection, datTmp);
		}
	}
	else
	{
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

#endif
