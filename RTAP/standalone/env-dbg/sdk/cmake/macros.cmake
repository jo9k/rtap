# print a clearly visible header message
macro(header_message text)
  message("")
  message("---------------------------------------------------------")
  message("${text}")
  message("---------------------------------------------------------")
  message("")
endmacro(header_message)

# execute sed on an input file
macro(jvx_one_sed_entry_define target input output commandline)
  add_custom_command(
    TARGET ${target}
    COMMAND ${SED}
    ARGS \"${commandline}\" ${input} > ${output}
    DEPENDS ${input}
    COMMENT "${target} :: ${input} -> ${output}")
endmacro(jvx_one_sed_entry_define)

# add object handles to a library
macro(jvx_addObjectAccess target suffix)
  message(WARNING "CMAKE MACRO jvx_addObjectAccess IS DEPRECATED")
  jvx_one_sed_entry_define(${target}${suffix} ${JVX_BASE_ROOT}/software/templates/sed_seeds/objectAccess.h.sed
    ${CMAKE_CURRENT_BINARY_DIR}/${target}${suffix}.h
    "s/JVX_PREPROCESSOR_TOKEN/__${target}${suffix}__H__/g\;s/JVX_OBJECT_INIT_FUNCTION/${target}_init/g\;s/JVX_OBJECT_TERMINATE_FUNCTION/${target}_terminate/g")
endmacro(jvx_addObjectAccess)

# add a collection of subdirs to build process
macro(jvx_addSubdirs message subdirlist)
  header_message(${message})
  foreach(oneProject ${subdirlist})
    add_subdirectory(${oneProject})
    message("")
  endforeach(oneProject ${subdirlist})
endmacro(jvx_addSubdirs)

# display flags
macro(jvx_displayFlags target)
  get_target_property(USED_COMPILE_DEFINITIONS ${target} COMPILE_DEFINITIONS)
  message("      Compile definitions: ${USED_COMPILE_DEFINITIONS}")
  get_target_property(USED_COMPILE_FLAGS ${target} COMPILE_FLAGS)
  message("      Compiler flags:      ${USED_COMPILE_FLAGS}")
  get_target_property(USED_LINK_FLAGS ${target} LINK_FLAGS)
  message("      Linker flags:        ${USED_LINK_FLAGS}")
  get_target_property(USED_LINK_LIBRARIES ${target} LINK_LIBRARIES)
  if("${USED_LINK_LIBRARIES}" MATCHES "USED_LINK_LIBRARIES-NOTFOUND")
    message("      Linked libraries:")
  else()
    message("      Linked libraries:    ${USED_LINK_LIBRARIES}")
  endif()
endmacro(jvx_displayFlags)

macro(jvx_genMatProperties targetname componenttype componentprefix localfilelist )
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/m-files/+${targetname})
  foreach(PCGFILE ${localfilelist})
	message("    > PCG MAT Generator: ${PCGFILE} for component of type ${componenttype}, generator prefix ${componentprefix}")
#	message("exec_program(\"${JVX_PCG_MATLAB} ${PCGFILE} -o ${CMAKE_CURRENT_BINARY_DIR}/m-files/+${targetname} -ctp ${componenttype} -cpf ${componentprefix}\")")
	install(CODE "exec_program(\"${JVX_PCG_MATLAB} ${PCGFILE} -o ${CMAKE_CURRENT_BINARY_DIR}/m-files/+${targetname} -ctp ${componenttype} -cpf ${componentprefix}\")")
  endforeach()
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/m-files/+${targetname} DESTINATION ${INSTALL_PATH_MATLAB_SUBPROJECT})
endmacro(jvx_genMatProperties)
