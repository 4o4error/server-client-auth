#CLient build system

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

MESSAGE("Building client library")

# Set the include paths for the CMake scripts that will be used to find other libs.

SET (SOURCE_FILES_CLIENT
${PROJECT_SOURCE_DIR}/include/client/SslClientAuthDll.h
${CMAKE_CURRENT_SOURCE_DIR}/SslAuthClientDll.cpp
)

#only for Visual Studio
SOURCE_GROUP("client" FILES
${SOURCE_FILE_CLIENT}
)

find_package(OpenSSL REQUIRED)

INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ASIO_INCLUDE_DIR})
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
# SHARED for DLL
add_library(client SHARED ${SOURCE_FILES_CLIENT})

# ---------------- Link Libraries section --------------------
# Set the link options: the libraries to link against. Store them in a temporary variable
# as these are the common, cross-system link libraries.

# Append OS system specific libraries.
IF(WIN32)
  SET(LINK_LIBS_CONFIG
    ws2_32.lib
    winmm.lib
    ${OPENSSL_LIBRARIES}
    ${LINK_LIBS_CONFIG}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS_CONFIG
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
	${OPENSSL_LIBRARIES}
    ${LINK_LIBS_CONFIG}
    )
ENDIF()
#-D ca sa ia ca si definitie
ADD_DEFINITIONS(-DSCA_EXP)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
ADD_DEFINITIONS(-DASIO_STANDALONE)

target_link_libraries(client ${LINK_LIBS_CONFIG})