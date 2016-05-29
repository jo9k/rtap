#ifndef __JVX_H__
#define __JVX_H__

#define JVX_VERSION_TOKEN "0.0.1"

// Platform extensions for multi-platform
#include "jvx_platform.h"

// Namespace definition (only if desired)
#include "jvx_ns.h"

// =============================================================
// TYPEDEFS TYPEDEFS TYPEDEFS TYPEDEFS TYPEDEFS TYPEDEFS TYPEDEFS
// =============================================================

// System typedefs
#include "typedefs/TjvxTypes.h"
#ifdef __cplusplus
#include "typedefs/TjvxTypes_cpp.h"
#endif

// Some typedefs
#include "typedefs/TjvxConfigTokens.h"
#include "typedefs/TjvxProperties.h"
#include "typedefs/TjvxConfiglines.h"
#include "typedefs/TjvxBinaryFastDataControl.h"

// Some tool typedefs
#include "typedefs/tools/TjvxDataLogger.h"
#include "typedefs/tools/TjvxRs232.h"
#include "typedefs/tools/TjvxConfigProcessor.h"
#include "typedefs/tools/TjvxExternalCall.h"
#include "typedefs/tools/TjvxWebServer.h"

// Hidden interfaces
#include "typedefs/TjvxSequencer.h"
#include "typedefs/TjvxSequencerControl.h"
#include "typedefs/TjvxReport.h"

// Definition of function entry points for all components
#include "typedefs/TjvxAccess.h"

// Definition of Javox protocol for all remote data transfers
#include "typedefs/network/TjvxProtocol.h"

// Local delegate link types for hosts
#include "typedefs/hosts/TjvxMainWindowControl.h"
// =============================================================
// INTERFACES INTERFACES INTERFACES INTERFACES INTERFACES INTERFACES
// =============================================================

// Main System interfaces
#include "interfaces/IjvxDeallocate.h"
#include "interfaces/IjvxInformation.h"
#include "interfaces/IjvxObject.h"
#include "interfaces/IjvxHiddenInterface.h"
#include "interfaces/IjvxProperties.h"
#include "interfaces/IjvxBinaryFastDataControl.h"
#include "interfaces/IjvxCrypt.h"

// Tool Interfaces
#include "interfaces/tools/IjvxDataLogger.h"
#include "interfaces/tools/IjvxDataLogReader.h"
#include "interfaces/tools/IjvxRs232.h"
#include "interfaces/tools/IjvxConfigProcessor.h"
#include "interfaces/tools/IjvxToolsHost.h"
#include "interfaces/tools/IjvxExternalCallTarget.h"
#include "interfaces/tools/IjvxExternalCall.h"
#include "interfaces/tools/IjvxThreadController.h"
#include "interfaces/tools/IjvxWebServer.h"

// Hidden interfaces
#include "interfaces/IjvxSequencer.h"
#include "interfaces/IjvxSequencerControl.h"
#include "interfaces/IjvxConfigurationLine.h"
#include "interfaces/IjvxHostTypeHandler.h"
#include "interfaces/IjvxReport.h"
#include "interfaces/IjvxReportOnConfig.h"
#include "interfaces/IjvxConfiguration.h"

// Host interface
#include "interfaces/IjvxStateMachine.h"
#include "interfaces/IjvxHost.h"
#include "interfaces/IjvxDataProcessor.h"
#include "interfaces/IjvxDataProcessorSelector.h"

// Technology interface
#include "interfaces/IjvxNode.h"
#include "interfaces/IjvxDevice.h"
#include "interfaces/IjvxTechnology.h"

// Local delegate link interface (Backward references)
#include "interfaces/IjvxSubModuleInstall.h"

// Local delegate link interface for hosts
#include "interfaces/hosts/IjvxMainWindowControl.h"

// =============================================================
// MORE TYPEDEFS MORE TYPEDEFS MORE TYPEDEFS MORE TYPEDEFS MORE TYPEDEFS
// =============================================================
// Access to components and interfaces (must be here as it requires definition on IjvxObject)
#include "typedefs/TjvxAccess.h"

// =============================================================
// HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS HELPERS
// =============================================================

// Helper functionalities
#include "helpers/HjvxObject.h"
#include "helpers/HjvxMisc.h"
#include "helpers/HjvxHostsAndProperties.h"
#include "helpers/HjvxProperties.h"
#include "helpers/HjvxSequencer.h"
#include "helpers/HjvxComponentTypeSelectors.h"

// Helpers
#include "helpers/tools/HjvxRs232.h"

#include "jvx_product.h"

#ifdef QT_ALLOW_QT_WIDGETS
#include "interfaces/qt/IjvxQtWidgetHost.h"
#include "interfaces/qt/IjvxQtWidgetUi.h"
#endif


#endif
