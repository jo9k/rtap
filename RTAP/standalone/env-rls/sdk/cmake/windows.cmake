header_message("Windows specific configuration")

# platform specific configuration options
set(SED "sed" CACHE PATH "Path to stream editor (sed) TO BE REMOVED")
set(MATLAB_PATH_64 $ENV{MATLAB_PATH_64} CACHE PATH "Path to Matlab installation (64bit)")
set(MATLAB_PATH_32 $ENV{MATLAB_PATH_32} CACHE PATH "Path to Matlab installation (32bit)")
set(OCTAVE_PATH_64 $ENV{OCTAVE_PATH_64} CACHE PATH "Path to Octave installation (64bit)")
set(OCTAVE_PATH_32 $ENV{OCTAVE_PATH_32} CACHE PATH "Path to Octave installation (32bit)")
set(QT_PATH_64 $ENV{QT_PATH_64} CACHE PATH "Path to QT installation (64bit)")
set(QT_PATH_32 $ENV{QT_PATH_32} CACHE PATH "Path to QT installation (32bit)")

# deactivate unsupported stuff
if (JVX_USE_PART_ALSA)
  message("XX> deactivating ALSA support (not supported on Windows)")
  set(JVX_USE_PART_ALSA OFF)
endif()
if (JVX_USE_PART_COREAUDIO)
  message("XX> deactivating core audio support (not supported on Windows)")
  set(JVX_USE_PART_COREAUDIO OFF)
endif()

# Set the maximum path length to just less than the allowed 250 due to bug in cmake
set(CMAKE_OBJECT_PATH_MAX 240)

# pre-/suffixes
set(JVX_SHARED_EXTENSION dll)
set(JVX_STATIC_EXTENSION lib)
set(JVX_SCRIPT_EXTENSION ".bat")

# Global disable for shared libs (in case shared libs are not supported)
set(JVX_DISABLE_ALL_SHARED FALSE)

# Configure Qt
if(JVX_PLATFORM MATCHES "32bit")
  set(QT_PATH ${QT_PATH_32})
else()
  set(QT_PATH ${QT_PATH_64})
endif()
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
# FIXME: needed under win? set(JVX_QT_RPATH -Wl,-rpath-link,${QT_PATH}/lib)

if(JVX_DSP_BASE_USE_DATA_FORMAT_FLOAT)
  set(JVX_COMPILE_DISABLE_WARNING "/wd4244 /wd4305")
endif()

# Flags for shared libraries
set(JVX_CMAKE_C_FLAGS_SHARED "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_CXX_FLAGS_SHARED "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_LINKER_FLAGS_SHARED "")

# Flags for shared objects with export file list
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_COMPONENTS "/DEF:${JVX_BASE_ROOT}/software/exports/components/windows/exports.def")
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_LOCAL "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/exports/windows/exports.def")

# Flags for static libraries
set(JVX_CMAKE_C_FLAGS_STATIC "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_CXX_FLAGS_STATIC "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_C_FLAGS_STATIC_PIC "${JVX_CMAKE_C_FLAGS_STATIC}")
set(JVX_CMAKE_CXX_FLAGS_STATIC_PIC "${JVX_CMAKE_CXX_FLAGS_STATIC}")
set(JVX_CMAKE_LINKER_FLAGS_STATIC "")
set(JVX_CMAKE_LINKER_FLAGS_STATIC_PIC "")

