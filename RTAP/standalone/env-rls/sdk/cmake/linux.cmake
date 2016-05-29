header_message("Linux specific configuration")

# platform specific configuration options
set(SED "/bin/sed" CACHE PATH "Path to stream editor (sed) TO BE REMOVED")

# deactivate unsupported stuff
if (JVX_USE_PART_ASIO)
  message("XX> deactivating ASIO support (not supported on Linux)")
  set(JVX_USE_PART_ASIO OFF)
endif()
if (JVX_USE_PART_RS232)
  message("XX> deactivating RS232 support (not supported on Linux)")
  set(JVX_USE_PART_RS232 OFF)
endif()
if (JVX_USE_PART_CORECONTROL)
  message("XX> deactivating corecontrol support (not supported on Linux)")
  set(JVX_USE_PART_CORECONTROL OFF)
endif()
if (JVX_USE_PART_COREAUDIO)
  message("XX> deactivating core audio support (not supported on Linux)")
  set(JVX_USE_PART_COREAUDIO OFF)
endif()

# pre-/suffixes
set(JVX_SHARED_EXTENSION so)
set(JVX_STATIC_EXTENSION a)
set(JVX_SCRIPT_EXTENSION "")

# Gloab disable for shared libs (in case shared libs are not supported)
set(JVX_DISABLE_ALL_SHARED FALSE)

# use pkg-config for libraries that do not bring a cmake-package definition
find_package(PkgConfig)

# Flags for shared libraries
set(JVX_CMAKE_C_FLAGS_SHARED "--std=gnu99 -fPIC -pthread")
set(JVX_CMAKE_CXX_FLAGS_SHARED "--std=c++0x -fPIC -pthread")
set(JVX_CMAKE_LINKER_FLAGS_SHARED "-Wl,--no-undefined -shared")

# Flags for shared objects with export file list
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_COMPONENTS "${JVX_CMAKE_LINKER_FLAGS_SHARED} -Wl,--retain-symbols-file=${JVX_BASE_ROOT}/software/exports/components/linux/exports.def")
set(JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_LOCAL "${JVX_CMAKE_LINKER_FLAGS_SHARED} -Wl,--retain-symbols-file=${JVX_BASE_ROOT}/software/exports/components/linux/exports.def")

# Flags for static libraries
set(JVX_CMAKE_C_FLAGS_STATIC "--std=gnu99 -pthread")
set(JVX_CMAKE_CXX_FLAGS_STATIC "--std=c++0x -pthread")
set(JVX_CMAKE_C_FLAGS_STATIC_PIC "${JVX_CMAKE_C_FLAGS_STATIC} -fPIC")
set(JVX_CMAKE_CXX_FLAGS_STATIC_PIC "${JVX_CMAKE_CXX_FLAGS_STATIC} -fPIC")
set(JVX_CMAKE_LINKER_FLAGS_STATIC "")
set(JVX_CMAKE_LINKER_FLAGS_STATIC_PIC "")

# Flags for executables
set(JVX_CMAKE_C_FLAGS_EXEC "--std=gnu99 -fPIC -pthread")
set(JVX_CMAKE_CXX_FLAGS_EXEC "--std=c++0x -pthread")
set(JVX_CMAKE_LINKER_FLAGS_EXEC "")

set(JVX_SYSTEM_LIBRARIES dl pthread)

###
# macros
###

# configure FFT library
macro (find_fft)
  message("--> Looking for fftw library")

    if(JVX_CROSS_COMPILE)
        message("--> ${CMAKE_SYSROOT}")
		    include(FindPkgConfig)
		    set(PKG_CONFIG_PATH_OLD "$ENV{PKG_CONFIG_PATH}")
		    #set(ENV{PKG_CONFIG_SYSROOT_DIR} "${JVX_LINUX_CROSSCOMPILER_LIB_ROOT}")
		    set(ENV{PKG_CONFIG_PATH} "${JVX_PACKAGE_ROOT_PATH}/pkgconfig")
        message("--> ${JVX_PACKAGE_ROOT_PATH}/pkgconfig")
    endif()

  if(JVX_DSP_BASE_USE_DATA_FORMAT_FLOAT)
    pkg_check_modules(FFT fftw3f REQUIRED)
  else()
    pkg_check_modules(FFT fftw3 REQUIRED)
  endif()

    if(JVX_CROSS_COMPILE)
        set(ENV{PKG_CONFIG_PATH} "${PKG_CONFIG_PATH_OLD}")
        set(FFT_LIBDIR "${JVX_PACKAGE_ROOT_PATH}/${FFT_LIBDIR}")
        set(FFT_INCLUDEDIR "${JVX_PACKAGE_ROOT_PATH}/${FFT_INCLUDEDIR}")
    endif()

  set(FFT_COMPILE_DEFINITIONS "")
  message("     include path: ${FFT_INCLUDEDIR}")
  message("     lib path: ${FFT_LIBDIR}")
  message("     compile definitions: ${FFT_COMPILE_DEFINITIONS}")
  message("     lib: ${FFT_LIBRARIES}")
