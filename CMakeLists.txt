cmake_minimum_required(VERSION 3.6.0)

PROJECT(FitGL)


# send all exes to bin/ dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# c++17 for gcc
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # -W -Wall")# -pedantic")
ENDIF()

# set path to find modules
set(CMAKE_MODULE_PATH "${FitGL_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

# enable folders for visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#find all needed libraries

find_package(ste HINTS ${FitGL_SOURCE_DIR}/3rdParty/ste)
         
         

include(FitMacros)

find_package(GPUEngine QUIET COMPONENTS geGL geSG geVu geVuSG CONFIG)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(FreeImage QUIET)
find_package(FreeImagePlus QUIET)
find_package(assimp QUIET)
#find_package(vulkan QUIET)

include_directories(${FitGL_SOURCE_DIR}/Fit)

add_definitions(-DRESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources/")

add_subdirectory(3rdParty/imgui)
add_subdirectory(Fit)
add_subdirectory(FitGL)

add_subdirectory(Apps/OpenGL)


