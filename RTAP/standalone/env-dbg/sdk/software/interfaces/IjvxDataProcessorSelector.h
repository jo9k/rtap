#ifndef __IJVXDATAPROCESSORSELECTOR_H__
#define __IJVXDATAPROCESSORSELECTOR_H__

JVX_INTERFACE IjvxDataProcessorSelector
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxDataProcessorSelector(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION number_processors(jvxSize* num) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION type_processors(jvxSize idx, jvxComponentType* theType) = 0;

};

#endif