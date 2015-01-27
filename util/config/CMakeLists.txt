MESSAGE(STATUS "")
MESSAGE(STATUS "### Building util.config ###")
MESSAGE(STATUS "************************************************")
MESSAGE(STATUS "")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

SET (SOURCE_FILES_CONFIG
 "${CMAKE_CURRENT_SOURCE_DIR}/config_reader_build_opts.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/IConfigClass.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigClass.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigClass.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigException.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigException.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigReader.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigReader.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/ConfigFactory.h")
 
 
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
ENDIF()
ADD_DEFINITIONS(-DCONFIG_READER_EXP)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)



target_link_libraries(config ${LINK_LIBS_CONFIG})