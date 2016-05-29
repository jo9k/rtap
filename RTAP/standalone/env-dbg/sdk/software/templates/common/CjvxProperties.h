#ifndef __CJVXPROPERTIES_H__
#define __CJVXPROPERTIES_H__

#include "jvx.h"


class CjvxProperties
{
protected:
	typedef struct
	{
		jvxHandle* fld;
		jvxSize num;
		jvxDataFormat format;
		jvxPropertyCategoryType cat;
		jvxSize id;
		jvxUInt64 allow_state_mask;
		jvxUInt64 allow_threading_mask;
		std::string name;
		std::string description;
		std::string descriptor;
		jvxPropertyAccessType accessType;
		jvxBool* isValid;
		jvxBool* isFrozen;
		jvxPropertyDecoderHintType decTp;
		jvxPropertyContext context;
		struct
		{
			jvxPropertyAccessType accessType;
			jvxBool* isValid;
			jvxHandle* fld;
			jvxSize num;
		} shadow;
	} oneProperty;

	struct
	{
		JVX_MUTEX_HANDLE csec;
		jvxBool useLock;
		JVX_THREAD_ID threadId;
		std::vector<oneProperty> registeredProperties;
		jvxSize nextPropId;
		jvxSize propSetRevsision;
	} _common_set_properties;

public:

	CjvxProperties()
	{
		JVX_INITIALIZE_MUTEX(_common_set_properties.csec);
		_common_set_properties.useLock = false;
		_common_set_properties.threadId = JVX_NULLTHREAD;
		_common_set_properties.nextPropId = 1;
		_common_set_properties.propSetRevsision = 0;
	};

	 ~CjvxProperties()
	 {
		 _common_set_properties.useLock = false;
		 _common_set_properties.threadId = JVX_NULLTHREAD;
		 JVX_TERMINATE_MUTEX(_common_set_properties.csec);
	 };

 	 inline jvxErrorType _reset_properties()
	 {
		 _common_set_properties.registeredProperties.clear();
		 _common_set_properties.nextPropId = 1;
		 return JVX_NO_ERROR;
	 };

	 inline jvxErrorType _number_properties(jvxSize* num)
	 {
		 if(num)
		 {
			 *num = _common_set_properties.registeredProperties.size();
		 }
		 return JVX_NO_ERROR;
	 };

	 inline jvxErrorType _get_revision(jvxSize* revision)
	 {
		 if(revision)
			 *revision = _common_set_properties.propSetRevsision;
		 return JVX_NO_ERROR;
	 }


	 jvxErrorType _lock_properties()
	 {
		 JVX_LOCK_MUTEX(_common_set_properties.csec);
		 return JVX_NO_ERROR;
	 };

	 inline jvxErrorType _trylock_properties()
	 {
		 JVX_TRY_LOCK_MUTEX_RESULT_TYPE res = JVX_TRY_LOCK_MUTEX_SUCCESS;
		 JVX_TRY_LOCK_MUTEX(res, _common_set_properties.csec);
		 if(res == JVX_TRY_LOCK_MUTEX_SUCCESS)
		 {
			 return JVX_NO_ERROR;
		 }
		 return JVX_ERROR_COMPONENT_BUSY;
	 };

	 inline jvxErrorType _unlock_properties()
	 {
		 JVX_UNLOCK_MUTEX(_common_set_properties.csec);
		 return JVX_NO_ERROR;
	 };

	 inline jvxErrorType _description_property(jvxSize idx,
						   jvxPropertyCategoryType* category,
						   jvxUInt64* allowStateMask,
						   jvxUInt64* allowThreadingMask,
						   jvxDataFormat* format,
						   jvxSize* num,
						   jvxPropertyAccessType* accessType,
						   jvxPropertyDecoderHintType* decTp,
						   jvxSize* handleIdx,
						   jvxPropertyContext* theContxt,
						   jvxString** name,
						   jvxString** description,
						   jvxString** descriptor,
						   bool* isValid)
	 {
		 jvxErrorType res = JVX_NO_ERROR;
 		_lock_properties();

		 if(idx < _common_set_properties.registeredProperties.size())
		 {
			 if(category)
			 {
				 *category = _common_set_properties.registeredProperties[idx].cat;
			 }
			 if(allowStateMask)
			 {
				 *allowStateMask = _common_set_properties.registeredProperties[idx].allow_state_mask;
			 }
			 if(allowThreadingMask)
			 {
				 *allowThreadingMask= _common_set_properties.registeredProperties[idx].allow_threading_mask;
			 }
			 if(format)
			 {
				 *format = _common_set_properties.registeredProperties[idx].format;
			 }
			 if(accessType)
			 {
				 *accessType = _common_set_properties.registeredProperties[idx].accessType;
			 }
			 if(decTp)
			 {
				 *decTp = _common_set_properties.registeredProperties[idx].decTp;
			 }
			 if(num)
			 {
				 *num = _common_set_properties.registeredProperties[idx].num;
			 }
			 if(handleIdx)
			 {
				 *handleIdx = _common_set_properties.registeredProperties[idx].id;
			 }
			 if(theContxt)
			 {
				 *theContxt = _common_set_properties.registeredProperties[idx].context;
			 }

			 HjvxObject_produceString(name, _common_set_properties.registeredProperties[idx].name);

			 HjvxObject_produceString(description, _common_set_properties.registeredProperties[idx].description);

			 HjvxObject_produceString(descriptor, _common_set_properties.registeredProperties[idx].descriptor);

			 if(isValid)
			 {
				 *isValid = *_common_set_properties.registeredProperties[idx].isValid;
			 }

			 res = JVX_NO_ERROR;
		 }
		 else
		 {
			 res = JVX_ERROR_ID_OUT_OF_BOUNDS;
		 }

		 _unlock_properties();

		 return(res);

	 };

