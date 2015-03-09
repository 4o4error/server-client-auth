SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../CMake/;${PROJECT_SOURCE_DIR}/CMake/;${CMAKE_ROOT}/Modules;${CMAKE_MODULE_PATH}")

MESSAGE("#Building Server#")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)



find_package(OpenSSL REQUIRED)
SET(ASIO_INCLUDE_DIR CACHE "ASIO_INCLUDE_DIR?" FORCE)
link_directories(${PROJECT_SOURCE_DIR}/build/)
include_directories(${PROJECT_SOURCE_DIR})
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories(${OPENSSL_INCLUDE_DIR} ${ASIO_INCLUDE_DIR})
SET ( SOURCE_FILES_SERVER
"${CMAKE_CURRENT_SOURCE_DIR}/server.cpp"
)


SET (LINK_LIBS_SERVER
  ${OPENSSL_LIBRARIES}
)

message(${LINK_LIBS_SERVER})
IF(WIN32)
  SET(LINK_LIBS_SERVER
    ws2_32.lib
    winmm.lib
    config.lib
    sqliteInterface.lib
    ${LINK_LIBS_SERVER}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS_SERVER
    config
    sqliteInterface
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
    ${LINK_LIBS_SERVER}
    )
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fpermissive  -o test_server -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -fpermissive -o test_server -pthread")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
ENDIF()
add_definitions(-DASIO_STANDALONE -D_WIN32_WINNT=0x0501 )
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

add_executable (test_server ${SOURCE_FILES_SERVER})
target_link_libraries(test_server  ${LINK_LIBS_SERVER})