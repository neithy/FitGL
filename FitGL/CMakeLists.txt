set(LIB_NAME FitGL)

set(deps Fit geGL)

checkDependencies("${LIB_NAME}" "${deps}")

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME}/include)

set(includes
	${HEADER_PATH}/OpenGLApp.h
	${HEADER_PATH}/FitGL.h
	${HEADER_PATH}/FitGLClasses.h
	${HEADER_PATH}/geGLClasses.h
	${HEADER_PATH}/ShaderUtils.h
	${HEADER_PATH}/LoaderGL.h
	${HEADER_PATH}/TimingMonitorGL.h
  )

set(sources
	src/OpenGLApp.cpp
	src/LoaderTextureGL.cpp
	src/TimingMonitorGL.cpp
  )

add_library(${LIB_NAME}
  ${includes}
  ${sources}
  )

set(include_dir ${HEADER_PATH})
set(include_dir_export ${HEADER_PATH})

target_link_libraries(${LIB_NAME} ${deps})
set_target_properties(${LIB_NAME} PROPERTIES
  INCLUDE_DIRECTORIES "${include_dir}"
  INTERFACE_INCLUDE_DIRECTORIES "${include_dir_export}"
  )

install(FILES
  ${includes_export} DESTINATION include/${LIB_NAME}
  )

install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )

