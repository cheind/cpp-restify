/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/server.h>
#include <restify/request.h>
#include <restify/response.h>
#include <json/json.h>
#include <iostream>

TEST_CASE("server")
{
    Json::Value v(Json::nullValue);
    restify::Server server(v);
    server.start();
    std::cin.get();
    server.stop();
}
