
MACRO(LIST_CONTAINS var value)
  SET(${var})
  FOREACH (value2 ${ARGN})
    IF (${value} STREQUAL ${value2})
      SET(${var} TRUE)
    ENDIF (${value} STREQUAL ${value2})
  ENDFOREACH (value2)
ENDMACRO(LIST_CONTAINS)

MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)

  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})
    LIST_CONTAINS(is_arg_name ${arg} ${arg_names})
    IF (is_arg_name)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name)
      LIST_CONTAINS(is_option ${arg} ${option_names})
      IF (is_option)
	SET(${prefix}_${arg} TRUE)
      ELSE (is_option)
	SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option)
    ENDIF (is_arg_name)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

MACRO(CAR var)
  SET(${var} ${ARGV1})
ENDMACRO(CAR)

MACRO(CDR var junk)
  SET(${var} ${ARGN})
ENDMACRO(CDR)

MACRO(ADD_HEADERS targetDir)    
  file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")  
  INSTALL(FILES ${header_files} DESTINATION ${targetDir})
    
ENDMACRO(ADD_HEADERS)

MACRO(ADD_COMPUCELL3D_PLUGIN)
   PARSE_ARGUMENTS(LIBRARY
    "LINK_LIBRARIES;DEPENDS;SUFFIX;EXTRA_COMPILER_FLAGS"
    ""
    ${ARGN}
    )
  CAR(LIBRARY_NAME ${LIBRARY_DEFAULT_ARGS})
  CDR(LIBRARY_SOURCES ${LIBRARY_DEFAULT_ARGS})

  
  INCLUDE_DIRECTORIES ( 
        ${COMPUCELL3D_SOURCE_DIR}/core
        ${COMPUCELL3D_SOURCE_DIR}/core/CompuCell3D/plugins
  )  
    
  file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx" "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
  
  
  
  ADD_LIBRARY(${LIBRARY_NAME}Shared SHARED ${LIBRARY_SOURCES} ${source_files})
  # ADD_LIBRARY(${LIBRARY_NAME}Shared SHARED ${LIBRARY_SOURCES})
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME}Shared ${LIBRARY_LINK_LIBRARIES})
    IF(USE_LIBRARY_VERSIONS)
      SET_TARGET_PROPERTIES(
      ${LIBRARY_NAME}Shared PROPERTIES
      ${COMPUCELL3D_LIBRARY_PROPERTIES})
    ENDIF(USE_LIBRARY_VERSIONS)

  SET_TARGET_PROPERTIES(${LIBRARY_NAME}Shared  PROPERTIES OUTPUT_NAME CC3D${LIBRARY_NAME}${LIBRARY_SUFFIX} COMPILE_FLAGS "${LIBRARY_EXTRA_COMPILER_FLAGS}")
  
  INSTALL_TARGETS(${COMPUCELL3D_INSTALL_PLUGIN_DIR} RUNTIME_DIRECTORY ${COMPUCELL3D_INSTALL_PLUGIN_DIR} ${LIBRARY_NAME}Shared)

  
  # installing headers  
  

  ADD_HEADERS("${CMAKE_INSTALL_PREFIX}/include/CompuCell3D/CompuCell3D/plugins/${LIBRARY_NAME}")
  
ENDMACRO(ADD_COMPUCELL3D_PLUGIN)





MACRO(ADD_COMPUCELL3D_STEPPABLE)
   PARSE_ARGUMENTS(LIBRARY
    "LINK_LIBRARIES;DEPENDS;SUFFIX;EXTRA_COMPILER_FLAGS"
    ""
    ${ARGN}
    )
  CAR(LIBRARY_NAME ${LIBRARY_DEFAULT_ARGS})
  CDR(LIBRARY_SOURCES ${LIBRARY_DEFAULT_ARGS})


  
  INCLUDE_DIRECTORIES ( 
        ${COMPUCELL3D_SOURCE_DIR}/core
        ${COMPUCELL3D_SOURCE_DIR}/core/CompuCell3D/steppables
  )  
  
  file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx" "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )  
  
  ADD_LIBRARY(${LIBRARY_NAME}Shared SHARED ${LIBRARY_SOURCES} ${source_files} )
  # ADD_LIBRARY(${LIBRARY_NAME}Shared SHARED ${LIBRARY_SOURCES})
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME}Shared ${LIBRARY_LINK_LIBRARIES})

    IF(USE_LIBRARY_VERSIONS)
      SET_TARGET_PROPERTIES(
      ${LIBRARY_NAME}Shared PROPERTIES
      ${COMPUCELL3D_LIBRARY_PROPERTIES})
    ENDIF(USE_LIBRARY_VERSIONS)

  SET_TARGET_PROPERTIES(${LIBRARY_NAME}Shared  PROPERTIES OUTPUT_NAME CC3D${LIBRARY_NAME}${LIBRARY_SUFFIX} COMPILE_FLAGS "${LIBRARY_EXTRA_COMPILER_FLAGS}")

  # INSTALL_TARGETS(/lib/CompuCell3DSteppables RUNTIME_DIRECTORY /lib/CompuCell3DSteppables ${LIBRARY_NAME}Shared)
  INSTALL_TARGETS(${COMPUCELL3D_INSTALL_STEPPABLE_DIR} RUNTIME_DIRECTORY ${COMPUCELL3D_INSTALL_STEPPABLE_DIR} ${LIBRARY_NAME}Shared)
  
  
  ADD_HEADERS("${CMAKE_INSTALL_PREFIX}/include/CompuCell3D/CompuCell3D/steppables/${LIBRARY_NAME}")
  
ENDMACRO(ADD_COMPUCELL3D_STEPPABLE)