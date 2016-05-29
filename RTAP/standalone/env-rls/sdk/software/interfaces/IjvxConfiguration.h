#ifndef __IJVXCONFIGURATION_H__
#define __IJVXCONFIGURATION_H__

JVX_INTERFACE IjvxConfiguration
{
public:
	virtual JVX_CALLINGCONVENTION ~IjvxConfiguration(){};

	virtual jvxErrorType JVX_CALLINGCONVENTION put_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionToContainAllSubsectionsForMe, const char* filename = "", jvxInt32 lineno = -1 ) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION get_configuration(IjvxConfigProcessor* processor, jvxHandle* sectionWhereToAddAllSubsections) = 0;
};

#endif