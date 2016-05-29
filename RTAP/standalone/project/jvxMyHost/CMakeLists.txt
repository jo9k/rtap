if(DEFINED JVX_SDK_PATH)
	cmake_minimum_required (VERSION 2.8.11)
	include(${JVX_SDK_PATH}/cmake/standalone.cmake)
endif(DEFINED JVX_SDK_PATH)

# Some common include files
set(PROJECT_NAME jvxMyHost)
include(${JVX_CMAKE_DIR}/common_local_head.cmake)

# Decide which host library to link with
set(JVX_LINK_WITH_AUDIO_HOST TRUE)

# Add mainWidget and other ui files
set(LOCAL_UI_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/uisrc/myMainWidget.ui
	)

# Qt resource file add
set(LOCAL_QT_RESOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/uisrc/jvxMyNewHost.qrc
	)

# Source code files
set(LOCAL_SOURCES
    ${JVX_BASE_ROOT}/software/templates/jvxHosts/qt-host-ui-extensions/mainCentral.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/jvxAudioHost_hooks.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/myCentralWidget.cpp
	)

# Some required include paths
include_directories(
  ${JVX_BASE_ROOT}/software/templates/jvxHosts/qt-host-ui-extensions/
  ${JVX_SUBPRODUCT_BINARY_DIR}/sources/jvxComponents/jvxAudioNodes/jvxAuNMyAlgorithm/
 )

# Add algorithm specific library
set(LOCAL_LIBS
  jvxAuNMyAlgorithm_static
)

# Link against either audio host lib or develop host lib
if(JVX_LINK_WITH_AUDIO_HOST)
	
	# Link with audio host
	set(LOCAL_LIBS ${LOCAL_LIBS}
		jvxLAudioHost__algostatic_static
		)
		
	# In case of standalone builds, we need to pull in some other libraries
	if(DEFINED JVX_SDK_PATH)
		include(${JVX_CMAKE_DIR}/cmake-sdk/cmake-lib-packages.audiohost.cmake)
	endif()
else()

	# Link with develop host
	set(LOCAL_LIBS ${LOCAL_LIBS}
		jvxLDevelopHost_static
		)
	# In case of standalone builds, we need to pull in some other libraries
	if(DEFINED JVX_SDK_PATH)
		include(${JVX_CMAKE_DIR}/cmake-sdk/cmake-lib-packages.develophost.cmake)
	endif()
				
endif()

# Some compile defines
#set(LOCAL_COMPILE_DEFINITIONS "JVX_ADD_HOST_WEBSERVER;JVX_DONT_DERIVE_FROM_UI_DESIGNER;JVX_PROJECT_NAMESPACE=JVX_MY_HOST;JVX_NO_SEQUENCER;JVX_NO_RTVIEWER;JVX_NO_MESSAGES")
set(LOCAL_COMPILE_DEFINITIONS "JVX_DONT_DERIVE_FROM_UI_DESIGNER;JVX_PROJECT_NAMESPACE=JVX_MY_HOST")

# Macos x specific configurations
if(JVX_OS MATCHES "macosx")
  set(JVX_MAC_OS_CREATE_BUNDLE TRUE)
else(JVX_OS MATCHES "macosx")
  set(CONFIGURE_LOCAL_START_SCRIPT TRUE)
endif(JVX_OS MATCHES "macosx")

# Some common include files
include(${JVX_CMAKE_DIR}/common_local_exe_foot.cmake)