# Flags for executables
set(JVX_CMAKE_C_FLAGS_EXEC "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_CXX_FLAGS_EXEC "/D_CRT_SECURE_NO_WARNINGS ${JVX_COMPILE_DISABLE_WARNING}")
set(JVX_CMAKE_LINKER_FLAGS_EXEC "")

set(JVX_SYSTEM_LIBRARIES "")
set(JVX_SOCKET_LIBRARIES "ws2_32;wsock32")

if(JVX_USE_QT)
	#install(DIRECTORY ${QT_PATH}/bin/ DESTINATION ${INSTALL_PATH_RUNTIME_LIBS} FILES_MATCHING PATTERN "*.dll")
	# We must be careful to copy only a subset of the QT dlls: It may be that we use another qt lib on the
	# development machine and then forget to copy the dll. In that case, program will run on development machine
	# but will fail at the customers machine
	install(FILES
		${QT_PATH}/bin/Qt5Cored.dll
		${QT_PATH}/bin/Qt5Guid.dll
		${QT_PATH}/bin/Qt5Widgetsd.dll
		${QT_PATH}/bin/Qt5PrintSupportd.dll
		${QT_PATH}/bin/libGLESv2d.dll
		${QT_PATH}/bin/libEGLd.dll
		DESTINATION ${INSTALL_PATH_LIB_SHARED}
		CONFIGURATIONS Debug UnstableDebug)

	install(FILES
		${QT_PATH}/bin/Qt5Core.dll
		${QT_PATH}/bin/Qt5Gui.dll
		${QT_PATH}/bin/Qt5Widgets.dll
		${QT_PATH}/bin/Qt5PrintSupport.dll
		${QT_PATH}/bin/libGLESv2.dll
		DESTINATION ${INSTALL_PATH_LIB_SHARED}
		CONFIGURATIONS Release UnstableRelease)

	install(FILES
		${QT_PATH}/plugins/platforms/qminimald.dll
		${QT_PATH}/plugins/platforms/qoffscreend.dll
		${QT_PATH}/plugins/platforms/qwindowsd.dll
		DESTINATION ${INSTALL_PATH_BIN}/platforms
		CONFIGURATIONS Debug UnstableDebug)

	install(FILES
		${QT_PATH}/plugins/platforms/qminimal.dll
		${QT_PATH}/plugins/platforms/qoffscreen.dll
		${QT_PATH}/plugins/platforms/qwindows.dll
		DESTINATION ${INSTALL_PATH_BIN}/platforms
		CONFIGURATIONS Release UnstableRelease)
endif()

###
# macros
###

# configure FFT library
macro (find_fft)
  set(FFTW_VERSION "3.3")
  set(FFTW_VERSION_LIB "3-3")
  set(FFTWF_VERSION_LIB "3f-3")
  if(JVX_PLATFORM MATCHES "32bit")
	set(FFTW_INCLUDE_PATH_OSGUESS_PREFIX third_party/fftw-${FFTW_VERSION}-win/Win32)
    set(FFTW_INCLUDE_PATH_OSGUESS ${JVX_BASE_LIBS_INCLUDE_PATH}/${FFTW_INCLUDE_PATH_OSGUESS_PREFIX})
    set(FFTW_LIB_PATH_OSGUESS ${JVX_BASE_LIBS_LIB_PATH}/${FFTW_INCLUDE_PATH_OSGUESS_PREFIX})
  else()
	set(FFTW_INCLUDE_PATH_OSGUESS_PREFIX third_party/fftw-${FFTW_VERSION}-win/x64)
    set(FFTW_INCLUDE_PATH_OSGUESS ${JVX_BASE_LIBS_INCLUDE_PATH}/${FFTW_INCLUDE_PATH_OSGUESS_PREFIX})
    set(FFTW_LIB_PATH_OSGUESS ${JVX_BASE_LIBS_LIB_PATH}/${FFTW_INCLUDE_PATH_OSGUESS_PREFIX})
  endif()
  set(FFTW_LIBNAME_OS libfftw${FFTW_VERSION_LIB}.lib)
  set(FFTWF_LIBNAME_OS libfftw${FFTWF_VERSION_LIB}.lib)
  message("--> Looking for fftw library")
  find_path (FFT_INCLUDEDIR fftw3.h PATHS "${FFTW_INCLUDE_PATH_OSGUESS}")
  if(FFT_INCLUDEDIR)
    if(JVX_DSP_BASE_USE_DATA_FORMAT_FLOAT)
      find_library (FFT_LIBRARIES libfftw${FFTWF_VERSION_LIB}.lib PATHS "${FFTW_LIB_PATH_OSGUESS}")
    else()
      find_library (FFT_LIBRARIES libfftw${FFTW_VERSION_LIB}.lib PATHS "${FFTW_LIB_PATH_OSGUESS}")
    endif()
    set(FFT_COMPILE_DEFINITIONS "")
    message("    include path: ${FFT_INCLUDEDIR}")
    message("    compile definitions: ${FFT_COMPILE_DEFINITIONS}")
    message("    lib: ${FFT_LIBRARIES}")
  else()
    message(FATAL_ERROR "could not find fftw library")
  endif()
endmacro (find_fft)

# Matlab path specifications
macro (find_matlab)
  if(JVX_PLATFORM MATCHES "32bit")
    set(MATLAB_PATH ${MATLAB_PATH_32})
  else()
    set(MATLAB_PATH ${MATLAB_PATH_64})
  endif()
  if(IS_DIRECTORY "${MATLAB_PATH}")
    message("--> Matlab: ${MATLAB_PATH}")
    if(JVX_PLATFORM MATCHES "32bit")
      set(MATLAB_LIB_SUBDIR "win32")
      set(MATLAB_MEX_SUFFIX ".mexw32")
      set(MATLAB_MEX_SUBDIR "win32")
    else()
      set(MATLAB_LIB_SUBDIR "win64")
      set(MATLAB_MEX_SUFFIX ".mexw64")
      set(MATLAB_MEX_SUBDIR "win64")
    endif()
    set(Matlab_INCLUDE_DIRS ${MATLAB_PATH}/extern/include)
    set(MATLAB_PATH_LIB ${MATLAB_PATH}/extern/lib/${MATLAB_MEX_SUBDIR}/microsoft)
    set(JVX_SYSTEM_MATLAB_MEX_LIBRARIES libmx libmat libmex)
    set(JVX_CMAKE_LINKER_FLAGS_MEX "/DEF:${JVX_BASE_ROOT}/software/exports/components/windows/exports-mex.def")
  else(IS_DIRECTORY "${MATLAB_PATH}")
    message(FATAL_ERROR "XX> could not find Matlab, option must be deactivated manually to procede.")
    set(JVX_USE_PART_MATLAB OFF)
  endif(IS_DIRECTORY "${MATLAB_PATH}")
endmacro (find_matlab)

# Octave path specifications
macro (find_octave)
  if(JVX_PLATFORM MATCHES "32bit")
    set(OCTAVE_PATH ${OCTAVE_PATH_32})
  else()
    set(OCTAVE_PATH ${OCTAVE_PATH_64})
  endif()
  exec_program("${OCTAVE_PATH}/bin/octave-config" ARGS "-v" OUTPUT_VARIABLE OCTAVE_VERSION)
  exec_program("${OCTAVE_PATH}/bin/octave-config" ARGS "-p  BINDIR" OUTPUT_VARIABLE OCTAVE_BIN_PATH)
  if(IS_DIRECTORY "${OCTAVE_BIN_PATH}")
    message("--> found octave-${OCTAVE_VERSION}")
    exec_program("${OCTAVE_PATH}/bin/octave-config" ARGS "-p OCTINCLUDEDIR" OUTPUT_VARIABLE OCTAVE_INCLUDE_PATH)
    exec_program("${OCTAVE_PATH}/bin/octave-config" ARGS "-p OCTLIBDIR" OUTPUT_VARIABLE OCTAVE_PATH_LIB)
    if(EXISTS ${OCTAVE_PATH}/bin/octave-cli.exe)
      set(OCTAVE ${OCTAVE_PATH}/bin/octave-cli.exe)
    else()
      set(OCTAVE ${OCTAVE_PATH}/bin/octave.exe)
    endif()
    set(OCTAVE_MEX_SUFFIX ".mex")
    set(JVX_SYSTEM_OCTAVE_MEX_LIBRARIES octinterp octave)
    set(JVX_CMAKE_LINKER_FLAGS_OCTAVE "/DEF:${JVX_BASE_ROOT}/software/exports/components/windows/exports-mex.def")

    # workaround for octave 4
    if(OCTAVE_VERSION VERSION_GREATER "3.9.9")
      message("--> Copying octave libs to build tree (workaround for Octave 4.0.0)")
      exec_program(${CMAKE_COMMAND} ARGS "-E copy ${OCTAVE_PATH_LIB}/liboctinterp.dll.a ${CMAKE_CURRENT_BINARY_DIR}/octinterp.lib")
      exec_program(${CMAKE_COMMAND} ARGS "-E copy ${OCTAVE_PATH_LIB}/liboctave.dll.a ${CMAKE_CURRENT_BINARY_DIR}/octave.lib")
      set(OCTAVE_PATH_LIB ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  else(IS_DIRECTORY "${OCTAVE_BIN_PATH}")
    message(FATAL_ERROR "XX> could not find Octave, option must be deactivated manually to procede.")
    set(JVX_USE_PART_OCTAVE OFF)
  endif(IS_DIRECTORY "${OCTAVE_BIN_PATH}")
endmacro (find_octave)

# Configure PCAP Library

macro(find_pcap)
  message("--> Looking for libpcap")
  set(PCAP_LIBRARY_PATH $ENV{PCAP_LIBRARY_PATH})
  if(JVX_PLATFORM MATCHES "32bit")
    set(PCAP_INCLUDEDIR_OSGUESS ${PCAP_LIBRARY_PATH}/Include)
    set(PCAP_LIB_PATH_OSGUESS ${PCAP_LIBRARY_PATH}/Lib)
  else()
    set(PCAP_INCLUDEDIR_OSGUESS ${PCAP_LIBRARY_PATH}/Include)
    set(PCAP_LIB_PATH_OSGUESS ${PCAP_LIBRARY_PATH}/Lib/x64)
  endif()
  set(PCAP_LIBNAME_OS "wpcap")
  set(PCAPPA_LIBNAME_OS "packet")
  find_path (PCAP_INCLUDEDIR pcap.h PATHS "${PCAP_INCLUDEDIR_OSGUESS}")
  
  if(PCAP_INCLUDEDIR)
    set(PCAP_FOUND TRUE)
    find_library (PCAP_LIBRARIES NAMES ${PCAP_LIBNAME_OS} PATHS "${PCAP_LIB_PATH_OSGUESS}")
    find_library (PCAPPA_LIBRARIES NAMES ${PCAPPA_LIBNAME_OS} PATHS "${PCAP_LIB_PATH_OSGUESS}")
    message("    include path: ${PCAP_INCLUDEDIR}")
    message("    lib: ${PCAP_LIBRARIES}")
    message("    lib: ${PCAPPA_LIBRARIES}")
    add_library(${PCAP_LIBNAME_OS} SHARED IMPORTED)
    add_library(${PCAPPA_LIBNAME_OS} SHARED IMPORTED)
  else()
    set(PCAP_FOUND FALSE)
    message(FATAL_ERROR "    lib pcap not available")
  endif()
endmacro (find_pcap)

# This macro lets you find executable programs on the host system
# -- identical to find_package for native windows builds
macro (find_host_package)
  find_package(${ARGN})
endmacro (find_host_package)
