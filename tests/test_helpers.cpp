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

TEST_CASE("helpers-json-builder") {
    
    restify::JsonBuilder jbp;

    Json::Value v = jbp
        .set("body.message", "Hello World")
        .set("body.count", 3)
        .set("body.elements.[0]", "a")
        .set("body.elements.[1]", "b")
        .set("body.elements.[2]", "c")
        .set("body.elements", "[3]", "d")
        .toJson();

   
    REQUIRE(v["body"].isObject());
    REQUIRE(v["body"]["message"].asString() == "Hello World");
    REQUIRE(v["body"]["elements"].isArray());
    REQUIRE(v["body"]["elements"].size() == 4);
    REQUIRE(v["body"]["elements"][0] == "a");
    REQUIRE(v["body"]["elements"][1] == "b");
    REQUIRE(v["body"]["elements"][2] == "c");
    REQUIRE(v["body"]["elements"][3] == "d");
    REQUIRE(v["body"]["count"].asInt() == 3);
}

TEST_CASE("helpers-json-builder-alt") {

    Json::Value v = restify::json()
        ("body.message", "Hello World")
        ("body.count", 3)
        ("body.elements.[0]", "a")
        ("body.elements.[1]", "b")
        ("body.elements.[2]", "c");


    REQUIRE(v["body"].isObject());
    REQUIRE(v["body"]["message"].asString() == "Hello World");
    REQUIRE(v["body"]["elements"].isArray());
    REQUIRE(v["body"]["elements"].size() == 3);
    REQUIRE(v["body"]["elements"][0] == "a");
    REQUIRE(v["body"]["elements"][1] == "b");
    REQUIRE(v["body"]["elements"][2] == "c");
    REQUIRE(v["body"]["count"].asInt() == 3);

}

TEST_CASE("helpers-json-builder-adapt") {

    Json::Value val;
    val["body"]["message"] = "Hello World";

    Json::Value v = restify::json(val)
        ("body.count", 3)
        ("body.elements.[0]", "a")
        ("body.elements.[1]", "b")
        ("body.elements.[2]", "c");


    REQUIRE(v["body"].isObject());
    REQUIRE(v["body"]["message"].asString() == "Hello World");
    REQUIRE(v["body"]["elements"].isArray());
    REQUIRE(v["body"]["elements"].size() == 3);
    REQUIRE(v["body"]["elements"][0] == "a");
    REQUIRE(v["body"]["elements"][1] == "b");
    REQUIRE(v["body"]["elements"][2] == "c");
    REQUIRE(v["body"]["count"].asInt() == 3);

}

#include <restify/server.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/error.h>
#include <restify/handler.h>

/*
TEST_CASE("server") {
    
    Json::Value v(Json::nullValue);
    
    restify::Server server(v);
    
    Json::Value cfg;
    cfg["path"] = "/users/:id";
    cfg["methods"].append("GET");
    cfg["methods"].append("POST");
    server.route(cfg, [](const restify::Request &req, restify::Response &rep) {
        
        std::cout << req.toJson().toStyledString() << std::endl;

        CPPRESTIFY_FAIL(restify::StatusCode::NotFound, "Not found");

        return true;
    });

    server.otherwise(restify::RedirectRequestHandler(restify::json()("url", "/users/232")));
    
    server.start();
    
    std::cin.get();
    
    server.stop();
    
    
}

*/