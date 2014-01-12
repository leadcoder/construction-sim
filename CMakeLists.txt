cmake_minimum_required (VERSION 2.8)
project (ConstructionSim)

# The version number.
set (CSIM_VERSION_MAJOR 1)
set (CSIM_VERSION_MINOR 0)

#set(CMAKE_MODULE_PATH
#  "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
#	)
#include (Common)

set(GASS_DIR $ENV{GASS_HOME} CACHE PATH "GASS folder")
set(GASS_DEPENDENCIES_DIR $ENV{GASS_DEPENDENCIES} CACHE PATH "GASS 3rd-party dependency folder")

if(EXISTS "${GASS_DEPENDENCIES_DIR}/boost-v1.54")
	set(BOOST_DIR  "${GASS_DEPENDENCIES_DIR}/boost-v1.54" CACHE PATH "BOOST folder")
else()
	set(BOOST_DIR $ENV{BOOST_HOME} CACHE PATH "BOOST folder")
endif()

message( STATUS "CMAKE_CL_64: ${CMAKE_CL_64}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #dynnamic libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #dlls and exe

set(CMAKE_DEBUG_POSTFIX  "_d" CACHE STRING "add a postfix, usually d on windows")

set(CSIM_INSTALL_BIN_DIR  "bin")
set(CSIM_INSTALL_BIN_DIR_DEBUG "bin/debug")
set(CSIM_INSTALL_BIN_DIR_RELEASE "bin/release")

set(GASS_COMMON_INCLUDE_DIRS "${BOOST_DIR}"  "${GASS_DIR}/include" 	"${GASS_DEPENDENCIES_DIR}/tinyxml"	"${GASS_DEPENDENCIES_DIR}/tbb/include" "${GASS_DEPENDENCIES_DIR}/angelscript/include")
set(GASS_COMMON_LIB_DIRS "${BOOST_DIR}/lib/" "${GASS_DEPENDENCIES_DIR}/lib/" "${GASS_DEPENDENCIES_DIR}/angelscript/lib" "${GASS_DIR}/lib/")
set(GASS_COMMON_LINK_LIBRARIES
	optimized GASSCore
	optimized GASSSim
	optimized tinyxml
	optimized angelscript
	debug GASSCore_d
	debug GASSSim_d
	debug tinyxml_d 
	debug angelscriptd
	
	)
add_subdirectory( Source )
