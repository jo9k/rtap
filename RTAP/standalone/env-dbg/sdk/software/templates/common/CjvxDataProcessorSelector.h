#ifndef __CJVXDATAPROCESSORSELECTOR_H__
#define __CJVXDATAPROCESSORSELECTOR_H__

#include "jvx.h"


class CjvxDataProcessorSelector
{
protected:
	typedef struct
	{
		jvxComponentType theType;
		//jvxInt32 thePurpose;
	} dataProcessorAndPurpose;

	struct
	{
		std::vector<dataProcessorAndPurpose> entries;
	} _common_set_dataprocessor_selector;

public:

	CjvxDataProcessorSelector()
	{
	};

	~CjvxDataProcessorSelector()
	{
	};

	jvxErrorType _number_processors(jvxSize* num)
	{
		if(num)
		{
			*num = _common_set_dataprocessor_selector.entries.size();
		}
		return(JVX_NO_ERROR);
	};

	jvxErrorType _type_processors(jvxSize idx, jvxComponentType* theType/*, jvxInt32* purpose*/)
	{
		if(idx < _common_set_dataprocessor_selector.entries.size())
		{
			if(theType)
			{
				*theType = _common_set_dataprocessor_selector.entries[idx].theType;
			}
			/*
			if(purpose)
			{
				*purpose = _common_set_dataprocessor_selector.entries[idx].thePurpose;
			}*/
			return(JVX_NO_ERROR);
		}
		return(JVX_ERROR_ID_OUT_OF_BOUNDS);
	};
};

#endif