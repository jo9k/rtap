/* 
 *****************************************************
 * Filename: IjvxAlgorithmExtensions.h
 *****************************************************
 * Project: JVXroc-ESP (Echtzeit-Software-Plattform) *
 *****************************************************
 * Description:                                      *
 *****************************************************
 * Developed by JAVOX SOLUTIONS GMBH, 2012           *
 *****************************************************
 * COPYRIGHT BY JAVOX SOLUTION GMBH                  *
 *****************************************************
 * Contact: jvxroc@javox-solutions.com               *
 *****************************************************
*/

#ifndef __IJVXALGORITHMEXTENSIONS_H__
#define __IJVXALGORITHMEXTENSIONS_H__


JVX_INTERFACE IjvxAlgorithmExtensions
{
public:

  virtual JVX_CALLINGCONVENTION ~IjvxAlgorithmExtensions(){};

	typedef enum
	{
		JVX_ALGORITHM_EXTENSION_NONE = 0,
		JVX_ALGORITHM_EXTENSION_PREPARE_PROCESSING = 1
	} algorithmExtensionFeatures;

	//virtual jvxBool JVX_CALLINGCONVENTION specifyExactSamplerate(jvxData exactSamplerate) = 0;
	virtual jvxErrorType JVX_CALLINGCONVENTION prepareProcessingAlgorithm_ext(jvxInt32 sampleRate, 
		jvxBool* successSamplerate, jvxData samleRate_exact, jvxBool* successExactSamplerate, jvxInt32 buffersize, jvxBool* successBuffersize,
		jvxInt32 channelsInput, jvxBool* successChansInput, jvxInt32 channelsOutput, jvxBool* successChansOutput,
		jvxDataFormat format, jvxBool* successFormat, jvxInt32 selectionID = -1, jvxBool* successSelectionID = NULL) = 0;

	virtual jvxErrorType JVX_CALLINGCONVENTION supportsFeature(jvxBool* retVal, algorithmExtensionFeatures idFeature) = 0;
};

#endif
