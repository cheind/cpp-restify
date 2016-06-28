/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/router.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/helpers.h>
#include <restify/error.h>
#include <json/json.h>

TEST_CASE("router")
{
    restify::Router router;

    // Add a couple of routes
    Json::Value cfg;
    cfg["path"] = "/users";
    cfg["methods"].append("GET");
    cfg["methods"].append("POST");

    router.addRoute(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    cfg.clear();
    cfg["path"] = "/users/:id";
    cfg["methods"] = "GET";
    router.addRoute(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    cfg.clear();
    cfg["path"] = "/users/:id/card/:number";
    cfg["methods"] = "GET";
    router.addRoute(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    {
        // Unknown route
        restify::Request req;
        req
            .method("GET")
            .path("/nothere");

        restify::Response rep;

        REQUIRE(!router.dispatch(req, rep));
    }

    {
        // Known route, wrong method
        restify::Request req;
        req
            .method("PUT")
            .path("/users");

        restify::Response rep;

        REQUIRE(!router.dispatch(req, rep));
    }

    {
        // Known route and method
        restify::Request req;
        req
            .method("POST")
            .path("/users");

        restify::Response rep;

        REQUIRE(router.dispatch(req, rep));
    }

    {
        // Known route
        restify::Request req;
        req
            .method("GET")
            .path("/users/123");

        restify::Response rep;

        REQUIRE(router.dispatch(req, rep));
        REQUIRE(req.param("id").isString());        
        REQUIRE(req.param("id") == "123");
    }

    {
        // Known route with types
        restify::Request req;
        req
            .method("GET")
            .path("/users/123/card/456");

        restify::Response rep;
        
        using restify::json_cast;

        REQUIRE(router.dispatch(req, rep));
        REQUIRE(json_cast<int>(req.param("id")) == 123);
        REQUIRE(json_cast<int>(req.param("number")) == 456);
    }

    {
        // Known route with types, wrong type passed
        restify::Request req;
        req
            .method("GET")
            .path("/users/123/card/456dasd");

        restify::Response rep;
        
        using restify::json_cast;

        REQUIRE(router.dispatch(req, rep));
        REQUIRE_THROWS_AS(json_cast<int>(req.param("number")), restify::Error);
    }

}

TEST_CASE("router-with-default") {
    restify::Router router;

    int invokeDefaultCount = 0;
    int invokeUsersCount = 0;

    Json::Value cfg;
    cfg["path"] = "/users";
    cfg["methods"].append("GET");

    router.addRoute(cfg, [&invokeUsersCount](const restify::Request &req, restify::Response &rep) {
        ++invokeUsersCount;
        return true;
    });

    router.setDefaultRoute([&invokeDefaultCount](const restify::Request &req, restify::Response &rep) {
        ++invokeDefaultCount;
        return true;
    });

    restify::Response rep;

    REQUIRE(router.dispatch(restify::Request().method("GET").path("/users"), rep));
    REQUIRE(router.dispatch(restify::Request().method("GET").path("/not-here"), rep));
    REQUIRE(router.dispatch(restify::Request().method("GET").path("/users/123"), rep));
    REQUIRE(router.dispatch(restify::Request().method("GET").path("/"), rep));
    REQUIRE(router.dispatch(restify::Request().method("GET").path("/"), rep));

    REQUIRE(invokeUsersCount == 1);
    REQUIRE(invokeDefaultCount == 4);


}