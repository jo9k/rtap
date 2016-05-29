cmake_minimum_required (VERSION 2.8.11)
if(DEFINED JVX_SDK_PATH)
	include(${JVX_SDK_PATH}/cmake/standalone.cmake)
endif(DEFINED JVX_SDK_PATH)

set(PROJECT_NAME jvxAuNMyAlgorithm)

include(${JVX_CMAKE_DIR}/common_local_lib_head.cmake)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/target
  )

set(LOCAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/target/componentEntry.cpp
  ${JVX_BASE_ROOT}/software/templates/jvxComponents/jvxAudioNodes/CjvxAudioNode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CjvxAuNMyAlgorithm.cpp)


set(LOCAL_PCG_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/codeGen/exports_node.pcg
  ${JVX_BASE_ROOT}/software/templates/jvxComponents/jvxAudioNodes/CjvxAudioNode_pcg.pcg
  )

if(JVX_MATLAB_PROPERTY_GENERATOR) 
	jvx_genMatProperties(${JVX_TARGET_NAME} "JVX_COMPONENT_AUDIO_NODE" "node" "${LOCAL_PCG_FILES}")
endif()

set(IS_JVX_COMPONENT TRUE)
set(JVX_INSTALL_MATLAB_MINIMAL_FRAMEWORK TRUE)

include(${JVX_CMAKE_DIR}/common_local_lib_foot.cmake)


