# build unstable targets only with Unstable build types
if((NOT JVX_DEPLOY_UNSTABLE) AND IS_UNSTABLE)
  message("Excluding unstable target ${JVX_TARGET_NAME} from build.")
  return()
endif()

###
# reset some stuff
###
set(ADDITIONAL_CXX_FLAGS "")
set(ADDITIONAL_LIBS "")
set(ADDITIONAL_SOURCES "")
set(ADDITIONAL_COMPILE_DEFINITIONS "")
set(ADDITIONAL_LINKER_FLAGS "")
set(GENERATED_FILES "")
if(IS_UNSTABLE)
  set(INSTALL_COMPONENT "unstable")
else()
  set(INSTALL_COMPONENT "release")
endif()


###
# JVX Components are always built static AND shared
###
if(IS_JVX_COMPONENT)
  set(BUILD_STATIC TRUE)
  if(JVX_COMPONENT_ONLY_STATIC)
    set(BUILD_SHARED FALSE)
  else(JVX_COMPONENT_ONLY_STATIC)
    if(NOT JVX_DISABLE_ALL_SHARED)
      set(BUILD_SHARED TRUE)
    else()
      set(BUILD_SHARED FALSE)
    endif()
  endif(JVX_COMPONENT_ONLY_STATIC)
endif()

###
# run PCG
###
if(LOCAL_PCG_FILES)
  message("    > PCG")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
	foreach(PCGFILE ${LOCAL_PCG_FILES})
		get_filename_component(PCGDIR ${PCGFILE} DIRECTORY)
		get_filename_component(PCGTOKEN ${PCGFILE} NAME_WE)
		message("      ${PCGTOKEN}")
		add_custom_command(
			COMMAND ${JVX_PCG}
			ARGS ${PCGDIR}/${PCGTOKEN}.pcg -o ${CMAKE_CURRENT_BINARY_DIR}/generated/pcg_${PCGTOKEN}.h
			DEPENDS ${PCGDIR}/${PCGTOKEN}.pcg ${JVX_PCG}
			OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/generated/pcg_${PCGTOKEN}.h)
			set(GENERATED_FILES ${GENERATED_FILES} ${CMAKE_CURRENT_BINARY_DIR}/generated/pcg_${PCGTOKEN}.h)
	endforeach()
endif()

###
# run MCG
###
if(LOCAL_MCG_FILES)
  message("    > MCG")
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
  foreach(MCGFILE ${LOCAL_MCG_FILES})
    get_filename_component(MCGDIR ${MCGFILE} DIRECTORY)
    get_filename_component(MCGTOKEN ${MCGFILE} NAME_WE)
    message("      ${MCGTOKEN}")
    add_custom_command(
      COMMAND ${JVX_MCG}
      ARGS ${MCGDIR}/${MCGTOKEN}.mcg --out-h ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}.h --out-hp ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}_prototypes.h
      DEPENDS ${MCGDIR}/${MCGTOKEN}.mcg ${JVX_MCG}
      OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}.h ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}_prototypes.h)
    set(GENERATED_FILES ${GENERATED_FILES} ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}.h)
    set(GENERATED_FILES ${GENERATED_FILES} ${CMAKE_CURRENT_BINARY_DIR}/generated/mcg_${MCGTOKEN}_prototypes.h)
  endforeach()
endif()

