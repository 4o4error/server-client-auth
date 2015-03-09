MESSAGE(STATUS "")
MESSAGE(STATUS "### wELCOME TO THE SERVER-CLIENT AUTH LIBRARY ###")
MESSAGE(STATUS "************************************************")
MESSAGE(STATUS "")

#-------------------------------------------------------------------
# This file is part of the CMake build system for the server-client-auth library
#-------------------------------------------------------------------

# Configure Components build
cmake_minimum_required(VERSION 2.6)

# 
# Specify some user configurable options
# 

OPTION(BUILD_SERVER "Build server lib" ON)
OPTION(BUILD_CLIENT "Build Client lib" ON)
OPTION(BUILD_UTIL "Build util lib" ON)
OPTION(BUILD_SQLITE_INTERFACE "Build sqliteInterface lib" ON)
OPTION(BUILD_UNIT_TESTS "Build unit tests" OFF)

#
# Also load the CMakeLists file in sub-folders.
#

if (BUILD_SERVER)
  add_subdirectory(server)
endif ()

if (BUILD_CLIENT)
  add_subdirectory(client)
endif ()

if (BUILD_UTIL)
  add_subdirectory(util)
endif ()

if(BUILD_UNIT_TESTS)
  add_subdirectory(tests)
endif()

if(BUILD_SQLITE_INTERFACE)
  add_subdirectory(sqliteInterface)
endif()

#doxygen configuration based on the method found  @ https://mementocodex.wordpress.com/2013/01/19/how-to-generate-code-documentation-with-doxygen-and-cmake-a-slightly-improved-approach/
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
IF(BUILD_DOCUMENTATION)
 
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR
      "Doxygen is needed to build the documentation.")
  ENDIF()
 
  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in     )
  SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile              )
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
  SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}/documentation   ) # Pasted into Doxyfile.in
  SET( doxy_input           ${PROJECT_SOURCE_DIR}/                      ) # Pasted into Doxyfile.in
  SET( doxy_extra_files     ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.dox    ) # Pasted into Doxyfile.in
 
  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

  ADD_CUSTOM_COMMAND( OUTPUT ${doxy_html_index_file}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile} ALL
                      # The following should be ${doxyfile} only but it
                      # will break the dependency.
                      # The optimal solution would be creating a 
                      # custom_command for ${doxyfile} generation
                      # but I still have to figure out how...
                      MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                      DEPENDS project_targets ${doxy_extra_files}
                      COMMENT "Generating HTML documentation")
 
  ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )
 
  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc )
ENDIF()

IF(UNIX AND NOT APPLE)
add_definitions(-DCMAKE_CXX_COMPILER=gcc)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
ENDIF()