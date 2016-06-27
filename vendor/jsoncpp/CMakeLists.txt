cmake_minimum_required(VERSION 3.0)

project(jsoncpp)

# Library

add_library(jsoncpp
	json/json-forwards.h
	json/json.h
	jsoncpp.cpp
)