###
# run octave
###
if (LOCAL_OCTCONF_SCRIPTS OR USES_OTHER_LOCAL_OCTCONF_SCRIPTS)
  if(JVX_USE_PART_OCTAVE)
    message("    > Octave (config header generation)")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
    foreach(LOCAL_OCTCONF_SCRIPT ${LOCAL_OCTCONF_SCRIPTS})
      execute_process(COMMAND ${OCTAVE} -qf --no-gui ${LOCAL_OCTCONF_SCRIPT} show_filename OUTPUT_VARIABLE OUTFILE)
      string(REPLACE "\n" "" OUTFILE ${OUTFILE})
      set(OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/generated/${OUTFILE})
      message("      ${LOCAL_OCTCONF_SCRIPT} -> ${OUTFILE}")
      add_custom_command(
        COMMAND ${OCTAVE}
        ARGS -q ${LOCAL_OCTCONF_SCRIPT} ${OUTFILE}
        OUTPUT  ${OUTFILE}
        DEPENDS ${LOCAL_OCTCONF_SCRIPT} ${OCTAVE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
      set(GENERATED_FILES ${GENERATED_FILES} ${OUTFILE})
    endforeach()
  else()
    message("    > Octave not available (use pre-generated config header)")
    set(ADDITIONAL_COMPILE_DEFINITIONS "${ADDITIONAL_COMPILE_DEFINITIONS};JVX_DSP_USE_DEFAULT_OCTCONF")
  endif()
endif()

###
# static lib
###
if(BUILD_STATIC)
	if(CUDA_SOURCES)
		cuda_add_library(${JVX_TARGET_NAME_STATIC} STATIC ${LOCAL_SOURCES} ${CUDA_SOURCES} ${ADDITIONAL_SOURCES} ${GENERATED_FILES} )
		if(CUDA_USE_FFTW)
			CUDA_ADD_CUFFT_TO_TARGET(${JVX_TARGET_NAME_STATIC})
		endif(CUDA_USE_FFTW)
	else()
		add_library(${JVX_TARGET_NAME_STATIC} STATIC ${LOCAL_SOURCES} ${ADDITIONAL_SOURCES} ${GENERATED_FILES})
	endif()

  # Copy variable used by both BUILD_STATIC and BUILD_SHARED but modified in this block
  set(ADDITIONAL_COMPILE_DEFINITIONS_STATIC "${ADDITIONAL_COMPILE_DEFINITIONS}")

  # The cmake code generator for ios is somewhat buggy. In order to be able to debug libs compiled with
  # xcodebuild, the following was told to allow debugging
  if(IOS)
    set_xcode_property(${JVX_TARGET_NAME_STATIC} GCC_GENERATE_DEBUGGING_SYMBOLS YES)
  endif()

  # Javox component libs get different compile definitions and a generated header
  if(IS_JVX_COMPONENT)
    message("    > Javox component library (static)")
    set(ADDITIONAL_COMPILE_DEFINITIONS_STATIC "${ADDITIONAL_COMPILE_DEFINITIONS_STATIC};JVX_OBJECT_INIT_FUNCTION=${JVX_TARGET_NAME}_init;JVX_OBJECT_TERMINATE_FUNCTION=${JVX_TARGET_NAME}_terminate;JVX_MODULE_NAME=\"${JVX_MODULE_NAME}\"")
    configure_file(${JVX_BASE_ROOT}/software/templates/objectAccess.h.in ${CMAKE_CURRENT_BINARY_DIR}/${JVX_TARGET_NAME}.h)
  else()
    message("    > Library (static)")
  endif()

  # set flags
  target_compile_definitions(${JVX_TARGET_NAME_STATIC} PRIVATE ${LOCAL_COMPILE_DEFINITIONS} ${ADDITIONAL_COMPILE_DEFINITIONS_STATIC}  ${LOCAL_COMPILE_DEFINITIONS_STATIC})
  if(IS_C_LIB)
    set_target_properties(${JVX_TARGET_NAME_STATIC} PROPERTIES
      COMPILE_FLAGS "${CMAKE_C_FLAGS} ${JVX_CMAKE_C_FLAGS_STATIC_PIC} ${LOCAL_C_FLAGS} ${ADDITIONAL_C_FLAGS}"
      LINK_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${JVX_CMAKE_LINKER_FLAGS_STATIC_PIC} ${LOCAL_LINKER_FLAGS} ${ADDITIONAL_LINKER_FLAGS}")
  else()
    set_target_properties(${JVX_TARGET_NAME_STATIC} PROPERTIES
      COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${JVX_CMAKE_CXX_FLAGS_STATIC_PIC} ${LOCAL_CXX_FLAGS} ${ADDITIONAL_CXX_FLAGS}"
      LINK_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${JVX_CMAKE_LINKER_FLAGS_STATIC_PIC} ${LOCAL_LINKER_FLAGS} ${ADDITIONAL_LINKER_FLAGS}")
  endif()
  target_link_libraries(${JVX_TARGET_NAME_STATIC} ${LOCAL_LIBS} ${ADDITIONAL_LIBS} ${JVX_SYSTEM_LIBRARIES})

  jvx_displayFlags(${JVX_TARGET_NAME_STATIC})

  # installation
  if(IS_JVX_COMPONENT)

	if(JVX_RELEASE_SDK OR JVX_USES_SDK)
		# Here, we need to define a special rule for ios since there is a 5 year old (!) bug in cmake
		# There was someone working on a fix in 2015 but this seems not yet integrated into my cmake version
		if (IOS)
			install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}-${JVX_IOS_PLATFORM_NAME}/${JVX_STATIC_PREFIX}${JVX_TARGET_NAME_STATIC}.${JVX_STATIC_EXTENSION} DESTINATION ${INSTALL_PATH_COMPONENTS_STATIC})
		else()
			install(TARGETS ${JVX_TARGET_NAME_STATIC} DESTINATION ${INSTALL_PATH_COMPONENTS_STATIC} COMPONENT ${INSTALL_COMPONENT})
		endif()

		install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${JVX_TARGET_NAME}.h DESTINATION ${INSTALL_PATH_INCLUDE_COMPONENTS_STATIC} COMPONENT ${INSTALL_COMPONENT})
		install(FILES ${LOCAL_ADDITIONAL_INSTALL_HEADERS} DESTINATION ${INSTALL_PATH_INCLUDE_COMPONENTS_STATIC} COMPONENT ${INSTALL_COMPONENT})

	endif()
  else()

	if(JVX_RELEASE_SDK OR JVX_USES_SDK)

		get_filename_component(suffix ${CMAKE_CURRENT_SOURCE_DIR}  NAME )

		# Here, we need to define a special rule for ios since there is a 5 year old (!) bug in cmake
		# There was someone working on a fix in 2015 but this seems not yet integrated into my cmake version
		if (IOS)
			install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}-${JVX_IOS_PLATFORM_NAME}/${JVX_STATIC_PREFIX}${JVX_TARGET_NAME_STATIC}.${JVX_STATIC_EXTENSION} DESTINATION ${INSTALL_PATH_LIB_STATIC})
		else()
			install(TARGETS ${JVX_TARGET_NAME_STATIC} DESTINATION ${INSTALL_PATH_LIB_STATIC} COMPONENT ${INSTALL_COMPONENT})
		endif()

		if(INSTALL_DEDICATED_INCLUDE_TREE)
			install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${INSTALL_PATH_INCLUDE_STATIC}/${suffix} COMPONENT ${INSTALL_COMPONENT} FILES_MATCHING PATTERN "*.h")
		endif()

		if(LOCAL_ADDITIONAL_INSTALL_PATH)
			install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_ADDITIONAL_INSTALL_PATH}/ DESTINATION ${INSTALL_PATH_INCLUDE_STATIC}/${suffix}/${LOCAL_ADDITIONAL_INSTALL_PATH} COMPONENT ${INSTALL_COMPONENT} FILES_MATCHING PATTERN "*.h")
		endif()

		if(LOCAL_ADDITIONAL_INSTALL_HEADERS)
			install(FILES ${LOCAL_ADDITIONAL_INSTALL_HEADERS} DESTINATION ${INSTALL_PATH_INCLUDE_STATIC}/${suffix}/include COMPONENT ${INSTALL_COMPONENT})
		endif()

	endif()
  endif()

