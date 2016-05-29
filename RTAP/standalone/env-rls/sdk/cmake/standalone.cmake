set(JVX_CMAKE_DIR ${JVX_SDK_PATH}/cmake)
	set(JVX_BASE_ROOT ${JVX_SDK_PATH})
	set(JVX_SUBPRODUCT_ROOT ${JVX_SDK_PATH})

	include(${JVX_CMAKE_DIR}/macros.cmake)
	include(${JVX_CMAKE_DIR}/platform.cmake)

	# Collect all components for a possible app bundle Mac OS
	### set(JVX_COLLECT_COMPONENTS_MAC_BUNDLE "" CACHE INTERNAL "all components")

	set(INSTALL_PATH_RELEASE_ROOT "${JVX_SDK_PATH}")
	set(INSTALL_PATH_RELEASE_ROOT_MIN_SLASH "$JVX_SDK_PATH}/")

	set(INSTALL_PATH_LIB_STATIC "${INSTALL_PATH_RELEASE_ROOT}/lib")
	set(INSTALL_PATH_INCLUDE_STATIC "${INSTALL_PATH_RELEASE_ROOT}/include")
	set(INSTALL_PATH_COMPONENTS_STATIC "${INSTALL_PATH_RELEASE_ROOT}/lib/jvxComponents-static")
	set(INSTALL_PATH_INCLUDE_COMPONENTS_STATIC "${INSTALL_PATH_RELEASE_ROOT}/include/jvxComponents-static")

	set(INSTALL_PATH_BIN "${INSTALL_PATH_RELEASE_ROOT}/bin")
	set(INSTALL_PATH_SCRIPTS "${INSTALL_PATH_RELEASE_ROOT}")
	set(INSTALL_PATH_IMAGES "${INSTALL_PATH_RELEASE_ROOT}/images")
	set(INSTALL_PATH_LIB_SHARED "${INSTALL_PATH_RELEASE_ROOT}/lib")
	set(INSTALL_PATH_INCLUDE_SHARED "${INSTALL_PATH_RELEASE_ROOT}/include")
	set(INSTALL_PATH_COMPONENTS_SHARED "${INSTALL_PATH_RELEASE_ROOT}/jvxComponents")
	set(INSTALL_PATH_INCLUDE_COMPONENTS_SHARED "${INSTALL_PATH_RELEASE_ROOT}/include/jvxComponents-shared")
	set(INSTALL_PATH_MATLAB "${INSTALL_PATH_RELEASE_ROOT}/matlab")
	set(INSTALL_PATH_OCTAVE "${INSTALL_PATH_RELEASE_ROOT}/octave")

	if(JVX_SDK_RUNTIME_PATH)
		set(INSTALL_PATH_BIN "${JVX_SDK_RUNTIME_PATH}/bin")
		set(INSTALL_PATH_SCRIPTS "${JVX_SDK_RUNTIME_PATH}")
		set(INSTALL_PATH_IMAGES "${JVX_SDK_RUNTIME_PATH}/images")
		set(INSTALL_PATH_LIB_SHARED "${JVX_SDK_RUNTIME_PATH}/lib")
		set(INSTALL_PATH_INCLUDE_SHARED "${JVX_SDK_RUNTIME_PATH}/include")
		set(INSTALL_PATH_COMPONENTS_SHARED "${JVX_SDK_RUNTIME_PATH}/jvxComponents")
		set(INSTALL_PATH_INCLUDE_COMPONENTS_SHARED "${JVX_SDK_RUNTIME_PATH}/include/jvxComponents-shared")
		set(INSTALL_PATH_MATLAB "${JVX_SDK_RUNTIME_PATH}/matlab")
		set(INSTALL_PATH_OCTAVE "${JVX_SDK_RUNTIME_PATH}/octave")
	endif()

	set(JVX_BASE_LIBS_INCLUDE_PATH "${JVX_SDK_PATH}/include")
	set(JVX_BASE_LIBS_LIB_PATH "${JVX_SDK_PATH}/lib")
	set(JVX_SUBPRODUCT_LIBS_INCLUDE_PATH "${JVX_SDK_PATH}/include")
	set(JVX_SUBPRODUCT_LIBS_LIB_PATH "${JVX_SDK_PATH}/lib")

	set(JVX_USES_SDK TRUE)

	# common include directories
	include_directories(
		${JVX_BASE_ROOT}/software
		${JVX_SUBPRODUCT_ROOT}/software
		${JVX_BASE_ROOT}/software/platform/${JVX_OS}
		${JVX_BASE_LIBS_INCLUDE_PATH}/jvx-dsp-base/include/
		${JVX_BASE_LIBS_INCLUDE_PATH}/jvx-dsp-base/include/os-${JVX_OS}
		${JVX_BASE_LIBS_INCLUDE_PATH}/jvxComponents-static
	)

	include(${JVX_CMAKE_DIR}/standalone-cfg.cmake)
	set(INSTALL_PATH_MATLAB_SUBPROJECT "${INSTALL_PATH_MATLAB}/m-files/${JVX_PRODUCT}Properties")

