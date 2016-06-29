cmake_minimum_required(VERSION 3.0)

project(cpp-restify)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
if (WIN32)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
endif()

# Options

option(CPPRESTIFY_WITH_CURL "When enabled and CURL is found, restify::Client is available." OFF)
option(CPPRESTIFY_SHARED "When enabled build a cpp-restify as shared library." ON)


# Vendor Libraries

add_subdirectory(vendor/mongoose)
add_subdirectory(vendor/jsoncpp)

if (CPPRESTIFY_WITH_CURL)
    find_package(CURL)
endif()

# Library

set(LIB_INCLUDE_DIRS 
	inc
	vendor/mongoose/
    vendor/jsoncpp/
)

set(LIB_SOURCES
    inc/restify/forward.h
    inc/restify/interface.h
    inc/restify/non_copyable.h
    inc/restify/request.h
    inc/restify/response.h
	inc/restify/server.h
    inc/restify/router.h
    inc/restify/helpers.h
    inc/restify/codes.h
    inc/restify/error.h
    inc/restify/http_renderer.h
    inc/restify/handler.h
	src/server.cpp
    src/request.cpp
    src/response.cpp
    src/router.cpp
    src/helpers.cpp
    src/error.cpp
    src/http_renderer.cpp
    src/handler.cpp
)

set(LIB_LINK_TARGETS mongoose jsoncpp)

if(CPPRESTIFY_WITH_CURL)
    list(APPEND LIB_SOURCES 
        inc/restify/client.h
        src/client.cpp
    )     
    list(APPEND LIB_LINK_TARGETS ${CURL_LIBRARIES})
    list(APPEND LIB_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
endif()

include_directories(${LIB_INCLUDE_DIRS})
if(CPPRESTIFY_SHARED)
    add_library(cpp-restify SHARED ${LIB_SOURCES})
else()
    add_library(cpp-restify ${LIB_SOURCES})
endif()
target_link_libraries(cpp-restify ${LIB_LINK_TARGETS})

# Tests

set(TEST_SOURCES
    tests/catch.hpp
    tests/server_fixture.h
    tests/test_request.cpp
    tests/test_response.cpp
    tests/test_router.cpp
    tests/test_helpers.cpp
)

set(TEST_LINK_TARGETS 
    cpp-restify 
    jsoncpp
)

if(CPPRESTIFY_WITH_CURL)
    list(APPEND TEST_SOURCES tests/test_server.cpp)
    list(APPEND TEST_LINK_TARGETS ${CURL_LIBRARIES})
    include_directories(${CURL_INCLUDE_DIRS})
endif()

add_executable(cpp-restify-tests ${TEST_SOURCES})
target_link_libraries(cpp-restify-tests ${TEST_LINK_TARGETS})