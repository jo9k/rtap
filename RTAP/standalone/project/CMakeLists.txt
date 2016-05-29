cmake_minimum_required (VERSION 2.8.11)

# If working with automatic relaive to absolute path generation,
# replace 
file(TO_CMAKE_PATH ${JVX_SDK_RUNTIME_PATH} JVX_SDK_RUNTIME_PATH)
file(TO_CMAKE_PATH ${JVX_SDK_PATH} JVX_SDK_PATH)

# Either build algorithm or host
if(JVX_COMPILE_ALGORITHM)
	add_subdirectory(jvxAuNMyAlgorithm)
endif()

if(JVX_COMPILE_HOST)
	add_subdirectory(jvxMyHost)
endif()
