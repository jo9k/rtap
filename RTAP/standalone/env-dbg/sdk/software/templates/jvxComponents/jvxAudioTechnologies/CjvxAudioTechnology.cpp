#include "jvx.h"

#include "templates/jvxComponents/jvxAudioTechnologies/CjvxAudioTechnology.h"
#include "templates/jvxComponents/jvxAudioTechnologies/CjvxAudioDevice.h"

#ifdef JVX_PROJECT_NAMESPACE
namespace JVX_PROJECT_NAMESPACE {
#endif

CjvxAudioTechnology::CjvxAudioTechnology(const char* description, bool multipleObjects, const char* module_name, const char* descriptor, jvxBitField featureClass): 
	CjvxTechnology(description, multipleObjects, module_name, descriptor, featureClass)
{
	_common_set.theComponentType = JVX_COMPONENT_AUDIO_TECHNOLOGY;
	_common_set.theObjectSpecialization = reinterpret_cast<jvxHandle*>(static_cast<IjvxTechnology*>(this));
	_common_set.thisisme = static_cast<IjvxObject*>(this);

}

CjvxAudioTechnology::~CjvxAudioTechnology()
{
	terminate();
}

jvxErrorType
CjvxAudioTechnology::activate()
{
	jvxErrorType res = _activate();
	if(res == JVX_NO_ERROR)
	{
#ifdef USE_STANDALONE_SKELETON
		// Do whatever is required
		CjvxAudioDevice* newDevice = new CjvxAudioDevice("Single Device", false, _common_set.theModuleName.c_str(), _common_set.theDescriptor.c_str(), _common_set.theFeatureClass);

		// Whatever to be done for initialization
		oneDeviceWrapper elm;
		elm.hdlDev= static_cast<IjvxDevice*>(newDevice);

		_common_tech_set.lstDevices.push_back(elm);
#endif
	}
	return(res);
}

jvxErrorType
CjvxAudioTechnology::deactivate()
{
	jvxSize i;
	jvxErrorType res = _deactivate();
	if(res == JVX_NO_ERROR)
	{
#ifdef USE_STANDALONE_SKELETON
		for(i = 0; i < _common_tech_set.lstDevices.size(); i++)
		{
			delete(_common_tech_set.lstDevices[i].hdlDev);
		}
		_common_tech_set.lstDevices.clear();
#endif
	}
	return(res);
}

#ifdef JVX_PROJECT_NAMESPACE
}
#endif