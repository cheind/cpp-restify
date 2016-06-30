/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/route.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/helpers.h>
#include <json/json.h>

TEST_CASE("route-param")
{
    using namespace restify;

    Json::Value extracted;

    // no params
    REQUIRE(ParameterRoute(
        json()
        ("path", "/users")
        ("methods.[0]", "GET")
        ("methods.[1]", "POST"),
        RequestHandler()
    ).match(
        json()
        ("path", "/users")
        ("method", "GET").toJson(),
        extracted
    ));
    REQUIRE(extracted.isObject());

    // invalid path
    extracted = Json::Value(Json::nullValue);
    REQUIRE(!ParameterRoute(
        json()
        ("path", "/users")
        ("methods.[0]", "GET")
        ("methods.[1]", "POST"),
        RequestHandler()
    ).match(
        json()
        ("path", "/use")
        ("method", "GET").toJson(),
        extracted
    ));

    

    // with params
    extracted = Json::Value(Json::nullValue);
    REQUIRE(ParameterRoute(
        json()
        ("path", "/users/:id/card/:number")
        ("methods.[0]", "GET")
        ("methods.[1]", "POST"),
        RequestHandler()
    ).match(
        json()
        ("path", "/users/123/card/45663")
        ("method", "GET").toJson(),
        extracted
    ));
    REQUIRE(json_cast<int>(extracted["id"]) == 123);
    REQUIRE(json_cast<int>(extracted["number"]) == 45663);

    // trailing slashes
    extracted = Json::Value(Json::nullValue);
    REQUIRE(ParameterRoute(
        json()
        ("path", "/users/:id/card/:number")
        ("methods.[0]", "GET")
        ("methods.[1]", "POST"),
        RequestHandler()
    ).match(
        json()
        ("path", "/users/123/card/45663/")
        ("method", "GET").toJson(),
        extracted
    ));
    REQUIRE(json_cast<int>(extracted["id"]) == 123);
    REQUIRE(json_cast<int>(extracted["number"]) == 45663);


}