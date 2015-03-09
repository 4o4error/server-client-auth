set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

OPTION(TEST_CONFIG_READER "Build unit tests for configuration reader" OFF)
OPTION(TEST_ENDURANCE_SCA "Build endurance test for sca" OFF)
if(TEST_CONFIG_READER)
add_subdirectory(readconfig_test)
endif()
IF(TEST_ENDURANCE_SCA)
add_subdirectory(endurance_test_sca)
endif()

