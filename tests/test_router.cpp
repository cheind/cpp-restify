/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/router.h>
#include <restify/route.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/helpers.h>
#include <restify/error.h>
#include <json/json.h>

TEST_CASE("router")
{
    restify::Router router;

    using restify::ParameterRoute;

    // Add a couple of routes
    Json::Value cfg;
    cfg["path"] = "/users";
    cfg["methods"].append("GET");
    cfg["methods"].append("POST");

    router.createRoute<ParameterRoute>(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    cfg.clear();
    cfg["path"] = "/users/:id";
    cfg["methods"] = "GET";
    router.createRoute<ParameterRoute>(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    cfg.clear();
    cfg["path"] = "/users/:id/card/:number";
    cfg["methods"] = "GET";
    router.createRoute<ParameterRoute>(cfg, [](const restify::Request &req, restify::Response &rep) {
        return true;
    });

    using restify::Request;

    {
        // Unknown route
        restify::Request req;        
        restify::json(req)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/nothere");

        restify::Response rep;

        REQUIRE(!router.route(req, rep));
    }

    {
        // Known route, wrong method
        restify::Request req;
        restify::json(req)
            (Request::Keys::method, "PUT")
            (Request::Keys::path, "/users");

        restify::Response rep;

        REQUIRE(!router.route(req, rep));
    }

    {
        // Known route and method
        restify::Request req;
        restify::json(req)
            (Request::Keys::method, "POST")
            (Request::Keys::path, "/users");

        restify::Response rep;

        REQUIRE(router.route(req, rep));
    }

    {
        // Known route
        restify::Request req;
        restify::json(req)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/users/123");

        restify::Response rep;

        REQUIRE(router.route(req, rep));
        REQUIRE(req.getParam("id").isString());        
        REQUIRE(req.getParam("id") == "123");
    }

    {
        // Known route
        restify::Request req;
        restify::json(req)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/users/123/card/456");

        restify::Response rep;
        
        using restify::json_cast;

        REQUIRE(router.route(req, rep));
        REQUIRE(json_cast<int>(req.getParam("id")) == 123);
        REQUIRE(json_cast<int>(req.getParam("number")) == 456);
    }

    {
        // Known route with types, wrong type passed
        restify::Request req;
        restify::json(req)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/users/123/card/456dasd");

        restify::Response rep;
        
        using restify::json_cast;

        REQUIRE(router.route(req, rep));
        REQUIRE_THROWS_AS(json_cast<int>(req.getParam("number")), restify::Error);
    }

}

TEST_CASE("router-with-default") {
    restify::Router router;
    using restify::ParameterRoute;
    using restify::AnyRoute;

    int invokeDefaultCount = 0;
    int invokeUsersCount = 0;

    router.createRoute<ParameterRoute>(
        restify::json()
            ("path", "/users")
            ("methods", "GET"),
        [&invokeUsersCount](const restify::Request &req, restify::Response &rep) {
            ++invokeUsersCount;
            return true;
    });

    router.createRoute<AnyRoute>([&invokeDefaultCount](const restify::Request &req, restify::Response &rep) {
        ++invokeDefaultCount;
        return true;
    });

    restify::Response rep;

    using restify::Request;

    {
        Request r;
        restify::json(r)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/users");
        REQUIRE(router.route(r, rep));
    }
    {
        Request r;
        restify::json(r)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/not-here");
        REQUIRE(router.route(r, rep));
    }
    {
        Request r;
        restify::json(r)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/users/123");
        REQUIRE(router.route(r, rep));
    }
    {
        Request r;
        restify::json(r)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "/");
        REQUIRE(router.route(r, rep));
    }
    {
        Request r;
        restify::json(r)
            (Request::Keys::method, "GET")
            (Request::Keys::path, "//");
        REQUIRE(router.route(r, rep));
    }

    REQUIRE(invokeUsersCount == 1);
    REQUIRE(invokeDefaultCount == 4);
}