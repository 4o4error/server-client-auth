# - Locate gtest, the google test framework.
# This module defines
#  GTest_LIBRARY
#  GTest_FOUND, if false, do not try to link against google test
#  GTest_INCLUDE_DIR, where to find the headers
#
# Created by Augustin Preda. Adapted from the [FindALUT.cmake] module (by Eric Wing).
# 

#INCLUDE(FindPackageHandleStandardArgs) 
INCLUDE(${CMAKE_SOURCE_DIR}/CMake/HandleLibraryTypes.cmake)

# Set the paths to search for as possible locations of the library.
SET (GTest_SEARCH_PATH ${PROJECT_SOURCE_DIR}/dependencies/gtest-1.7.0 
        ${PROJECT_SOURCE_DIR}/../dependencies/gtest-1.6.0
        ${PROJECT_SOURCE_DIR}/../dependencies/gtest
        $ENV{GTEST_ROOT})
FIND_PATH(GTest_INCLUDE_DIR gtest/gtest.h
  PATHS  ${GTest_SEARCH_PATH}
  PATH_SUFFIXES include 
                gtest-1.7.0/include
                gtest-1.6.0/include 
)

# Look for gtest.lib in both [Debug] and [Release] configurations
FIND_LIBRARY(GTest_LIBRARY_OPTIMIZED
  NAMES gtest
  PATHS ${GTest_SEARCH_PATH}
  PATH_SUFFIXES lib libs bin build/lib build/Release
)
FIND_LIBRARY(GTest_LIBRARY_DEBUG
  NAMES gtest
  PATHS ${GTest_SEARCH_PATH}
  PATH_SUFFIXES lib libs bin build/lib build/Debug
)

# Look for gtest.lib in both [Debug] and [Release] configurations
FIND_LIBRARY(GTestMain_LIBRARY_OPTIMIZED
  NAMES gtest_main
  PATHS ${GTest_SEARCH_PATH}
  PATH_SUFFIXES lib libs bin build/lib build/Release
)
FIND_LIBRARY(GTestMain_LIBRARY_DEBUG
  NAMES gtest_main
  PATHS ${GTest_SEARCH_PATH}
  PATH_SUFFIXES lib libs bin build/lib build/Debug
)

# In case the DLL is also needed, locate it.
IF (NOT GTest_STATIC)
  FIND_FILE (GTest_BINARY_REL 
      NAMES "gtest.dll" 
      HINTS ${GTest_SEARCH_PATH}
      PATH_SUFFIXES "" lib libs bin build/lib build/Release Release relwithdebinfo minsizerel
  )
        
  FIND_FILE (GTest_BINARY_DBG 
      NAMES "gtest.dll" "gtestd.dll" 
      HINTS ${GTest_SEARCH_PATH}
      PATH_SUFFIXES "" lib libs bin build/lib build/Debug debug
  )
ENDIF ()

# Handle the REQUIRED argument and set GTest_FOUND
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTest DEFAULT_MSG
#    GTest_LIBRARY_OPTIMIZED
  #  GTest_INCLUDE_DIR
#)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(GTest)

MARK_AS_ADVANCED(
    GTest_LIBRARY_OPTIMIZED
    GTest_LIBRARY_DEBUG
)
