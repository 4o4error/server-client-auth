MESSAGE(STATUS "")
MESSAGE(STATUS "### Building util.config ###")
MESSAGE(STATUS "************************************************")
MESSAGE(STATUS "")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

SET (SOURCE_FILES_CONFIG
 "${PROJECT_SOURCE_DIR}/include/util/config/IConfigClass.h"
 "${PROJECT_SOURCE_DIR}/include/util/config/ConfigClass.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigClass.cpp"
 "${PROJECT_SOURCE_DIR}/include/util/config/ConfigException.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigException.cpp"
 "${PROJECT_SOURCE_DIR}/include/util/config/ConfigReader.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigReader.cpp"
 "${PROJECT_SOURCE_DIR}/include/util/config/ConfigFactory.h")
 
 include_directories("${PROJECT_SOURCE_DIR}/include")
 
SOURCE_GROUP("config" FILES
  ${SOURCE_FILES_CONFIG}
)
 
add_library(config SHARED ${SOURCE_FILES_CONFIG})

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
    )
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
ADD_DEFINITIONS(-DSCA_EXP)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)





target_link_libraries(config ${LINK_LIBS_CONFIG})