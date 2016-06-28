/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/response.h>
#include <json/json.h>

TEST_CASE("response")
{
    restify::Response r;
    r
        .setCode(404)
        .beginBody()
        .set("message", "Not found.")
        .end()
        .setHeader("Location", "xxx");

    Json::Value c = r.toJson();

    REQUIRE(c["headers"]["Location"].asString() == "xxx");
    REQUIRE(c["body"]["message"].asString() == "Not found.");
    REQUIRE(c["statusCode"].asInt() == 404);
}
