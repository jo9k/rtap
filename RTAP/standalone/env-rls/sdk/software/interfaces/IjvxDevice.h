#ifndef __IJVXDEVICE_H__
#define __IJVXDEVICE_H__

JVX_INTERFACE IjvxDevice: public IjvxObject, public IjvxHiddenInterface, public IjvxStateMachine
{
	
public:

	virtual JVX_CALLINGCONVENTION ~IjvxDevice(){};
	

};

#endif