#ifndef __HJVXREADCONFIGENTRIES_H__
#define __HJVXREADCONFIGENTRIES_H__

#include "jvx.h"
#include "jvx_allocate.h"
#include <vector>
#include <string>

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dIntList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxSize idx, std::vector<jvxInt32>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(idx < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, idx);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(JVX_DATA2INT32(lst->bDblList[i]));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dDataList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxSize idx, std::vector<jvxData>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(idx < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, idx);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(lst->bDblList[i]);
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_nameEntry(IjvxConfigProcessor* theReader, jvxConfigData* theSection, std::string& entry)
{
	jvxErrorType res = JVX_NO_ERROR;
	jvxString* fldStr = NULL;
	theReader->getNameCurrentEntry(theSection, &fldStr);
	if(fldStr)
	{
		entry = fldStr->bString;
		theReader->deallocate(fldStr);
	}
	else
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_originEntry(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::string& fileName, jvxInt32& lineno)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxInt32 valI = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			theReader->getOriginSection(datTmp, &fldStr, &lineno);
			if(fldStr)
			{
				fileName = fldStr->bString;
				theReader->deallocate(fldStr);
			}
			else
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
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_originEntry(IjvxConfigProcessor* theReader, jvxConfigData* theSection, std::string& fileName, jvxInt32& lineno)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxString* fldStr = NULL;
	jvxInt32 valI = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getOriginSection(theSection, &fldStr, &lineno);
		if(fldStr)
		{
			fileName = fldStr->bString;
			theReader->deallocate(fldStr);
		}
		else
		{
			res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
		}
	}
	else
	{
		res = JVX_ERROR_ELEMENT_NOT_FOUND;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_references(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::string& nmKey, jvxBool* isAbsolute, std::vector<std::string>& lstReference)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getReferenceKey_allocate(datTmp, &fldStr);
			if(res == JVX_NO_ERROR)
			{
				if(fldStr)
				{
					nmKey = fldStr->bString;
					theReader->deallocate(fldStr);
					fldStr = NULL;
					theReader->getReferenceNumberPathTokens(datTmp, &num);
					for(i = 0; i < num; i++)
					{
						fldStr = NULL;
						theReader->getReferencePathToken_id_allocate(datTmp, &fldStr, i);
						if(fldStr)
						{
							lstReference.push_back(fldStr->bString);
							theReader->deallocate(fldStr);
						}
					}
					theReader->getReferencePathType(datTmp, isAbsolute);
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			} 
			else
			{ 
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Read single entries in config file 
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentData(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxData* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = valD;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentInt8(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt8* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = JVX_DATA2INT8(valD);
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentInt16(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt16* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = JVX_DATA2INT16(valD);
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentInt32(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt32* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = JVX_DATA2INT32(valD);
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentInt64(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt64* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = JVX_DATA2INT64(valD);
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}


inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentBitField(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxBitField* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = JVX_DATA2BITFIELD(valD);
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

template <typename T> inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentSpec(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, T* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxData valD = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			res = theReader->getAssignmentValue(datTmp, &valD);
			if(res == JVX_NO_ERROR)
			{
				if(entry)
				{
					*entry = (T)(JVX_DATA2INT16(valD));
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
/**********************************************************************************
 * Read array numeric entries in config file, size known in advance 
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dDataList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxData* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						if(entries)
						{
							for(i = 0; i < lst->lDblList; i++)
							{
								if(i < numValues)
								{
									entries[i] = lst->bDblList[i];
								}
								else
								{
									break;
								}
							}
							theReader->deallocate(lst);
						}
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dDataList_allocate(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken,  jvxData** entries, jvxSize* numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries && numValues)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						JVX_DSP_SAFE_ALLOCATE_FIELD(*entries, jvxData, lst->lDblList);
						*numValues = lst->lDblList;
						for(i = 0; i < lst->lDblList; i++)
						{
							if(i < *numValues)
							{
								(*entries)[i] = (jvxData)lst->bDblList[i];
							}
							else
							{
								break;
							}
						}
						theReader->deallocate(lst);
					}
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt8List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt8* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						if(entries)
						{
							for(i = 0; i < lst->lDblList; i++)
							{
								if(i < numValues)
								{
									entries[i] = JVX_DATA2INT8(lst->bDblList[i]);
								}
								else
								{
									break;
								}
							}
							theReader->deallocate(lst);
						}
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt16List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt16* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						if(entries)
						{
							for(i = 0; i < lst->lDblList; i++)
							{
								if(i < numValues)
								{
									entries[i] = JVX_DATA2INT16(lst->bDblList[i]);
								}
								else
								{
									break;
								}
							}
							theReader->deallocate(lst);
						}
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt32List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt32* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						if(entries)
						{
							for(i = 0; i < lst->lDblList; i++)
							{
								if(i < numValues)
								{
									entries[i] = JVX_DATA2INT32(lst->bDblList[i]);
								}
								else
								{
									break;
								}
							}
							theReader->deallocate(lst);
						}
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt64List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxInt64* entries, jvxSize numValues)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						if(entries)
						{
							for(i = 0; i < lst->lDblList; i++)
							{
								if(i < numValues)
								{
									entries[i] = JVX_DATA2INT64(lst->bDblList[i]);
								}
								else
								{
									break;
								}
							}
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Read array numeric entries in config file, size known in advance 
 *********************************************************************************/

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dDataList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxData>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(lst->bDblList[i]);
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
				


inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt8List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt8>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(JVX_DATA2INT8(lst->bDblList[i]));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt16List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt16>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(JVX_DATA2INT16(lst->bDblList[i]));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt32List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt32>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(JVX_DATA2INT32(lst->bDblList[i]));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_1dInt64List(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<jvxInt64>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back(JVX_DATA2INT64(lst->bDblList[i]));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

template <typename T> inline jvxErrorType
HjvxConfigProcessor_readEntry_1dSpecList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<T>& entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	entries.clear();

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, 0);
					if(lst)
					{
						for(i = 0; i < lst->lDblList; i++)
						{
							entries.push_back((T)(JVX_DATA2INT16(lst->bDblList[i])));
						}
						theReader->deallocate(lst);
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
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

/**********************************************************************************
 * Read string/string list entries in config file 
 *********************************************************************************/
inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentStringList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::vector<std::string>* entries)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberStrings(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				for(i = 0; i < num; i++)
				{
					theReader->getString_id_allocate(datTmp, &fldStr, i);
					if(fldStr)
					{
						if(entries)
						{
							entries->push_back(fldStr->bString);
						}
						theReader->deallocate(fldStr);
					}
					else
					{
						res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
					}
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentSelectionList(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvx_oneSelectionOption_cpp* entries, bool onlySelectionToConfig = false)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL, *datTmp1 = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceSubsectionCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			if(!onlySelectionToConfig)
			{
				entries->entries.clear();
				HjvxConfigProcessor_readEntry_assignmentStringList(theReader, datTmp, JVX_TOKEN_CONFIG_SELECTIONLIST_NAMES, &entries->entries);
				HjvxConfigProcessor_readEntry_assignmentBitField(theReader, datTmp, JVX_TOKEN_CONFIG_EXCLUSIVE_VALUE, &entries->exclusive);
			}
			HjvxConfigProcessor_readEntry_assignmentBitField(theReader, datTmp, JVX_TOKEN_CONFIG_SELECTION_VALUE, &entries->selection);
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}





inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentString(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, std::string* entry)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;

	if(theReader && theSection)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getAssignmentString_allocate(datTmp, &fldStr);
			if(res == JVX_NO_ERROR)
			{
				if(fldStr)
				{
					if(entry)
					{
						*entry = fldStr->bString;
					}
					theReader->deallocate(fldStr);
				}
				else
				{ 
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

inline jvxErrorType
HjvxConfigProcessor_readEntry_assignmentValueInRange(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken, jvxValueInRange* entries, bool onlySelectionToConfig = false)
{
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL, *datTmp1 = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;

	if(theReader && theSection)
	{
		theReader->getReferenceSubsectionCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			if(!onlySelectionToConfig)
			{
				HjvxConfigProcessor_readEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE_MIN, &entries->minVal);
				HjvxConfigProcessor_readEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE_MAX, &entries->maxVal);
			}
				HjvxConfigProcessor_readEntry_assignmentData(theReader, datTmp, JVX_TOKEN_CONFIG_VALUE_IN_RANGE, &entries->val);
		}
		else
		{ 
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}
inline jvxErrorType
HjvxConfigProcessor_readEntry_1dDataList_allocate_circ(IjvxConfigProcessor* theReader, jvxConfigData* theSection, const std::string& nmToken,  jvxData** entries, jvxSize* numValues, jvxSize& idxEntry)
{
	jvxSize i;
	jvxErrorType res = JVX_NO_ERROR;

	jvxConfigData* datTmp = NULL;
	jvxString* fldStr = NULL;
	jvxSize num = 0;
	jvxDataList* lst = NULL;

	if(theReader && theSection && entries && numValues)
	{
		theReader->getReferenceEntryCurrentSection_name(theSection, &datTmp, nmToken.c_str());
		if(datTmp)
		{
			fldStr = NULL;
			res = theReader->getNumberValueLists(datTmp, &num);
			if(res == JVX_NO_ERROR)
			{
				if(0 < num)
				{
					idxEntry = idxEntry % num;
					lst = NULL;
					theReader->getValueList_id_allocate(datTmp, &lst, idxEntry);
					if(lst)
					{
						JVX_DSP_SAFE_ALLOCATE_FIELD(*entries, jvxData, lst->lDblList);
						*numValues = lst->lDblList;
						for(i = 0; i < lst->lDblList; i++)
						{
							if(i < *numValues)
							{
								(*entries)[i] = (jvxData)lst->bDblList[i];
							}
							else
							{
								break;
							}
						}
						theReader->deallocate(lst);
					}
				}
				else
				{
					res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
				}
			}
			else
			{
				res = JVX_ERROR_WRONG_SECTION_TYPE;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
	}
	else
	{ 
		res = JVX_ERROR_INVALID_ARGUMENT;
	}
	return(res);
}

#endif
