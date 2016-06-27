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
    cfg["params"]["id"]["type"] = "int";
    cfg["params"]["number"]["type"] = "int";
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

        REQUIRE(router.dispatch(req, rep));
        REQUIRE(req.param("id").isInt());
        REQUIRE(req.param("number").isInt());
        REQUIRE(req.param("id") == 123);
        REQUIRE(req.param("number") == 456);
    }

    {
        // Known route with types, wrong type passed
        restify::Request req;
        req
            .method("GET")
            .path("/users/123/card/456dasd");

        restify::Response rep;

        REQUIRE(!router.dispatch(req, rep));

        restify::Request req2;
        req2
            .method("GET")
            .path("/users/123/card/dasd456dasd");

        REQUIRE(!router.dispatch(req2, rep));
    }

    

    

}
