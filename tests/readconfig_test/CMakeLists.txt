SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../CMake/;${PROJECT_SOURCE_DIR}/CMake/;${CMAKE_ROOT}/Modules;${CMAKE_MODULE_PATH}")

MESSAGE("#Building Unit Tests for Configuration Reader#")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

FIND_PACKAGE(GTestLocal)

link_directories(${PROJECT_SOURCE_DIR}/build/)
include_directories(${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/include/
                    ${GTest_INCLUDE_DIR})


SET ( SOURCE_FILES_CONFIG_READER
"test_configuration_reader.cpp")

SET (LINK_LIBS_CONFIG
  
)

SET (LINK_LIBS_CONFIG
  ${GTest_LIBRARY}
)

message(${LINK_LIBS_CONFIG})
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

add_executable (test_configuration_reader test_configuration_reader.cpp)
target_link_libraries(test_configuration_reader config ${LINK_LIBS_CONFIG})