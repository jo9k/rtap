# build unstable targets only with Unstable build types
if((NOT JVX_DEPLOY_UNSTABLE) AND IS_UNSTABLE)
  message("Excluding unstable target ${JVX_TARGET_NAME} from build.")
  return()
endif()

###
# reset some stuff
###
#set(ADDITIONAL_CXX_FLAGS "")
set(ADDITIONAL_LIBS "")
set(ADDITIONAL_SOURCES "")
set(ADDITIONAL_UI_SOURCES "")
set(ADDITIONAL_COMPILE_DEFINITIONS "")
#set(ADDITIONAL_LINKER_FLAGS "")
#set(GENERATED_FILES "")
if(IS_UNSTABLE)
  set(INSTALL_COMPONENT "unstable")
else()
  set(INSTALL_COMPONENT "release")
endif()

###
# QT support
###
if(LOCAL_UI_SOURCES OR ADDITIONAL_UI_SOURCES OR USE_QT_LIBRARIES)
  message("    > Executable (QT)")
  cmake_policy(SET CMP0020 NEW)
  set(CMAKE_AUTOMOC ON)

  if(JVX_USE_QT_WIDGETS)
    set(ADDITIONAL_COMPILE_DEFINITIONS "${ADDITIONAL_COMPILE_DEFINITIONS};QT_ALLOW_QT_WIDGETS")
    set(CMAKE_AUTOMOC_MOC_OPTIONS "-DQT_ALLOW_QT_WIDGETS")
  endif(JVX_USE_QT_WIDGETS)

  QT5_WRAP_UI(UI_HEADERS ${LOCAL_UI_SOURCES} ${ADDITIONAL_UI_SOURCES})
  qt5_add_resources(QT_RESOURCES ${LOCAL_QT_RESOURCES})
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} ${JVX_QT_LIBS})
else()
  message("    > Executable")
endif()

