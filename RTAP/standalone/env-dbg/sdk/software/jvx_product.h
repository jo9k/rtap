#ifndef __JVX_PRODUCT_H__
#define __JVX_PRODUCT_H__

#include "jvx_ns.h"

// Typedefs
#include "typedefs/controlWindow/TjvxControlWindow.h"
#include "typedefs/runtimeMessages/TjvxRuntimeMessages.h"
#include "typedefs/hostExtension/TjvxHostExtensions.h"
#include "typedefs/jvxTools/TjvxAudioFileIo.h"
#include "typedefs/jvxTools/TjvxGenericWrapperApi.h"

// Interfaces
#include "interfaces/IjvxAlgorithmCore.h"
#include "interfaces/IjvxAlgorithm.h"
#include "interfaces/IjvxAlgorithmExtensions.h"

#include "interfaces/IjvxExternalAudioChannels.h"

#include "interfaces/controlWindow/IjvxControlWindow.h"
#include "interfaces/controlWindow/IjvxWindowManagerBackward.h"
#include "interfaces/controlWindow/IjvxWindowManager.h"
 
#include "interfaces/hostExtension/IjvxHostExtensions.h"

#include "interfaces/runtimeMessages/IjvxRuntimeMessages.h"

#include "interfaces/logToFile/IjvxLogToFile.h"

#include "interfaces/jvxTools/IjvxRtAudioFileReader.h"
#include "interfaces/jvxTools/IjvxRtAudioFileWriter.h"

#include "interfaces/jvxTools/IjvxResampler.h"
#include "interfaces/jvxTools/IjvxDataConverter.h"

// Helper functions etc
#include "helpers/runtimeMessages/HjvxRuntimeMessages.h"
#include "helpers/jvxTools/HjvxAudioFileIo.h"
#include "helpers/jvxTools/HjvxGenericWrapperApi.h"

#endif