endmacro (find_fft)

# Matlab path specifications
macro (find_matlab)
  if(NOT DEFINED MATLAB_PATH)
    exec_program("matlab -n |grep  'MATLAB .*= .*'|cut -f 2 -d '='|cut -f 2 -d ' '" OUTPUT_VARIABLE MATLAB_PATH)
  endif()
  if(IS_DIRECTORY  "${MATLAB_PATH}")
    message("--> found matlab at ${MATLAB_PATH}")
    if(JVX_PLATFORM MATCHES "32bit")
      set(MATLAB_LIB_SUBDIR "glnx86")
      set(MATLAB_MEX_SUFFIX ".mexglx")
    else()
      set(MATLAB_LIB_SUBDIR "glnxa64")
      set(MATLAB_MEX_SUFFIX ".mexa64")
    endif()
    set(Matlab_INCLUDE_DIRS ${MATLAB_PATH}/extern/include)
    set(MATLAB_PATH_LIB ${MATLAB_PATH}/bin/${MATLAB_LIB_SUBDIR})
    set(JVX_SYSTEM_MATLAB_MEX_LIBRARIES mx mat mex)
    set(JVX_CXX_FLAGS_MEX_MATLAB "-D__STDC_UTF_16__ -pthread")
    set(JVX_CMAKE_LINKER_FLAGS_MEX "-Wl,--no-undefined -Wl,--retain-symbols-file=${JVX_BASE_ROOT}/software/exports/components/linux/exports-mex.def -Wl,-rpath-link,${MATLAB_LIB_PATH}")
  else(IS_DIRECTORY  "${MATLAB_PATH}")
    message("XX> could not find Matlab, deactivating support")
    set(JVX_USE_PART_MATLAB OFF)
  endif(IS_DIRECTORY  "${MATLAB_PATH}")
endmacro (find_matlab)

# Octave path specifications
macro (find_octave)
  exec_program("octave-config" ARGS "-v" OUTPUT_VARIABLE OCTAVE_VERSION)
  exec_program("octave-config" ARGS "-p  BINDIR" OUTPUT_VARIABLE OCTAVE_PATH)
  if(IS_DIRECTORY "${OCTAVE_PATH}")
    message("--> found octave-${OCTAVE_VERSION}")
    exec_program("octave-config" ARGS "-p OCTINCLUDEDIR" OUTPUT_VARIABLE OCTAVE_INCLUDE_PATH)
    exec_program("octave-config" ARGS "-p OCTLIBDIR" OUTPUT_VARIABLE OCTAVE_PATH_LIB)
    if(EXISTS ${OCTAVE_PATH}/octave-cli)
      set(OCTAVE ${OCTAVE_PATH}/octave-cli)
    else()
      set(OCTAVE ${OCTAVE_PATH}/octave)
    endif()
    set(OCTAVE_MEX_SUFFIX ".mex")
    #set(JVX_CXX_FLAGS_MEX_OCTAVE "-D__STDC_UTF_16__ -pthread")
    set(JVX_SYSTEM_OCTAVE_MEX_LIBRARIES octinterp octave)
    set(JVX_CMAKE_LINKER_FLAGS_OCTAVE "-Wl,--no-undefined -Wl,--retain-symbols-file=${JVX_BASE_ROOT}/software/exports/components/linux/exports-mex.def -Wl,-rpath-link,${OCTAVE_LIB_PATH}")
  else(IS_DIRECTORY "${OCTAVE_PATH}")
    message("XX> could not find Octave, deactivating support")
    set(JVX_USE_PART_OCTAVE OFF)
  endif(IS_DIRECTORY "${OCTAVE_PATH}")
endmacro (find_octave)

macro (find_pcap)
  message("--> Looking for libpcap")
  if(JVX_CROSS_COMPILE)
  else()
    exec_program("pcap-config --cflags | cut -c 3-" OUTPUT_VARIABLE PCAP_INCLUDEDIR)
    exec_program("pcap-config --libs | cut -f 1 -d ' ' | cut -c 3-" OUTPUT_VARIABLE PCAP_LIBDIR)
    exec_program("pcap-config --libs | cut -f 3 -d ' ' | cut -c 3-" OUTPUT_VARIABLE PCAP_LIBRARIES)
    #set(PCAP_LIBRARIES "${PCAP_LIBDIR}/${PCAP_LIBRARIES}")
  endif()

  message("-XX-> ${PCAP_INCLUDEDIR}")
  if(IS_DIRECTORY "${PCAP_INCLUDEDIR}")
    message("     include path: ${PCAP_INCLUDEDIR}")
    message("     library path: ${PCAP_LIBDIR}")
    message("     lib: ${PCAP_LIBRARIES}")
  else()
    message("     libpcap not available")
  endif()
endmacro (find_pcap)



# This macro lets you find executable programs on the host system
# -- identical to find_package for native linux builds
macro (find_host_package)
  if(JVX_CROSS_COMPILE)
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
  endif()
  find_package(${ARGN})
  if(JVX_CROSS_COMPILE)
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  endif()
endmacro (find_host_package)