endif()


###
# shared lib
###
if(NOT JVX_RELEASE_SDK)

	# Build the shared objects only if in runtime relase mode
	if(BUILD_SHARED)

		if(CUDA_SOURCES)
			cuda_add_library(${JVX_TARGET_NAME} SHARED ${LOCAL_SOURCES} ${CUDA_SOURCES} ${ADDITIONAL_SOURCES} ${GENERATED_FILES})

			if(CUDA_USE_FFTW)
				CUDA_ADD_CUFFT_TO_TARGET(${JVX_TARGET_NAME})
			endif(CUDA_USE_FFTW)
		else()
			add_library(${JVX_TARGET_NAME} SHARED ${LOCAL_SOURCES} ${ADDITIONAL_SOURCES} ${GENERATED_FILES})
		endif()

		set(ADDITIONAL_COMPILE_DEFINITIONS_SHARED "${ADDITIONAL_COMPILE_DEFINITIONS}")

		# Javox component libs get different linker flags
		if(IS_JVX_COMPONENT)
			message("    > Javox component library (shared)")
			set(ADDITIONAL_COMPILE_DEFINITIONS_SHARED "${ADDITIONAL_COMPILE_DEFINITIONS_SHARED};JVX_MODULE_NAME=\"${JVX_MODULE_NAME}\"")
			set(ADDITIONAL_LINKER_FLAGS "${ADDITIONAL_LINKER_FLAGS} ${JVX_CMAKE_LINKER_FLAGS_SHARED_EXPORT_COMPONENTS}")
			set(JVX_COLLECT_COMPONENTS_MAC_BUNDLE "${JVX_COLLECT_COMPONENTS_MAC_BUNDLE};${JVX_TARGET_NAME}.${JVX_SHARED_EXTENSION}" CACHE INTERNAL "all components")
		else()
			message("    > Library (shared)")
		endif()

		# set flags
		target_compile_definitions(${JVX_TARGET_NAME} PRIVATE "JVX_SHARED_LIB" ${LOCAL_COMPILE_DEFINITIONS} ${ADDITIONAL_COMPILE_DEFINITIONS_SHARED} ${LOCAL_COMPILE_DEFINITIONS_SHARED})
		if(IS_C_LIB)
			set_target_properties(${JVX_TARGET_NAME} PROPERTIES
			COMPILE_FLAGS "${CMAKE_C_FLAGS} ${JVX_CMAKE_C_FLAGS_SHARED} ${LOCAL_C_FLAGS} ${ADDITIONAL_C_FLAGS}"
			LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LOCAL_LINKER_FLAGS} ${ADDITIONAL_LINKER_FLAGS}")
		else()
			set_target_properties(${JVX_TARGET_NAME} PROPERTIES
			COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${JVX_CMAKE_CXX_FLAGS_SHARED} ${LOCAL_CXX_FLAGS} ${ADDITIONAL_CXX_FLAGS}"
			LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LOCAL_LINKER_FLAGS} ${ADDITIONAL_LINKER_FLAGS}")
		endif()
		target_link_libraries(${JVX_TARGET_NAME} ${JVX_SYSTEM_LIBRARIES} ${LOCAL_LIBS} ${ADDITIONAL_LIBS} ${JVX_SYSTEM_LIBRARIES})

		jvx_displayFlags(${JVX_TARGET_NAME})

		# installation
		if(IS_JVX_COMPONENT)
			install(TARGETS ${JVX_TARGET_NAME}
			RUNTIME DESTINATION ${INSTALL_PATH_COMPONENTS_SHARED}
			LIBRARY DESTINATION ${INSTALL_PATH_COMPONENTS_SHARED} COMPONENT ${INSTALL_COMPONENT})
		else()
			install(TARGETS ${JVX_TARGET_NAME}
				RUNTIME DESTINATION ${INSTALL_PATH_LIB_SHARED}
				LIBRARY DESTINATION ${INSTALL_PATH_LIB_SHARED} COMPONENT ${INSTALL_COMPONENT})
		endif()
	endif()
endif()


###
# error
###
if(NOT BUILD_SHARED AND NOT BUILD_STATIC)
  message(FATAL_ERROR "Library module must define BUILD_STATIC or BUILD_SHARED")
endif()

if(DEFINED JVX_SUBPRODUCT_ROOT)
           include(${JVX_SUBPRODUCT_ROOT}/cmake/lib.${JVX_PRODUCT}.cmake)
endif()
