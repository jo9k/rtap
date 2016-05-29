header_message("Mac OS X specific configuration")

# platform specific configuration options
set(SED "/usr/bin/sed" CACHE PATH "Path to stream editor (sed) TO BE REMOVED")

# deactivate unsupported stuff
if (JVX_USE_PART_ASIO)
  message("XX> deactivating ASIO support (not supported on Mac OS X)")
  set(JVX_USE_PART_ASIO OFF)
endif()
if (JVX_USE_PART_RS232)
  message("XX> deactivating RS232 support (not supported on Mac OS X)")
  set(JVX_USE_PART_RS232 OFF)
endif()
if (JVX_USE_PART_CORECONTROL)
  message("XX> deactivating corecontrol support (not supported on Mac OS X)")
  set(JVX_USE_PART_CORECONTROL OFF)
endif()
if (JVX_USE_PART_ALSA)
  message("XX> deactivating alsa audio support (not supported on Mac OS X)")
  set(JVX_USE_PART_ALSA OFF)
endif()

# Find the required core audio libraries
find_library(COREAUDIO CoreAudio)
if (NOT COREAUDIO)
    message(FATAL_ERROR "CoreAudio not found")
endif(NOT COREAUDIO)

find_library(COREAUDIOKIT CoreAudioKit)
if (NOT COREAUDIOKIT)
    message(FATAL_ERROR "CoreAudioKit not found")
endif(NOT COREAUDIOKIT)

find_library(COREFOUNDATION coreFoundation)
if (NOT COREFOUNDATION)
    message(FATAL_ERROR "CoreFoundation not found")
endif(NOT COREFOUNDATION)

find_library(FOUNDATION Foundation)
if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
endif(NOT FOUNDATION)

find_library(AVFOUNDATION AVFoundation)
if (NOT AVFOUNDATION)
    message(FATAL_ERROR "AVFoundation not found")
endif(NOT AVFOUNDATION)

find_library(CORESERVICES CoreServices)
if (NOT CORESERVICES)
    message(FATAL_ERROR "CoreServices not found")
endif(NOT CORESERVICES)

find_library(AUDIOUNIT AudioUnit)
if (NOT AUDIOUNIT)
    message(FATAL_ERROR "AudioUnit not found")
endif(NOT AUDIOUNIT)

find_library(AUDIOTOOLBOX AudioToolbox)
if (NOT AUDIOTOOLBOX)
    message(FATAL_ERROR "AudioToolbox not found")
endif(NOT AUDIOTOOLBOX)

find_library(ACCELERATE Accelerate)
if (NOT ACCELERATE)
    message(FATAL_ERROR "Accelerate not found")
endif(NOT ACCELERATE)

# Do not use RPATHs if you want to create deployable bundles
set(CMAKE_MACOSX_RPATH 0)

# pre-/suffixes
set(JVX_SHARED_EXTENSION dylib)
set(JVX_STATIC_EXTENSION a)
set(JVX_SCRIPT_EXTENSION "")

# Global disable for shared libs (in case shared libs are not supported)
set(JVX_DISABLE_ALL_SHARED FALSE)

# Flags for shared libraries
set(JVX_CMAKE_C_FLAGS_SHARED "--std=gnu99 -fPIC -pthread -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_CXX_FLAGS_SHARED "--std=c++0x -fPIC -pthread -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_LINKER_FLAGS_SHARED "-Wl,-undefined,error")

# Flags for shared objects with export file list
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_COMPONENTS "${JVX_CMAKE_LINKER_FLAGS_SHARED} -Wl,-exported_symbols_list,${JVX_BASE_ROOT}/software/exports/components/macosx/exports.def")
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_LOCAL "${JVX_CMAKE_LINKER_FLAGS_SHARED} -Wl,-exported_symbols_list,${JVX_BASE_ROOT}/software/exports/components/macosx/exports.def")

# Flags for static libraries
set(JVX_CMAKE_C_FLAGS_STATIC "--std=gnu99 -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_CXX_FLAGS_STATIC "--std=c++0x -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_C_FLAGS_STATIC_PIC "${JVX_CMAKE_C_FLAGS_STATIC} -fPIC")
set(JVX_CMAKE_CXX_FLAGS_STATIC_PIC "${JVX_CMAKE_CXX_FLAGS_STATIC} -fPIC")
set(JVX_CMAKE_LINKER_FLAGS_STATIC "")
set(JVX_CMAKE_LINKER_FLAGS_STATIC_PIC "")

