#-------------------------------------------------------------------
# This file is part of the CMake build system for the server-client-auth library
#-------------------------------------------------------------------

# Configure Components build
cmake_minimum_required(VERSION 2.6)

# 
# Specify some user configurable options
# 

OPTION(BUILD_SERVER, ON)
OPTION(BUILD_CLIENT, ON)

#
# Also load the CMakeLists file in sub-folders.
#

if (BUILD_SERVER)
  add_subdirectory(server)
endif ()

if(BUILD_CLIENT)
  add_subdirectory(client)
endif ()