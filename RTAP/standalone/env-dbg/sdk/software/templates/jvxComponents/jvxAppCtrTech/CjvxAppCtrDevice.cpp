
#include "templates/jvxComponents/jvxAppCtrTech/CjvxAppCtrDevice.h"

CjvxApplicationControllerDevice::CjvxApplicationControllerDevice(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
	CjvxObject(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_APPLICATION_CONTROLLER_DEVICE;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxDevice*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);


}

CjvxApplicationControllerDevice::~CjvxApplicationControllerDevice()
{
	this->terminate();
}

jvxErrorType
CjvxApplicationControllerDevice::activate()
{
	jvxErrorType res = JVX_NO_ERROR;
	res = _activate();
	if(res == JVX_NO_ERROR)
	{
		init__properties_active();
		allocate__properties_active();

		/*
		associate__properties_active(static_cast<CjvxProperties*>(this),
			&_common_set_audio_device.inputchannels, &_common_set_audio_device.outputchannels);
		*/
		register__properties_active(static_cast<CjvxProperties*>(this));
	}
	return(res);
}

jvxErrorType
CjvxApplicationControllerDevice::deactivate()
{
	jvxErrorType res = JVX_NO_ERROR;
	res = _deactivate();
	if(res == JVX_NO_ERROR)
	{
		unregister__properties_active(static_cast<CjvxProperties*>(this));
		//		deassociate__properties_active(static_cast<CjvxProperties*>(this));

		deallocate__properties_active();


	}
	return(res);
}
