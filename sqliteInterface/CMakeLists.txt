#Sqlite build system

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

MESSAGE("Building sqlite library")

# Set the include paths for the CMake scripts that will be used to find other libs.

SET (SOURCE_FILES_SQLITE_INTERFACE
${CMAKE_CURRENT_SOURCE_DIR}/Singleton.h
${CMAKE_CURRENT_SOURCE_DIR}/SqlDll.h
${CMAKE_CURRENT_SOURCE_DIR}/sqlOp.h
${CMAKE_CURRENT_SOURCE_DIR}/sqlite3ext.h
${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h
${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c
${CMAKE_CURRENT_SOURCE_DIR}/shell.c
${CMAKE_CURRENT_SOURCE_DIR}/sqlOp.cpp

)

#only for Visual Studio
SOURCE_GROUP("sqliteInterface" FILES
${SOURCE_FILES_SQLITE_INTERFACE}
)
include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(sqliteInterface SHARED ${SOURCE_FILES_SQLITE_INTERFACE})

# ---------------- Link Libraries section --------------------
# Set the link options: the libraries to link against. Store them in a temporary variable
# as these are the common, cross-system link libraries.

# Append OS system specific libraries.
IF(WIN32)
  SET(LINK_LIBS_CONFIG
    ws2_32.lib
    winmm.lib
    ${LINK_LIBS_CONFIG}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS_CONFIG
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
    ${LINK_LIBS_CONFIG}
    
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
    )
ENDIF()
#-D ca sa ia ca si definitie
ADD_DEFINITIONS(-DSCA_EXP)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)




target_link_libraries(sqliteInterface ${LINK_LIBS_CONFIG})