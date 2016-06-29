cmake_minimum_required(VERSION 3.0)

project(cpp-restify)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
endif()

# Vendor Libraries

add_subdirectory(vendor/mongoose)
add_subdirectory(vendor/jsoncpp)

# Library

include_directories(
	inc
	vendor/mongoose/
    vendor/jsoncpp/
)

add_library(cpp-restify SHARED
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

target_link_libraries(cpp-restify mongoose jsoncpp)

# Tests

set(TEST_SOURCES
    tests/catch.hpp
    tests/server_fixture.h
    tests/test_request.cpp
    tests/test_response.cpp
    tests/test_router.cpp
    tests/test_helpers.cpp
)

set(TEST_LINK_TARGETS cpp-restify jsoncpp)

find_package(CURL QUIET)
if(CURL_FOUND)
    list(APPEND TEST_SOURCES tests/test_server.cpp tests/curl_client.h)
    list(APPEND TEST_LINK_TARGETS ${CURL_LIBRARIES})
    include_directories(${CURL_INCLUDE_DIRS})
endif()

add_executable(cpp-restify-tests ${TEST_SOURCES})
target_link_libraries(cpp-restify-tests ${TEST_LINK_TARGETS})



