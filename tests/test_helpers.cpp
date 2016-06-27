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

TEST_CASE("helpers-json-cast")
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

TEST_CASE("helpers-split-string") {
    
    
    REQUIRE(restify::splitString(" a=2& b = 3.1&c = 1;2;3 ", '&', true) ==
            (std::vector<std::string> {"a=2", "b = 3.1", "c = 1;2;3"}));
    
    
}

#include <restify/server.h>
#include <restify/request.h>
#include <restify/response.h>

TEST_CASE("server") {
    
    Json::Value v(Json::nullValue);
    
    restify::Server server(v);
    
    Json::Value cfg;
    cfg["path"] = "/users/:id";
    cfg["methods"].append("GET");
    cfg["methods"].append("POST");
    server.route(cfg, [](const restify::Request &req, restify::Response &rep) {
        
        std::cout << req.toJson().toStyledString() << std::endl;
        return true;
    });
    
    server.start();
    
    std::cin.get();
    
    server.stop();
    
    
}