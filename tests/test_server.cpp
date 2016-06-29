/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"
#include "server_fixture.h"
#include "curl_client.h"

#include <restify/server.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/helpers.h>
#include <restify/error.h>
#include <json/json.h>

#include <future>
#include <chrono>
#include <iostream>

#define CPPRESTIFY_REQUIRE_RESPOND(future, timeout) \
    REQUIRE((future).wait_for(std::chrono::milliseconds(timeout)) == std::future_status::ready);

#define CPPRESTIFY_REQUIRE_TIMEOUT(future, timeout) \
    REQUIRE((future).wait_for(std::chrono::milliseconds(timeout)) == std::future_status::timeout);


TEST_CASE_METHOD(ServerFixture, "server-listens")
{
    _server.setConfig(
                     restify::json()
                     ("listening_ports", "127.0.0.1:8080, 127.0.0.1:8081")
    );
    _server.otherwise([](const restify::Request &req, restify::Response &rep) {
        rep.setCode(200).setBody("hello world");
        return true;
    });
    _server.start();
    
    Json::Value response = sendToServer(
        restify::json()
        ("url", "http://127.0.0.1:8080"));

    REQUIRE(response["success"] == true);
    REQUIRE(response["statusCode"] == 200 );
    REQUIRE(response["body"] == "hello world");
    REQUIRE(response["headers"]["Content-Type"] == "text/plain; charset=utf-8");

    response = sendToServer(
        restify::json()
        ("url", "http://127.0.0.1:8081"));

    REQUIRE(response["success"] == true);
    REQUIRE(response["statusCode"] == 200);
    REQUIRE(response["body"] == "hello world");
    REQUIRE(response["headers"]["Content-Type"] == "text/plain; charset=utf-8");
}

TEST_CASE_METHOD(ServerFixture, "server-post-json") {
    
    _server.setConfig(
        restify::json()
        ("listening_ports", "127.0.0.1:8080")
    );
    _server.route(
        restify::json()("path", "/echo")("methods", "POST"),
        [](const restify::Request &req, restify::Response &rep) {
      
        const Json::Value &body = req.getBody();

        if (!body.isObject()) {
            throw restify::Error(restify::StatusCode::BadRequest, "Expected Json");
        }

        rep.setCode(200).setBody(restify::json()("echo", body));
        
        return true;
    });
    _server.start();

    // Note, when sending json in body, Content-Type application/json is automatically set unless it's explicitly overriden using 'headers' field.
    Json::Value response = sendToServer(
        restify::json()
        ("url", "http://127.0.0.1:8080/echo")
        ("method", "POST")
        ("body.string", "hello world!")
        ("body.value", 3)
    );

    REQUIRE(response["success"] == true);
    REQUIRE(response["statusCode"] == 200);
    REQUIRE(response["body"]["echo"] == restify::json()("string", "hello world!")("value", 3));
    REQUIRE(response["headers"]["Content-Type"] == "application/json; charset=utf-8");

    // Explicitely set non json content type. That way body shouldn't be parsed as JSON in restify::Request.
    response = sendToServer(
        restify::json()
        ("url", "http://127.0.0.1:8080/echo")
        ("method", "POST")
        ("body.string", "hello world!")
        ("body.value", 3)
        ("headers.Content-Type", "text/plain")
    );
    REQUIRE(response["success"] == true);
    REQUIRE(response["statusCode"] == 400);
    REQUIRE(response["body"] == restify::json()("message", "Expected Json")("statusCode", 400));
    REQUIRE(response["headers"]["Content-Type"] == "application/json; charset=utf-8");
}