	 inline jvxErrorType _description_property(
						   const char* descriptor,
						   jvxDataFormat* format,
						   jvxSize* num,
   						   jvxSize* handleIdx, 
						   jvxPropertyCategoryType* category)
	 {
		 jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;

		_lock_properties();

		selection = findInPropertyList(_common_set_properties.registeredProperties, descriptor);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			if(format)
			{
				*format = selection->format;
			}
			if(num)
			{
				*num = selection->num;
			}
			if(handleIdx)
			{
				*handleIdx = selection->id;
			}
			if(category)
			{
				*category = selection->cat;
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		_unlock_properties();

		 return res;
	 };

	 inline jvxErrorType _set_property(jvxHandle* fld,
					   jvxSize numElements,
					   jvxDataFormat format,
					   jvxSize propId,
					   jvxPropertyCategoryType category,
					   jvxSize offsetStart,
					   jvxBool contentOnly,
					   jvxPropertyCallPurpose callPurpose,
					   IjvxObject* theObj)
	 {
		 jvxSize i;
		 jvxErrorType res = JVX_NO_ERROR;
		 std::vector<oneProperty>::iterator selection;
		 jvxState stat = JVX_STATE_NONE;
		 jvxSize maxSpace = 0;
		 jvxSize szElem = 0;

		 _lock_properties();

		 selection = findInPropertyList(_common_set_properties.registeredProperties, propId, category);
		 if(selection != _common_set_properties.registeredProperties.end())
		 {
			 if(format != selection->format)
			 {
				 res = JVX_ERROR_INVALID_ARGUMENT;
			 }
			 if(theObj)
			 {
				 theObj->state(&stat);
				 if((stat & selection->allow_state_mask) == 0)
				 {
					 res = JVX_ERROR_WRONG_STATE;
				 }
			 }
			 if(contentOnly)
			 {
				 if(selection->accessType == JVX_PROPERTY_ACCESS_READ_ONLY)
				 {
					res = JVX_ERROR_READ_ONLY;
				 }
			 }
			 else
			 {
				 if(selection->accessType != JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE)
				 {
					res = JVX_ERROR_READ_ONLY;
				 }
			 }
		 }
		 else
		 {
			 res = JVX_ERROR_ELEMENT_NOT_FOUND;
		 }

		 if(res == JVX_NO_ERROR)
		 {

			if(selection->fld)
			{
				switch(format)
				{
				case JVX_DATAFORMAT_STRING:
					if(numElements == 1)
					{
						std::string* str = (std::string*)selection->fld;
						if(fld)
						{
							*str = ((jvxString*) fld)->bString;
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_STRING_LIST:
					if(numElements == 1)
					{
						std::vector<std::string>* strL = (std::vector<std::string>*)selection->fld;
						strL->clear();
						jvxStringList* strLIn = (jvxStringList*)fld;
						if(strLIn)
						{
							for(i = 0; i < strLIn->lStrings; i++)
							{
								strL->push_back(strLIn->bStrings[i].bString);
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;

				case JVX_DATAFORMAT_SELECTION_LIST:
					if(numElements == 1)
					{
						jvx_oneSelectionOption_cpp* selL = (jvx_oneSelectionOption_cpp*)selection->fld;
						jvxSelectionList* selLIn = (jvxSelectionList*)fld;

						if(contentOnly)
						{
							selL->selection = selLIn->bitFieldSelected;
						}
						else
						{
							selL->entries.clear();
							if(selLIn)
							{
								if(selLIn->strList)
								{
									for(i = 0; i < selLIn->strList->lStrings; i++)
									{
										selL->entries.push_back(selLIn->strList->bStrings[i].bString);
									}
								}
								selL->selection = selLIn->bitFieldSelected;
								selL->exclusive = selLIn->bitFieldExclusive;
							}
							else
							{
								res = JVX_ERROR_INVALID_ARGUMENT;
							}
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;

				case JVX_DATAFORMAT_VALUE_IN_RANGE:
					if(numElements == 1)
					{
						jvxValueInRange* selL = (jvxValueInRange*)selection->fld;
						jvxValueInRange* selVal = (jvxValueInRange*)fld;

						if(!contentOnly)
						{
							selL->maxVal = selVal->maxVal;
							selL->minVal = selVal->minVal;
						}
						selL->val = selVal->val;
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_DATA:
					if(numElements + offsetStart <= selection->num)
					{
						jvxData* ptrOut = (jvxData*)selection->fld;
						ptrOut += offsetStart;
						jvxData* ptrIn = (jvxData*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxData) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_8BIT:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt8* ptrOut = (jvxInt8*)selection->fld;
						ptrOut += offsetStart;
						jvxInt8* ptrIn = (jvxInt8*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt8) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_16BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt16* ptrOut = (jvxInt16*)selection->fld;
						ptrOut += offsetStart;
						jvxInt16* ptrIn = (jvxInt16*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt16) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_24BIT_LE:
				case JVX_DATAFORMAT_32BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt32* ptrOut = (jvxInt32*)selection->fld;
						ptrOut += offsetStart;
						jvxInt32* ptrIn = (jvxInt32*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt32) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_64BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt64* ptrOut = (jvxInt64*)selection->fld;
						ptrOut += offsetStart;
						jvxInt64* ptrIn = (jvxInt64*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt64) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				default:
					maxSpace = (selection->num - offsetStart);
					szElem = jvx_numberBytesField(1, format, 1);
					if(numElements <= maxSpace)
					{
						memcpy(((jvxByte*)selection->fld + (szElem * offsetStart)), fld, szElem * numElements);
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
				}
			}
			else
			{
				res = JVX_ERROR_NOT_READY;
			}

		 }

		 _unlock_properties();

		 return res;
	 };

	 virtual jvxErrorType _set_property(jvxHandle* fld,
					  jvxSize offsetStart,
					  jvxSize numElements,
					  jvxDataFormat format,
					  jvxBool contentOnly,
					  const char* descriptor,
					  IjvxObject* theObj)
	{
		jvxSize i;
		 jvxErrorType res = JVX_NO_ERROR;
		 std::vector<oneProperty>::iterator selection;
		 jvxState stat = JVX_STATE_NONE;
		 jvxSize maxSpace = 0;
		 jvxSize szElem = 0;

		 _lock_properties();

		 selection = findInPropertyList(_common_set_properties.registeredProperties, descriptor);
		 if(selection != _common_set_properties.registeredProperties.end())
		 {
			 if(format != selection->format)
			 {
				 res = JVX_ERROR_INVALID_ARGUMENT;
			 }
			 if(theObj)
			 {
				 theObj->state(&stat);
				 if((stat & selection->allow_state_mask) == 0)
				 {
					 res = JVX_ERROR_WRONG_STATE;
				 }
			 }
			 if(contentOnly)
			 {
				 if(selection->accessType == JVX_PROPERTY_ACCESS_READ_ONLY)
				 {
					res = JVX_ERROR_READ_ONLY;
				 }
			 }
			 else
			 {
				 if(selection->accessType != JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE)
				 {
					res = JVX_ERROR_READ_ONLY;
				 }
			 }
		 }
		 else
		 {
			 res = JVX_ERROR_ELEMENT_NOT_FOUND;
		 }

		 if(res == JVX_NO_ERROR)
		 {
			if(selection->fld)
			{
				switch(format)
				{
				case JVX_DATAFORMAT_STRING:
					if(numElements == 1)
					{
						std::string* str = (std::string*)selection->fld;
						if(fld)
						{
							*str = ((jvxString*) fld)->bString;
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_STRING_LIST:
					if(numElements == 1)
					{
						std::vector<std::string>* strL = (std::vector<std::string>*)selection->fld;
						strL->clear();
						jvxStringList* strLIn = (jvxStringList*)fld;
						if(strLIn)
						{
							for(i = 0; i < strLIn->lStrings; i++)
							{
								strL->push_back(strLIn->bStrings[i].bString);
							}
						}
						else
						{
							res = JVX_ERROR_INVALID_ARGUMENT;
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;

				case JVX_DATAFORMAT_SELECTION_LIST:
					if(numElements == 1)
					{
						jvx_oneSelectionOption_cpp* selL = (jvx_oneSelectionOption_cpp*)selection->fld;
						jvxSelectionList* selLIn = (jvxSelectionList*)fld;

						if(contentOnly)
						{
							selL->selection = selLIn->bitFieldSelected;
						}
						else
						{
							selL->entries.clear();
							if(selLIn)
							{
								if(selLIn->strList)
								{
									for(i = 0; i < selLIn->strList->lStrings; i++)
									{
										selL->entries.push_back(selLIn->strList->bStrings[i].bString);
									}
								}
								selL->selection = selLIn->bitFieldSelected;
								selL->exclusive = selLIn->bitFieldExclusive;
							}
							else
							{
								res = JVX_ERROR_INVALID_ARGUMENT;
							}
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;

				case JVX_DATAFORMAT_VALUE_IN_RANGE:
					if(numElements == 1)
					{
						jvxValueInRange* selL = (jvxValueInRange*)selection->fld;
						jvxValueInRange* selVal = (jvxValueInRange*)fld;

						if(!contentOnly)
						{
							selL->maxVal = selVal->maxVal;
							selL->minVal = selVal->minVal;
						}
						selL->val = selVal->val;
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_DATA:
					if(numElements + offsetStart <= selection->num)
					{
						jvxData* ptrOut = (jvxData*)selection->fld;
						ptrOut += offsetStart;
						jvxData* ptrIn = (jvxData*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxData) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_8BIT:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt8* ptrOut = (jvxInt8*)selection->fld;
						ptrOut += offsetStart;
						jvxInt8* ptrIn = (jvxInt8*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt8) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_16BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt16* ptrOut = (jvxInt16*)selection->fld;
						ptrOut += offsetStart;
						jvxInt16* ptrIn = (jvxInt16*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt16) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_24BIT_LE:
				case JVX_DATAFORMAT_32BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt32* ptrOut = (jvxInt32*)selection->fld;
						ptrOut += offsetStart;
						jvxInt32* ptrIn = (jvxInt32*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt32) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_64BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt64* ptrOut = (jvxInt64*)selection->fld;
						ptrOut += offsetStart;
						jvxInt64* ptrIn = (jvxInt64*)fld;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt64) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				default:
					maxSpace = (selection->num - offsetStart);
					szElem = jvx_numberBytesField(1, format, 1);
					if(numElements <= maxSpace)
					{
						memcpy(((jvxByte*)selection->fld + (szElem * offsetStart)), fld, szElem * numElements);
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
				}
			}
			else
			{
				res = JVX_ERROR_NOT_READY;
			}

		 }

		 _unlock_properties();

		 return res;
	}

	 inline jvxErrorType _set_property_ext_ref(jvxHandle* fld,
						   jvxBool* isValid,
						   jvxSize numElements,
						   jvxDataFormat format,
						   jvxSize propId,
						   jvxPropertyCategoryType category,
						   jvxPropertyDecoderHintType htTp)
	 {
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		selection = findInPropertyList(_common_set_properties.registeredProperties, propId, category);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			if(format != selection->format)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}

			if(res == JVX_NO_ERROR)
			{
				switch(selection->format)
				{
				case JVX_DATAFORMAT_DATA:

					if(selection->decTp != htTp)
					{
						res = JVX_ERROR_UNSUPPORTED;
					}

					// Only support for jvxData fields <TEMP>
					if(selection->decTp != JVX_PROPERTY_DECODER_MULTI_CHANNEL_CIRCULAR_BUFFER)
					{
						res = JVX_ERROR_UNSUPPORTED;
					}
					break;
				default:
					res = JVX_ERROR_UNSUPPORTED;
				}
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		if(res == JVX_NO_ERROR)
		{
			if(fld)
			{
				// In case of external references, the jvxHandle* was assigned as a jvxHandle** when calling register originally!

				jvxHandle** originPtr = (jvxHandle**)selection->fld;
				if(*originPtr == NULL)
				{
					oneMultichannelCircBufferHeader* headerFld = (oneMultichannelCircBufferHeader*)fld;
					jvxSize expSize = jvx_numberBytesField(headerFld->bLength, selection->format,  headerFld->numChannels);
					expSize += sizeof(oneMultichannelCircBufferHeader);

					if(expSize == numElements)
					{
						// Safe old values
						selection->shadow.accessType = selection->accessType;
						selection->shadow.isValid = selection->isValid;
						selection->shadow.fld = selection->fld;
						selection->shadow.num = selection->num;

						selection->accessType = JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE;
						selection->isValid = isValid;

						*originPtr = fld;
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
				}
				else
				{
					res = JVX_ERROR_ALREADY_IN_USE;
				}
			}
			else
			{
				// "Unlink" the circ buffer field
				jvxHandle** originPtr = (jvxHandle**)selection->fld;
				*originPtr = NULL;

				selection->accessType = selection->shadow.accessType;
				selection->isValid = selection->shadow.isValid;
				selection->fld = selection->shadow.fld;
				selection->num = selection->shadow.num;
				if(selection->isValid)
				{
					*selection->isValid = false;				
				}
			}
		}

		_unlock_properties();

		return res;
	};

	
	inline jvxErrorType _set_property_ext_ref(jvxHandle* fld,
					  jvxSize numElements,
				      jvxDataFormat format,
				      const char* descriptor,
  					  jvxPropertyDecoderHintType htTp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		selection = findInPropertyList(_common_set_properties.registeredProperties, descriptor);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			if(format != selection->format)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}

			if(res == JVX_NO_ERROR)
			{
				switch(selection->format)
				{
				case JVX_DATAFORMAT_DATA:

					// Only support for jvxData fields
					if(selection->decTp != htTp)
					{
						res = JVX_ERROR_UNSUPPORTED;
					}

					// Temp check to be removed lateron
					if(selection->decTp != JVX_PROPERTY_DECODER_MULTI_CHANNEL_CIRCULAR_BUFFER)
					{
						res = JVX_ERROR_UNSUPPORTED;
					}

					break;
				default:
					res = JVX_ERROR_UNSUPPORTED;
				}
			}
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		if(res == JVX_NO_ERROR)
		{
			if(fld)
			{
				// In case of external references, the jvxHandle* was assigned as a jvxHandle** when calling register originally!

				jvxHandle** originPtr = (jvxHandle**)selection->fld;
				if(*originPtr == NULL)
				{
					oneMultichannelCircBufferHeader* headerFld = (oneMultichannelCircBufferHeader*)fld;
					jvxSize expSize = jvx_numberBytesField(headerFld->bLength, selection->format,  headerFld->numChannels);
					expSize += sizeof(oneMultichannelCircBufferHeader);

					if(expSize == numElements)
					{
						// Safe old values
						selection->shadow.accessType = selection->accessType;
						selection->shadow.isValid = selection->isValid;
						selection->shadow.fld = selection->fld;
						selection->shadow.num = selection->num;

						selection->accessType = JVX_PROPERTY_ACCESS_FULL_READ_AND_WRITE;						

						*originPtr = fld;
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
				}
				else
				{
					res = JVX_ERROR_ALREADY_IN_USE;
				}
			}
			else
			{
				// "Unlink" the circ buffer field
				jvxHandle** originPtr = (jvxHandle**)selection->fld;
				*originPtr = NULL;

				selection->accessType = selection->shadow.accessType;
				selection->isValid = selection->shadow.isValid;
				selection->fld = selection->shadow.fld;
				selection->num = selection->shadow.num;
				*selection->isValid = false;
			}
		}

		_unlock_properties();

		return res;
	};

	 inline jvxErrorType _get_property(jvxHandle* fld,
					  jvxSize numElements,
					  jvxDataFormat format,
					  jvxSize propId,
					  jvxPropertyCategoryType category,
					  jvxSize offsetStart,
					  jvxBool contentOnly,
					  jvxBool* isValid,
					  jvxPropertyAccessType* accessType,
					  jvxPropertyCallPurpose callPurpose,
					  IjvxObject* theObj)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		selection = findInPropertyList(_common_set_properties.registeredProperties, propId, category);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			if(format != selection->format)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			if(theObj)
			{
				theObj->state(&stat);
				if((stat & selection->allow_state_mask) == 0)
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}

			if(selection->accessType == JVX_PROPERTY_ACCESS_WRITE_ONLY)
			{
				res = JVX_ERROR_WRITE_ONLY;
			}

		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		if(res == JVX_NO_ERROR)
		{

			if(accessType)
			{
				*accessType = selection->accessType;
			}

			if(isValid)
			{
				if(selection->isValid)
				{
					*isValid = *selection->isValid;
				}
				else
				{
					*isValid = true;
				}
			}
			else
			{
				if(selection->isValid)
				{
					if(!*selection->isValid)
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
				}
			}
		}

		if(res == JVX_NO_ERROR)
		{
			if(selection->fld)
			{
				switch(format)
				{
				case JVX_DATAFORMAT_STRING:
					if(numElements == 1)
					{
						std::string* str = (std::string*)selection->fld;
						HjvxObject_produceString((jvxString**)fld, *str);
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_STRING_LIST:
					if(numElements == 1)
					{
						std::vector<std::string>* strL = (std::vector<std::string>*)selection->fld;
						HjvxObject_produceStringList((jvxStringList**)fld, *strL);
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_SELECTION_LIST:
					if(numElements == 1)
					{
						jvx_oneSelectionOption_cpp* selL = (jvx_oneSelectionOption_cpp*)selection->fld;
						jvxSelectionList* returnVal = (jvxSelectionList*)fld;
						returnVal->bitFieldSelected = selL->selection;
						returnVal->bitFieldExclusive = selL->exclusive;
						if(contentOnly)
						{
							if(returnVal->strList)
							{
								returnVal->strList->lStrings= selL->entries.size();
							}
						}
						else
						{
							HjvxObject_produceStringList(&returnVal->strList, selL->entries);
						}
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_VALUE_IN_RANGE:
					if(numElements == 1)
					{
						jvxValueInRange* selL = (jvxValueInRange*)selection->fld;
						jvxValueInRange* selVal = (jvxValueInRange*)fld;

//						if(!contentOnly)
//						{
							selVal->maxVal = selL->maxVal;
							selVal->minVal = selL->minVal;
//						}
						selVal->val = selL->val;
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
					break;
				case JVX_DATAFORMAT_DATA:
					if(numElements + offsetStart <= selection->num)
					{
						jvxData* ptrOut = (jvxData*)fld;
						jvxData* ptrIn = (jvxData*)selection->fld;
						ptrIn += offsetStart;
						memcpy(ptrOut, ptrIn, sizeof(jvxData) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_8BIT:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt8* ptrOut = (jvxInt8*)fld;
						jvxInt8* ptrIn = (jvxInt8*)selection->fld;
						ptrIn += offsetStart;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt8) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_16BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt16* ptrOut = (jvxInt16*)fld;
						jvxInt16* ptrIn = (jvxInt16*)selection->fld;
						ptrIn += offsetStart;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt16) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_24BIT_LE:
				case JVX_DATAFORMAT_32BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt32* ptrOut = (jvxInt32*)fld;
						jvxInt32* ptrIn = (jvxInt32*)selection->fld;
						ptrIn += offsetStart;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt32) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				case JVX_DATAFORMAT_64BIT_LE:
					if(numElements + offsetStart <= selection->num)
					{
						jvxInt64* ptrOut = (jvxInt64*)fld;
						jvxInt64* ptrIn = (jvxInt64*)selection->fld;
						ptrIn += offsetStart;
						memcpy(ptrOut, ptrIn, sizeof(jvxInt64) * numElements);
					}
					else
					{
						res = JVX_ERROR_INVALID_SETTING;
					}
					break;
				default:
					maxSpace = (selection->num - offsetStart);
					szElem = jvx_numberBytesField(1, format, 1);
					if(numElements <= maxSpace)
					{
						memcpy(fld, ((jvxByte*)selection->fld + (szElem * offsetStart)), szElem * numElements);
					}
					else
					{
						res = JVX_ERROR_SIZE_MISMATCH;
					}
				}
			}
			else
			{
				if(isValid)
				{
					*isValid = false;
				}
				else
				{
					res = JVX_ERROR_INVALID_SETTING;
				}
			}
		}

		_unlock_properties();

		return res;
	};

	inline jvxErrorType _get_property(jvxHandle* fld,
		jvxSize offsetStart,
		jvxSize numElements,
		jvxDataFormat format,
		jvxBool contentOnly, 
		const char* descriptor,
		IjvxObject* theObj)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		selection = findInPropertyList(_common_set_properties.registeredProperties, descriptor);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			if(format != selection->format)
			{
				res = JVX_ERROR_INVALID_ARGUMENT;
			}
			if(theObj)
			{
				theObj->state(&stat);
				if((stat & selection->allow_state_mask) == 0)
				{
					res = JVX_ERROR_WRONG_STATE;
				}
			}

			if(selection->accessType == JVX_PROPERTY_ACCESS_WRITE_ONLY)
			{
				res = JVX_ERROR_WRITE_ONLY;
			}

		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}

		if(res == JVX_NO_ERROR)
		{
			if(selection->fld)
			{
				if(selection->isValid)
				{
					switch(format)
					{
					case JVX_DATAFORMAT_STRING:
						if(numElements == 1)
						{
							std::string* str = (std::string*)selection->fld;
							HjvxObject_produceString((jvxString**)fld, *str);
						}
						else
						{
							res = JVX_ERROR_SIZE_MISMATCH;
						}
						break;
					case JVX_DATAFORMAT_STRING_LIST:
						if(numElements == 1)
						{
							std::vector<std::string>* strL = (std::vector<std::string>*)selection->fld;
							HjvxObject_produceStringList((jvxStringList**)fld, *strL);
						}
						else
						{
							res = JVX_ERROR_SIZE_MISMATCH;
						}
						break;
					case JVX_DATAFORMAT_SELECTION_LIST:
						if(numElements == 1)
						{
							jvx_oneSelectionOption_cpp* selL = (jvx_oneSelectionOption_cpp*)selection->fld;
							jvxSelectionList* returnVal = (jvxSelectionList*)fld;
							returnVal->bitFieldSelected = selL->selection;
							returnVal->bitFieldExclusive = selL->exclusive;
							if(contentOnly)
							{
								if(returnVal->strList)
								{
									returnVal->strList->lStrings= selL->entries.size();
								}
							}
							else
							{
								HjvxObject_produceStringList(&returnVal->strList, selL->entries);
							}
						}
						else
						{
							res = JVX_ERROR_SIZE_MISMATCH;
						}
						break;
					case JVX_DATAFORMAT_VALUE_IN_RANGE:
						if(numElements == 1)
						{
							jvxValueInRange* selL = (jvxValueInRange*)selection->fld;
							jvxValueInRange* selVal = (jvxValueInRange*)fld;

							//						if(!contentOnly)
							//						{
							selVal->maxVal = selL->maxVal;
							selVal->minVal = selL->minVal;
							//						}
							selVal->val = selL->val;
						}
						else
						{
							res = JVX_ERROR_SIZE_MISMATCH;
						}
						break;
					case JVX_DATAFORMAT_DATA:
						if(numElements + offsetStart <= selection->num)
						{
							jvxData* ptrOut = (jvxData*)fld;
							jvxData* ptrIn = (jvxData*)selection->fld;
							ptrIn += offsetStart;
							memcpy(ptrOut, ptrIn, sizeof(jvxData) * numElements);
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
						break;
					case JVX_DATAFORMAT_8BIT:
						if(numElements + offsetStart <= selection->num)
						{
							jvxInt8* ptrOut = (jvxInt8*)fld;
							jvxInt8* ptrIn = (jvxInt8*)selection->fld;
							ptrIn += offsetStart;
							memcpy(ptrOut, ptrIn, sizeof(jvxInt8) * numElements);
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
						break;
					case JVX_DATAFORMAT_16BIT_LE:
						if(numElements + offsetStart <= selection->num)
						{
							jvxInt16* ptrOut = (jvxInt16*)fld;
							jvxInt16* ptrIn = (jvxInt16*)selection->fld;
							ptrIn += offsetStart;
							memcpy(ptrOut, ptrIn, sizeof(jvxInt16) * numElements);
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
						break;
					case JVX_DATAFORMAT_24BIT_LE:
					case JVX_DATAFORMAT_32BIT_LE:
						if(numElements + offsetStart <= selection->num)
						{
							jvxInt32* ptrOut = (jvxInt32*)fld;
							jvxInt32* ptrIn = (jvxInt32*)selection->fld;
							ptrIn += offsetStart;
							memcpy(ptrOut, ptrIn, sizeof(jvxInt32) * numElements);
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
						break;
					case JVX_DATAFORMAT_64BIT_LE:
						if(numElements + offsetStart <= selection->num)
						{
							jvxInt64* ptrOut = (jvxInt64*)fld;
							jvxInt64* ptrIn = (jvxInt64*)selection->fld;
							ptrIn += offsetStart;
							memcpy(ptrOut, ptrIn, sizeof(jvxInt64) * numElements);
						}
						else
						{
							res = JVX_ERROR_INVALID_SETTING;
						}
						break;
					default:
						maxSpace = (selection->num - offsetStart);
						szElem = jvx_numberBytesField(1, format, 1);
						if(numElements <= maxSpace)
						{
							memcpy(fld, ((jvxByte*)selection->fld + (szElem * offsetStart)), szElem * numElements);
						}
						else
						{
							res = JVX_ERROR_SIZE_MISMATCH;
						}
					}
				}
				else
				{
					res = JVX_ERROR_INVALID_SETTING;
				}
			}
			else
			{
				res = JVX_ERROR_INVALID_SETTING;
			}
		}

		_unlock_properties();

		return res;
	};

	inline jvxErrorType _initialize_properties(IjvxObject* hObj, bool useLock)
	{
		_common_set_properties.useLock = useLock;
		return JVX_NO_ERROR;
	};

	inline jvxErrorType _setThreadId_properties(JVX_THREAD_ID theThreadId)
	{
		_common_set_properties.threadId = theThreadId;
		return JVX_NO_ERROR;
	};


	inline jvxErrorType _register_property(jvxHandle* fld,
					       jvxSize numberElements,
					       jvxDataFormat format,
					       jvxPropertyCategoryType cat,
					       jvxInt32* idProp,
					       jvxUInt64 allowStateMask,
					       jvxUInt64 allowThreadingMask,
						   const char* name,
					       const char* description,
					       const char* descriptor,
					       jvxBool* isValid,
					       jvxBool* isFrozen,
					       jvxPropertyAccessType accessType,
					       jvxPropertyDecoderHintType decTp,
					       jvxPropertyContext contxt)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;
		jvxSize propProposed = _common_set_properties.nextPropId;

		if(idProp)
		{
			if(*idProp >= 0)
			{
				propProposed = *idProp;
				if(cat == JVX_PROPERTY_CATEGORY_UNSPECIFIC)
				{
					_common_set_properties.nextPropId = JVX_MAX(_common_set_properties.nextPropId, propProposed) + 1;
				}
			}
			else
			{
				if(cat == JVX_PROPERTY_CATEGORY_UNSPECIFIC)
				{
					_common_set_properties.nextPropId++;
				}
				else
				{
					assert(0);
				}
			}
		}
		else
		{
			return JVX_ERROR_INVALID_ARGUMENT;
		}

		selection = findInPropertyList(_common_set_properties.registeredProperties, propProposed, cat);
		if(selection == _common_set_properties.registeredProperties.end())
		{
			oneProperty elm;
			elm.allow_state_mask = allowStateMask;
			elm.allow_threading_mask = allowThreadingMask;
			elm.cat = cat;
			elm.name = name;
			elm.description = description;
			elm.descriptor = descriptor;
			elm.fld = fld;
			elm.format = format;
			elm.id = propProposed;
			elm.num = numberElements;
			elm.accessType = accessType;
			elm.decTp = decTp;
			elm.isValid = isValid;
			elm.isFrozen = isFrozen;
			elm.context = contxt;
			elm.shadow.isValid = NULL;
			_common_set_properties.registeredProperties.push_back(elm);
			_common_set_properties.propSetRevsision++;

			*idProp = (int)propProposed;
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return res;
	}

	inline jvxErrorType _update_property(jvxHandle* fld, jvxSize numberElements, jvxPropertyCategoryType cat, jvxInt32 idProp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInPropertyList(_common_set_properties.registeredProperties, idProp, cat);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			_lock_properties();
			selection->fld = fld;
			selection->num = numberElements;
			_unlock_properties();
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		return res;
	};

	inline jvxErrorType _update_property_access_type(jvxPropertyAccessType accessType, jvxPropertyCategoryType cat, jvxInt32 idProp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInPropertyList(_common_set_properties.registeredProperties, idProp, cat);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			selection->shadow.accessType = selection->accessType;
			selection->accessType = accessType;
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		return res;
	};

	inline jvxErrorType _undo_update_property_access_type( jvxPropertyCategoryType cat, jvxInt32 idProp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInPropertyList(_common_set_properties.registeredProperties, idProp, cat);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			selection->accessType = selection->shadow.accessType;
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		return res;
	};
	inline jvxErrorType _update_property_decoder_type(jvxPropertyDecoderHintType decTp, jvxPropertyCategoryType cat, jvxInt32 idProp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInPropertyList(_common_set_properties.registeredProperties, idProp, cat);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			selection->decTp = decTp;
		}
		else
		{
			res = JVX_ERROR_ELEMENT_NOT_FOUND;
		}
		return res;
	};

	inline jvxErrorType _unregister_property(jvxPropertyCategoryType cat, jvxSize idProp)
	{
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxState stat = JVX_STATE_NONE;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		selection = findInPropertyList(_common_set_properties.registeredProperties, idProp, cat);
		if(selection != _common_set_properties.registeredProperties.end())
		{
			_common_set_properties.registeredProperties.erase(selection);
			_common_set_properties.propSetRevsision++;
		}
		else
		{
			res = JVX_ERROR_DUPLICATE_ENTRY;
		}
		return res;
	};

	/*
	inline jvxErrorType _trigger_properties_have_changed(IjvxObject* theObj, IjvxHiddenInterface* hostRef, jvxPropertyCategoryType cat, jvxInt16 propid, bool contentOnly, jvxInt16 callpurp = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC)
	{
		jvxErrorType res = JVX_NO_ERROR;
		IjvxReport* theReport = NULL;
		jvxComponentType tp = JVX_COMPONENT_UNKNOWN;

		if(hostRef && theObj)
		{
			hostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if(theReport)
			{
				theObj->request_specialization(NULL, &tp, NULL);
				theReport->report_internals_have_changed(tp, theObj, cat, propid, contentOnly, JVX_COMPONENT_UNKNOWN, callpurp);
				hostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
			else
			{
				res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
			}
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
		return res;
	};

	inline jvxErrorType _trigger_properties_have_changed_target(IjvxObject* theObj, IjvxHiddenInterface* hostRef, jvxPropertyCategoryType cat, jvxInt16 propid, bool contentOnly, jvxComponentType tpTo, jvxInt16 callpurp = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC)
	{
		jvxErrorType res = JVX_NO_ERROR;
		IjvxReport* theReport = NULL;
		jvxComponentType tpFrom = JVX_COMPONENT_UNKNOWN;

		if(hostRef && theObj)
		{
			hostRef->request_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle**>(&theReport));
			if(theReport)
			{
				theObj->request_specialization(NULL, &tpFrom, NULL);
				theReport->report_internals_have_changed(tpFrom, theObj, cat, propid, contentOnly, tpTo, callpurp);
				hostRef->return_hidden_interface(JVX_INTERFACE_REPORT, reinterpret_cast<jvxHandle*>(theReport));
			}
			else
			{
				res = JVX_ERROR_CALL_SUB_COMPONENT_FAILED;
			}
		}
		else
		{
			res = JVX_ERROR_INVALID_ARGUMENT;
		}
		return res;
	};
	*/

	virtual jvxErrorType JVX_CALLINGCONVENTION _request_takeover_property(jvxComponentType thisismytype, IjvxObject* thisisme,
		jvxHandle* fld, jvxSize numElements, jvxDataFormat format, jvxSize offsetStart, jvxBool onlyContent,
		jvxPropertyCategoryType category, jvxInt16 propId, jvxInt16 callpurp = JVX_PROPERTY_CALL_PURPOSE_NONE_SPECIFIC)
	{
		return JVX_ERROR_UNSUPPORTED;
	};

	virtual jvxErrorType JVX_CALLINGCONVENTION _freeze_properties(jvxSize* idProps, jvxPropertyCategoryType* category, jvxSize num)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		for(i = 0; i < num; i++)
		{
			selection = findInPropertyList(_common_set_properties.registeredProperties, idProps[i], category[i]);
			if(selection != _common_set_properties.registeredProperties.end())
			{
				*selection->isFrozen = true;
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}

		_unlock_properties();

		if(res != JVX_NO_ERROR)
		{
			_unfreeze_properties(idProps, category, num);
		}
		return res;
	};


	virtual jvxErrorType JVX_CALLINGCONVENTION _unfreeze_properties(jvxSize* idProps, jvxPropertyCategoryType* category, jvxSize num)
	{
		jvxSize i;
		jvxErrorType res = JVX_NO_ERROR;
		std::vector<oneProperty>::iterator selection;
		jvxSize maxSpace = 0;
		jvxSize szElem = 0;

		_lock_properties();

		for(i = 0; i < num; i++)
		{
			selection = findInPropertyList(_common_set_properties.registeredProperties, idProps[i], category[i]);
			if(selection != _common_set_properties.registeredProperties.end())
			{
				*selection->isFrozen = false;
			}
			else
			{
				res = JVX_ERROR_ELEMENT_NOT_FOUND;
			}
		}

		_unlock_properties();
		assert(res == JVX_NO_ERROR);
		return res;
	};

	// =========================================================================================
	inline std::vector<CjvxProperties::oneProperty>::iterator findInPropertyList(std::vector<CjvxProperties::oneProperty>& lst, jvxSize propId, jvxPropertyCategoryType cat)
	{
		std::vector<CjvxProperties::oneProperty>::iterator elm = lst.begin();
		while(elm != lst.end())
		{
			if(
				(elm->cat == cat) &&
				(elm->id == propId)
				)
			{
				break;
			}
			elm++;
		}
		return elm;
	};

		// =========================================================================================
	inline std::vector<CjvxProperties::oneProperty>::iterator findInPropertyList(std::vector<CjvxProperties::oneProperty>& lst, const char* descriptor)
	{
		std::vector<CjvxProperties::oneProperty>::iterator elm = lst.begin();
		while(elm != lst.end())
		{
			if(
				(elm->descriptor == descriptor))
			{
				break;
			}
			elm++;
		}
		return elm;
	};
/*
	inline jvxErrorType _deallocate(jvxStringList* str)
	{
		HjvxObject_deallocate(str);
		return JVX_NO_ERROR;
	};
	*/

};

#endif
