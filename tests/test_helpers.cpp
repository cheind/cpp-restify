/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/helpers.h>
#include <iostream>

TEST_CASE("json-cast")
{
    using namespace restify;
    
    Json::Value strInt = "123";
    REQUIRE(json_cast<std::string>(strInt) == "123");
    REQUIRE(json_cast<int>(strInt) == 123);
    REQUIRE(json_cast<float>(strInt) == 123.f);
    
    Json::Value strBool0 = "true";
    REQUIRE(json_cast<bool>(strBool0) == true);
    
    Json::Value arr;
    arr.append("1");
    arr.append("2");
    REQUIRE(json_cast<std::string>(arr) == "[ \"1\", \"2\" ]\n");
    
    
    Json::Value strNotInt = "23kdnd";
    REQUIRE_THROWS_AS(json_cast<int>(strNotInt), Error);
    REQUIRE_THROWS_AS(json_cast<float>(strNotInt), Error);
    REQUIRE_THROWS_AS(json_cast<bool>(strNotInt), Error);



}