# MAC OS bundle
if(JVX_OS MATCHES "macosx" AND JVX_MAC_OS_CREATE_BUNDLE)
    set(CMAKE_MACOSX_RPATH TRUE) # Do not use RPATHs if you want to create deployable bundles
    set(MACOSX_BUNDLE_ICON_FILE Icon.icns)
    set(JVX_ICON ${CMAKE_CURRENT_SOURCE_DIR}/xcode/${JVX_TARGET_NAME}/Icons.xcassets/Icon.icns)
    set_source_files_properties(${JVX_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${JVX_TARGET_NAME} MACOSX_BUNDLE ${LOCAL_SOURCES} ${ADDITIONAL_SOURCES} ${QT_RESOURCES} ${UI_HEADERS}  ${JVX_ICON})
else()
#message("add_executable(${JVX_TARGET_NAME} ${LOCAL_SOURCES} ${ADDITIONAL_SOURCES} ${QT_RESOURCES} ${UI_HEADERS})")
  add_executable(${JVX_TARGET_NAME} ${LOCAL_SOURCES} ${ADDITIONAL_SOURCES} ${QT_RESOURCES} ${UI_HEADERS})
endif()

# set flags
target_compile_definitions(${JVX_TARGET_NAME} PRIVATE ${LOCAL_COMPILE_DEFINITIONS} ${ADDITIONAL_COMPILE_DEFINITIONS})
set_target_properties(${JVX_TARGET_NAME} PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${JVX_CMAKE_LINKER_FLAGS_EXEC} ${LOCAL_CXX_FLAGS}"
  LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${JVX_CMAKE_LINKER_FLAGS_EXEC} ${LOCAL_LINKER_FLAGS}")
target_link_libraries(${JVX_TARGET_NAME} ${LOCAL_LIBS} ${ADDITIONAL_LIBS} ${JVX_SYSTEM_LIBRARIES} )

jvx_displayFlags(${JVX_TARGET_NAME})

###
# installation
###

# The target itself (*.exe)
if(JVX_OS MATCHES "macosx" AND JVX_MAC_OS_CREATE_BUNDLE)

    set(CMAKE_MACOSX_RPATH ON)
    SET(CMAKE_SKIP_BUILD_RPATH TRUE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set_target_properties(${JVX_TARGET_NAME} PROPERTIES INSTALL_RPATH "/Users/javox-admin/Qt5.5.0/5.5/clang_64/lib/")
    install(TARGETS ${JVX_TARGET_NAME} DESTINATION ${INSTALL_PATH_RELEASE_ROOT} COMPONENT ${INSTALL_COMPONENT})
    # message("L: ${_locN}")
    SET(allLibs "")
    SET(allBLibs "")
    set(allAPluginsFrom "")
    set(allAPluginsTo "")
    set(allAPluginsFix "")
    # message("Collected items: ${JVX_COLLECT_COMPONENTS_MAC_BUNDLE}")

    # Collect all Qt plugins, seems that all plugins are associated with QtGui
    if(LOCAL_UI_SOURCES OR ADDITIONAL_UI_SOURCES)
      foreach(plugin ${Qt5Gui_PLUGINS})
	get_target_property(_loc ${plugin} LOCATION)
	get_filename_component(_locN ${_loc} NAME)
	SET(allLibs ${allLibs} ${_loc})
	SET(allBLibs ${allBLibs} ${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/PlugIns/${_locN})
      endforeach(plugin ${Qt5Gui_PLUGINS})
      install(FILES ${allLibs} DESTINATION ${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/PlugIns)
    endif()
    # message(" xxx> All qt libs etc: ${allLibs}")
    # message(" xxx> Target path ${CMAKE_CURRENT_BINARY_DIR}/${JVX_TARGET_NAME}.app")

    # Collect all audio components
    if(JVX_MAC_OS_CREATE_BUNDLE_AUDIO_PLUGINS)
      foreach(aplugin ${JVX_COLLECT_COMPONENTS_MAC_BUNDLE})
        SET(allAPluginsFrom ${allAPluginsFrom} ${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_COMPONENTS_SHARED}/lib${aplugin})
        SET(allAPluginsTo ${allAPluginsTo}
          ${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/${INSTALL_PATH_COMPONENTS_SHARED}/lib${aplugin})
        SET(allAPluginsFix ${allAPluginsFix}
          ${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/${INSTALL_PATH_COMPONENTS_SHARED}/lib${aplugin})
      endforeach()
      install(FILES ${allAPluginsFrom} DESTINATION ${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/${INSTALL_PATH_COMPONENTS_SHARED})
    endif()
    # message(" xxx> All audio libs: ${allAPluginsFrom}")
    # message(" xxx> Target path ${CMAKE_CURRENT_BINARY_DIR}/${JVX_TARGET_NAME}.app")

    install(DIRECTORY ${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_LIB_SHARED}/
      DESTINATION ${INSTALL_PATH_RELEASE_ROOT}/${JVX_TARGET_NAME}.app/Contents/lib/ FILES_MATCHING PATTERN "*.dylib")
    INSTALL(CODE "include(BundleUtilities)
                  fixup_bundle(\"${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app\"
		  \"${allBLibs};${allAPluginsFix}\"
                  \"${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/lib;${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH_RELEASE_ROOT_MIN_SLASH}${JVX_TARGET_NAME}.app/Contents/Plugins\")" COMPONENT Runtime)
else()
	if(NOT JVX_DO_NO_INSTALL)
		install(TARGETS ${JVX_TARGET_NAME} DESTINATION ${INSTALL_PATH_BIN} COMPONENT ${INSTALL_COMPONENT})
	endif()
endif()

# The relevant images (if used)
if(LOCAL_IMAGES_SOURCE_DIR)
  install(DIRECTORY ${LOCAL_IMAGES_SOURCE_DIR}/ DESTINATION ${INSTALL_PATH_IMAGES} COMPONENT ${INSTALL_COMPONENT} FILES_MATCHING PATTERN "*.png")
endif(LOCAL_IMAGES_SOURCE_DIR)

# The corresponding start script
if(LOCAL_START_SCRIPT)
  install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/${JVX_OS}/${LOCAL_START_SCRIPT}${JVX_SCRIPT_EXTENSION} DESTINATION ${INSTALL_PATH_SCRIPTS} COMPONENT ${INSTALL_COMPONENT})
else(LOCAL_START_SCRIPT)
  if(CONFIGURE_LOCAL_START_SCRIPT)
    configure_file(${JVX_BASE_ROOT}/software/templates/scripts/start${JVX_SCRIPT_EXTENSION}.${JVX_OS}.in ${CMAKE_CURRENT_BINARY_DIR}/start-${JVX_TARGET_NAME}${JVX_SCRIPT_EXTENSION})
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/start-${JVX_TARGET_NAME}${JVX_SCRIPT_EXTENSION} DESTINATION ${INSTALL_PATH_SCRIPTS} COMPONENT ${INSTALL_COMPONENT})
  endif(CONFIGURE_LOCAL_START_SCRIPT)
endif(LOCAL_START_SCRIPT)
