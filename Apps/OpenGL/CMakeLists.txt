#dependency check
set(deps FitGL)

checkDependencies("OpenGL Examples" "${deps}")

set(apps)

file(GLOB dirs  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(dir ${dirs})
	if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
		set(FitGL_BUILD_APP_${dir} ON CACHE BOOL "Build ${app} application.")
		if(NOT FitGL_BUILD_APP_${dir})
		continue()
		endif()
		 
		if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
			add_subdirectory(${dir})
		else()
			LIST(APPEND apps ${dir})
		endif()
	endif()
endforeach()


file(GLOB commonShaders ${FitGL_SOURCE_DIR}/resources/shaders/*)

foreach(app ${apps})
	project(${app})
	file(GLOB resources ${CMAKE_CURRENT_SOURCE_DIR}/${app}/Resources/*)
	file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/${app}/*)
	add_executable(${app} ${src} ${resources} ${commonShaders})
	source_group("Resources" FILES ${resources})
	source_group("Common shaders" FILES ${commonShaders})
	target_link_libraries(${app} ${deps})
	set_target_properties(${app} PROPERTIES FOLDER "OpenGL")

	target_compile_definitions(${app} PRIVATE -DAPP_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/${app}/Resources/")
endforeach()
