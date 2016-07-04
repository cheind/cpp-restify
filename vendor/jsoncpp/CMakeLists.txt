cmake_minimum_required(VERSION 3.0)

project(jsoncpp)

# Library

add_library(jsoncpp
	json/json-forwards.h
	json/json.h
	jsoncpp.cpp
)

 install(FILES json/json-forwards.h json/json.h DESTINATION inc/json)
 install(TARGETS jsoncpp EXPORT jsoncpp-targets DESTINATION lib/jsoncpp)
 install(EXPORT jsoncpp-targets DESTINATION lib/jsoncpp)
 install(FILES JsonCPP-config.cmake DESTINATION lib/jsoncpp)


