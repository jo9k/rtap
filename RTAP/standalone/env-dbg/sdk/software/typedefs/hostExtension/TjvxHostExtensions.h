/* 
 *****************************************************
 * Filename: TrtpHostExtensions.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description:                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: rtproc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __TJVXHOSTEXTENSIONS_H__
#define __TJVXHOSTEXTENSIONS_H__

typedef enum
{
		JVX_HOST_EXTENSION_FEATURE_NONE = 0,
		JVX_HOST_EXTENSION_FEATURE_HARDWARE_SETUP_CHANGED = 1,
		JVX_HOST_EXTENSION_FEATURE_SOFTWARE_SETUP_CHANGED = 2,
		JVX_HOST_EXTENSION_FEATURE_START_PROCESSING = 3,
		JVX_HOST_EXTENSION_FEATURE_STOP_PROCESSING = 4
} jvxHostExtensionFeatureType;

#endif