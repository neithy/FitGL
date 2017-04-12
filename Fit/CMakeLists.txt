set(LIB_NAME Fit)

set(deps SDL2 glm imgui)

checkDependencies("${LIB_NAME}" "${deps}")

if(TARGET FreeImage AND TARGET FreeImagePlus)
	add_definitions(-DUSE_FREEIMAGE)
	set(deps ${deps} FreeImage FreeImagePlus)	
endif()

if(TARGET assimp)
	add_definitions(-DUSE_ASSIMP)
	include_directories(${ASSIMP_INCLUDE_DIR})	
	set(deps ${deps} assimp)	
endif()

set(HEADER_PATH ${FitGL_SOURCE_DIR}/${LIB_NAME}/include)

set(includes
	${HEADER_PATH}/BaseApp.h
	${HEADER_PATH}/bunny.h
	${HEADER_PATH}/Fit.h
	${HEADER_PATH}/FitClasses.h
	${HEADER_PATH}/SDLEventProc.h
	${HEADER_PATH}/SDLWindow.h
	${HEADER_PATH}/Timer.h
	${HEADER_PATH}/Gui.h
	${HEADER_PATH}/Loader.h
	${HEADER_PATH}/Camera.h
	${HEADER_PATH}/Manipulator.h
	${HEADER_PATH}/Image.h
	${HEADER_PATH}/FileWatcher.h
  )

set(sources
	src/BaseApp.cpp
	src/SDLEventProc.cpp
	src/SDLWindow.cpp
	src/Loader.cpp
	src/LoaderText.cpp
	src/LoaderImage.cpp
	src/LoaderBinary.cpp
	src/Gui.cpp
	src/Manipulator.cpp
	src/Camera.cpp
	src/FileWatcher.cpp
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

