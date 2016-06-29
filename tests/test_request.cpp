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
    using restify::Request;

    restify::Request r;
    restify::json(r)
        (Request::Keys::method, "POST")
        (Request::Keys::path, "/users/123")
        (Request::Keys::params, "a", 3)
        (Request::Keys::params, "b", true)
        (Request::Keys::headers, "Content-Type", "text/plain")
        (Request::Keys::headers, "Content-Length", 20);

    REQUIRE(r.getMethod() == "POST");
    REQUIRE(r.getPath() == "/users/123");
    REQUIRE(r.getParams()["a"].asInt() == 3);
    REQUIRE(r.getParams()["b"].asBool());
    REQUIRE(r.getParam("a").asInt() == 3);
    REQUIRE(r.getParam("b").asBool());

    REQUIRE(r.getHeaders()["Content-Type"].asString() == "text/plain");
    REQUIRE(r.getHeader("NOT-HERE", "ABC").asString() == "ABC");
}

TEST_CASE("request-body") {
    using restify::Request;

    restify::Request r;
    restify::json(r)
        (Request::Keys::method, "POST")
        (Request::Keys::path, "/users/123")
        (Request::Keys::params, "a", 3)
        (Request::Keys::params, "b", true)
        (Request::Keys::headers, "Content-Type", "text/plain")
        (Request::Keys::headers, "Content-Length", 20)
        (Request::Keys::body, restify::json()("value", "hello world!"));

    REQUIRE(r.getMethod() == "POST");
    REQUIRE(r.getPath() == "/users/123");
    REQUIRE(r.getParams()["a"].asInt() == 3);
    REQUIRE(r.getParams()["b"].asBool());
    REQUIRE(r.getParam("a").asInt() == 3);
    REQUIRE(r.getParam("b").asBool());
    REQUIRE(r.getHeaders()["Content-Type"].asString() == "text/plain");
    REQUIRE(r.getHeader("NOT-HERE", "ABC").asString() == "ABC");
    REQUIRE(r.getBody().isObject());
    REQUIRE(r.getBody()["value"] == "hello world!");
}

TEST_CASE("request-query-string")
{
    /*
    using restify::Request;

    restify::Request r;
    restify::json(r.toJson())
        (Request::Keys::query, "a= 3.2&b=hugo&c= true");
    
    using restify::json_cast;
    
    REQUIRE(json_cast<float>(r.getParam("a")) == 3.2f);
    REQUIRE(json_cast<std::string>(r.getParam("b")) == "hugo");
    REQUIRE(json_cast<bool>(r.getParam("c")) == true);
    */
}