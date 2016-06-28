/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <restify/request.h>
#include <json/json.h>

#include <restify/helpers.h>

TEST_CASE("request")
{
    restify::Request r;
    r.method("POST");
    r.path("/users/123");
    
    Json::Value &params = r.params();
    params["a"] = 3;
    params["b"] = true;
    
    Json::Value &headers = r.headers();
    headers["Content-Type"] = "text/plain";
    headers["Content-Length"] = 20;

    REQUIRE(r.method() == "POST");
    REQUIRE(r.path() == "/users/123");
    REQUIRE(r.params()["a"].asInt() == 3);
    REQUIRE(r.params()["b"].asBool());

    REQUIRE(r.headers()["Content-Type"].asString() == "text/plain");

    REQUIRE(r.setHeader("NOT-HERE", "ABC") == "ABC");
}

TEST_CASE("request-qeury-string")
{
    restify::Request r;
    
    r.queryString("a= 3.2&b=hugo&c= true");
    
    using restify::json_cast;
    
    REQUIRE(json_cast<float>(r.param("a")) == 3.2f);
    REQUIRE(json_cast<std::string>(r.param("b")) == "hugo");
    REQUIRE(json_cast<bool>(r.param("c")) == true);
}