# Flags for executables
set(JVX_CMAKE_C_FLAGS_EXEC "--std=gnu99 -fPIC -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_CXX_FLAGS_EXEC "--std=c++0x -Wno-switch -Wno-deprecated-register -Wno-comment")
set(JVX_CMAKE_LINKER_FLAGS_EXEC "")

set(JVX_SYSTEM_LIBRARIES dl pthread)

###
# macros
###

# configure FFT library
macro (find_fft)
  message("--> Configuring FFT library")
  set(FFT_INCLUDEDIR "")
  set(FFT_COMPILE_DEFINITIONS "JVX_FFT_APPLE")
  set(FFT_LIBRARIES ${ACCELERATE})
  message("    include path: ${FFT_INCLUDEDIR}")
  message("    compile definitions: ${FFT_COMPILE_DEFINITIONS}")
  message("    lib: ${FFT_LIBRARIES}")
endmacro (find_fft)

# Matlab path specifications
macro (find_matlab)
  set(MATLAB_PATH_64 $ENV{MATLAB_PATH_64} CACHE PATH "Path to Matlab installation (64bit)")
  set(MATLAB_PATH ${MATLAB_PATH_64})
  if(IS_DIRECTORY "${MATLAB_PATH}")
    message("--> Matlab: ${MATLAB_PATH}")
    set(MATLAB_LIB_SUBDIR "maci64")
    set(MATLAB_MEX_SUFFIX ".mexmaci64")
    set(MATLAB_MEX_SUBDIR "maci64")
    set(Matlab_INCLUDE_DIRS ${MATLAB_PATH}/extern/include)
    set(MATLAB_PATH_LIB ${MATLAB_PATH}/bin/${MATLAB_MEX_SUBDIR})
    set(JVX_SYSTEM_MATLAB_MEX_LIBRARIES libmx.dylib libmat.dylib libmex.dylib)
  else(IS_DIRECTORY "${MATLAB_PATH}")
    message("XX> could not find Matlab, deactivating support")
    set(JVX_USE_PART_MATLAB OFF)
  endif(IS_DIRECTORY "${MATLAB_PATH}")
endmacro (find_matlab)

# Octave path specifications
macro (find_octave)
  exec_program("octave-config" ARGS "-v" OUTPUT_VARIABLE OCTAVE_VERSION)
  exec_program("octave-config" ARGS "-p  BINDIR" OUTPUT_VARIABLE OCTAVE_PATH)
  if(IS_DIRECTORY "${OCTAVE_PATH}")
    message("--> found octave-${OCTAVE_VERSION}")
    exec_program("octave-config" ARGS "-p OCTINCLUDEDIR" OUTPUT_VARIABLE OCTAVE_INCLUDE_PATH)
    exec_program("octave-config" ARGS "-p OCTLIBDIR" OUTPUT_VARIABLE OCTAVE_PATH_LIB)
    set(OCTAVE ${OCTAVE_PATH}/octave)
    set(OCTAVE_MEX_SUFFIX ".mex")
    set(JVX_SYSTEM_OCTAVE_MEX_LIBRARIES octinterp octave)
    set(JVX_CMAKE_LINKER_FLAGS_OCTAVE "-Wl,-undefined,error -Wl,-exported_symbols_list,${JVX_BASE_ROOT}/software/exports/components/macosx/exports-mex.def")
  else(IS_DIRECTORY "${OCTAVE_PATH}")
    message("XX> could not find Octave, deactivating support")
    set(JVX_USE_PART_OCTAVE OFF)
  endif(IS_DIRECTORY "${OCTAVE_PATH}")
endmacro (find_octave)

# This macro lets you find executable programs on the host system
# -- identical to find_package for native macosx builds
macro (find_host_package)
  find_package(${ARGN})
endmacro (find_host_package)

# Collect all components for a possible app bundle Mac OS
set(JVX_COLLECT_COMPONENTS_MAC_BUNDLE "" CACHE INTERNAL "all components")