###
# configure external packages
###

header_message("Configuring external tools")

# we always need an fft library
find_fft()

# configure matlab and/or octave
if(JVX_USE_PART_OCTAVE)
  find_octave()
endif()
if(JVX_USE_PART_MATLAB)
  find_matlab()
#   find_package(Matlab COMPONENTS MX_LIBRARY)
#   set(MATLAB_MEX_SUFFIX ".mexa64")
#   set(JVX_CXX_FLAGS_MEX_MATLAB "-D__STDC_UTF_16__")
#   message(${Matlab_FOUND})
#   message(${Matlab_ROOT_DIR})
# #  message(${Matlab_MAIN_PROGRAM})
#   message(${Matlab_INCLUDE_DIRS})
#   message(${Matlab_MEX_LIBRARY})
#   message(${Matlab_MX_LIBRARY})
# #  message(${Matlab_ENG_LIBRARY})
#   message(${Matlab_LIBRARIES})
# #  message(${Matlab_MEX_COMPILER})
endif()

if(JVX_USE_QT)
	# qt, if desired test the system for what is required
	message("--> Looking for Qt5")

	# If we do a cross compile, the moc, uic and rcc must be called from another place
	# than that at which the libraries and headers exist
	if(JVX_CROSS_COMPILE)
		add_executable(Qt5::moc IMPORTED)
		set(imported_location "moc")
		set_target_properties(Qt5::moc PROPERTIES
			IMPORTED_LOCATION ${imported_location}
		)
		# For CMake automoc feature
		get_target_property(QT_MOC_EXECUTABLE Qt5::moc LOCATION)

		add_executable(Qt5::uic IMPORTED)
		set(imported_location "uic")
		set_target_properties(Qt5::uic PROPERTIES
			IMPORTED_LOCATION ${imported_location}
		)
		# For CMake automoc feature
		get_target_property(QT_UIC_EXECUTABLE Qt5::uic LOCATION)

		add_executable(Qt5::rcc IMPORTED)
		set(imported_location "rcc")
		set_target_properties(Qt5::rcc PROPERTIES
			IMPORTED_LOCATION ${imported_location}
		)
		# For CMake automoc feature
		get_target_property(QT_RCC_EXECUTABLE Qt5::rcc LOCATION)

    add_executable(Qt5::qmake IMPORTED)
		set(imported_location "qmake")
		set_target_properties(Qt5::qmake PROPERTIES
			IMPORTED_LOCATION ${imported_location}
		)
		# For CMake automoc feature
		get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake LOCATION)

	endif()

	find_package(Qt5Core)
	if(NOT ${Qt5Core_FOUND})
		set(JVX_USE_QT FALSE)
		message("    lib qt5 not available.")
	else()
		find_package(Qt5Gui REQUIRED)
		find_package(Qt5Widgets REQUIRED)
		find_package(Qt5Concurrent REQUIRED)
		find_package(Qt5PrintSupport REQUIRED)
		set(JVX_QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent Qt5::PrintSupport)
		get_target_property(QT_LIB_PATH Qt5::Core LOCATION)
		get_filename_component(QT_LIB_PATH ${QT_LIB_PATH} DIRECTORY)
		message("    lib: ${QT_LIB_PATH}")
	endif()

	# configure qwt (the lib is built in-tree)
	set(JVX_QWT_VERSION "6.1.2")
	set(JVX_QWT_POLAR_VERSION "1.1.0")

else()
	message("--> Qt5 not used")

	# Deactivate the depending QT libraries
	set(JVX_USE_QWT FALSE)
	set(JVX_USE_QWT_POLAR FALSE)
	set(JVX_USE_QT_WIDGETS FALSE)
endif()

if(JVX_USE_PCAP)
	find_pcap()
endif()

	# Add path to place where all the libraries are located
	link_directories(
		${JVX_SDK_PATH}/lib
		${JVX_SDK_PATH}/lib/jvxComponents-static
	)
	if(${JVX_OS} MATCHES "windows")

		# For nmake makefiles, we need to be accurate
		set(JVX_PCG "${JVX_SDK_PATH}/bin/jvxExPropC.exe")
		set(JVX_PCG_MATLAB "${JVX_SDK_PATH}/bin/jvxExPropMat.exe")
		set(JVX_PCG "${JVX_SDK_PATH}/bin/jvxExPropC.exe")
	else()
		set(JVX_PCG "${JVX_SDK_PATH}/bin/jvxExPropC")
		set(JVX_PCG_MATLAB "${JVX_SDK_PATH}/bin/jvxExPropMat")
		set(JVX_PCG "${JVX_SDK_PATH}/bin/jvxExPropC")
	endif()