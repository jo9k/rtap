#ifndef __CJVXDEALLOCATE_H__
#define __CJVXDEALLOCATE_H__

#include "jvx.h"

class CjvxDeallocate
{
public:

	CjvxDeallocate(){};

	inline jvxErrorType _deallocate(jvxString* str)
	{
		HjvxObject_deallocate(str);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _deallocate(jvxError* err)
	{

		HjvxObject_deallocate(err);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _deallocate(jvxStringList* strL)
	{
		HjvxObject_deallocate(strL);
		return(JVX_NO_ERROR);
	};

	inline jvxErrorType _deallocate(jvxDataList* dblL)
	{
		HjvxObject_deallocate(dblL);
		return(JVX_NO_ERROR);
	};

};

#endif
