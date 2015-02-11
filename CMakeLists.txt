cmake_minimum_required (VERSION 2.8)
project (ConstructionSim)

# The version number.
set (CSIM_VERSION_MAJOR 1)
set (CSIM_VERSION_MINOR 0)

set(GASS_DIR $ENV{GASS_HOME} CACHE PATH "GASS folder")
set(GASS_DEPENDENCIES_DIR $ENV{GASS_DEPENDENCIES} CACHE PATH "GASS 3rd-party dependency folder")

SET(CMAKE_MODULE_PATH
	${GASS_DIR}/CMake
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #dynnamic libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #dlls and exe

set(CMAKE_DEBUG_POSTFIX  "_d" CACHE STRING "add a postfix, usually d on windows")

set(CSIM_INSTALL_BIN_DIR  "bin")
set(CSIM_INSTALL_BIN_DIR_DEBUG "bin/debug")
set(CSIM_INSTALL_BIN_DIR_RELEASE "bin/release")

add_subdirectory( Source )