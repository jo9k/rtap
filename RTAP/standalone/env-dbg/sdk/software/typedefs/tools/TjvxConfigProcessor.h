#ifndef __TJVXCONFIGPROCESSOR_H__
#define __TJVXCONFIGPROCESSOR_H__

//! Unspecific type for config data sections
typedef void jvxConfigData;

//! Unspecific type for config list sections
typedef void jvxConfigList;

typedef enum
{
	JVX_CONFIG_SECTION_TYPE_UNKNOWN = 0,
	JVX_CONFIG_SECTION_TYPE_SECTION = 1,
	JVX_CONFIG_SECTION_TYPE_ASSIGNMENTSTRING = 2,
	JVX_CONFIG_SECTION_TYPE_ASSIGNMENTVALUE = 3,
	JVX_CONFIG_SECTION_TYPE_VALUELIST = 4,
	JVX_CONFIG_SECTION_TYPE_STRINGLIST = 5,
	JVX_CONFIG_SECTION_TYPE_REFERENCE = 6
} jvxConfigSectionTypes;

#endif