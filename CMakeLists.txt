cmake_minimum_required(VERSION 3.0)

project(cpp-restify)

# Options

option(CPPRESTIFY_WITH_CURL "When enabled and CURL is found, restify::Client is available." OFF)
option(CPPRESTIFY_SHARED "When enabled build a cpp-restify as shared library." ON)
option(CPPRESTIFY_CXX_STANDARD_14 "When enabled uses experimental features from C++14." ON)
# Not an option right now, but will flex with more backends.
set(CPPRESTIFY_WITH_MONGOOSE ON)

# Compiler settings

if (CPPRESTIFY_CXX_STANDARD_14)
    set(CMAKE_CXX_STANDARD 14)
else()
    set(CMAKE_CXX_STANDARD 11)
    set(CPPRESTIFY_WITH_WJAKOB_FILEYSTEM ON)
endif()

if (WIN32)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
endif()

# Vendor Libraries

add_subdirectory(vendor/jsoncpp)

if (CPPRESTIFY_WITH_CURL)
    find_package(CURL)
endif()

# Library

set(LIB_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
	inc
    vendor/jsoncpp/
)

set(LIB_HEADERS
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
    inc/restify/handler.h
    inc/restify/request_reader.h
    inc/restify/response_writer.h
    inc/restify/connection.h
    inc/restify/route.h
    inc/restify/backend.h
    inc/restify/mime_types.h
    inc/restify/filesystem/filesystem.h
)

set(LIB_SOURCES
	src/server.cpp
    src/request.cpp
    src/response.cpp
    src/router.cpp
    src/helpers.cpp
    src/error.cpp
    src/handler.cpp
    src/request_reader.cpp
    src/response_writer.cpp
    src/connection.cpp
    src/route.cpp
    src/backend.cpp
    src/mime_types.cpp
)

set(LIB_LINK_TARGETS jsoncpp)

if(CPPRESTIFY_WITH_CURL)
    list(APPEND LIB_HEADERS
        inc/restify/client.h
    )
    list(APPEND LIB_SOURCES
        src/client.cpp
    )
    list(APPEND LIB_LINK_TARGETS ${CURL_LIBRARIES})
    list(APPEND LIB_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
endif()

if (CPPRESTIFY_WITH_WJAKOB_FILEYSTEM)
    include_directories(vendor/filesystem-wjakob)
    list(APPEND LIB_SOURCES
        src/filesystem/wjakob/filesystem.cpp
    )
else()
    list(APPEND LIB_SOURCES
        src/filesystem/cpp14/filesystem.cpp
    )
endif()

if(CPPRESTIFY_WITH_MONGOOSE)
	include_directories(vendor/mongoose/)
    list(APPEND LIB_HEADERS
        inc/restify/mongoose/mongoose_backend.h
        inc/restify/mongoose/mongoose_connection.h
        inc/restify/mongoose/mongoose_request_reader.h
    )
    list(APPEND LIB_SOURCES
        src/mongoose/mongoose_backend.cpp
        src/mongoose/mongoose_connection.cpp
        src/mongoose/mongoose_request_reader.cpp
				vendor/mongoose/mongoose.c
    )
endif()

set(CPPRESTIFY_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(inc/restify/config.h.in restify_build_config.h @ONLY)

include_directories(${LIB_INCLUDE_DIRS})
if(CPPRESTIFY_SHARED)
    add_library(cpp-restify SHARED ${LIB_SOURCES} ${LIB_HEADERS})
else()
    add_library(cpp-restify ${LIB_SOURCES} ${LIB_HEADERS})
endif()
target_link_libraries(cpp-restify ${LIB_LINK_TARGETS})
set_target_properties(cpp-restify PROPERTIES DEBUG_POSTFIX "d")

install(FILES ${LIB_HEADERS} DESTINATION inc/restify)
install(TARGETS cpp-restify EXPORT cpp-restify-targets DESTINATION lib/restify)
install(EXPORT cpp-restify-targets DESTINATION lib/restify)
install(FILES CPPRestify-config.cmake DESTINATION lib/restify)

# Tests

set(TEST_SOURCES
    tests/catch.hpp
    tests/server_fixture.h
    tests/test_request.cpp
    tests/test_response.cpp
    tests/test_router.cpp
    tests/test_route.cpp
    tests/test_helpers.cpp
    tests/test_mime_types.cpp
    tests/test_filesystem.cpp
)

set(TEST_LINK_TARGETS
    cpp-restify
    jsoncpp
)

if(CPPRESTIFY_WITH_CURL)
    list(APPEND TEST_SOURCES tests/test_server.cpp)
    list(APPEND TEST_LINK_TARGETS ${CURL_LIBRARIES})
    include_directories(${CURL_INCLUDE_DIRS})
else()
    list(APPEND TEST_SOURCES tests/warn_no_curl.cpp)
    message(WARNING "cpp-restify is build without CURL. Many unit tests won't be included.")
endif()

add_executable(cpp-restify-tests ${TEST_SOURCES})
target_link_libraries(cpp-restify-tests ${TEST_LINK_TARGETS})
