MESSAGE(STATUS "")
MESSAGE(STATUS "### Building server ###")
MESSAGE(STATUS "************************************************")
MESSAGE(STATUS "")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)


SET (SOURCE_FILES_SERVER
 "${CMAKE_CURRENT_SOURCE_DIR}/DbConnection.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/DbConnection.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/Connection.hpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/Connection.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/server.cpp")
 
 
SOURCE_GROUP("server" FILES
  ${SOURCE_FILES_SERVER}
)
 
find_package(OpenSSL REQUIRED)
include_directories(${ASIO_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/dependencies)
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/build)
# ---------------- Link Libraries section --------------------
# Set the link options: the libraries to link against. Store them in a temporary variable
# as these are the common, cross-system link libraries.

# Append OS system specific libraries.
IF(WIN32)
  SET(LINK_LIBS_CONFIG
    ws2_32.lib
    winmm.lib
   ${OPENSSL_LIBRARIES}
    config.lib
    ${LINK_LIBS_CONFIG}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS_CONFIG
    config
    ${OPENSSL_LIBRARIES}
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
    ${LINK_LIBS_CONFIG}
    )
ENDIF()
ADD_DEFINITIONS(-DASIO_STANDALONE)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

add_executable (server ${SOURCE_FILES_SERVER})
target_link_libraries(server  ${LINK_LIBS_CONFIG})