SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../CMake/;${PROJECT_SOURCE_DIR}/CMake/;${CMAKE_ROOT}/Modules;${CMAKE_MODULE_PATH}")

MESSAGE("#Building Endurance Test#")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

FIND_PACKAGE(GTestLocal REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)

link_directories(${PROJECT_SOURCE_DIR}/build/)
include_directories(${PROJECT_SOURCE_DIR}/
                    ${GTest_INCLUDE_DIR}
                    ${OPENSSL_INCLUDE_DIR}
                    ${ASIO_INCLUDE_DIR}
                              )


SET ( SOURCE_FILES_CONFIG_READER
"endurance_test_sca.cpp")

SET (LINK_LIBS_CONFIG
  
)

SET (LINK_LIBS_CONFIG
  ${GTest_LIBRARY}
  ${OPENSSL_LIBRARIES}
)

message(${LINK_LIBS_CONFIG})
IF(WIN32)
  SET(LINK_LIBS_CONFIG
    ws2_32.lib
    winmm.lib
    client.lib
    ${LINK_LIBS_CONFIG}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS_CONFIG
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
    ${LINK_LIBS_CONFIG}
    )
ENDIF()
#unicode 
add_definitions(-DUNICODE)
#asio only verion of lib 
add_definitions(-DASIO_STANDALONE)
add_executable (endurance_test_sca endurance_test_sca.cpp)
target_link_libraries(endurance_test_sca  ${LINK_LIBS_CONFIG})