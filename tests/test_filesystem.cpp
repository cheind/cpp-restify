/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/filesystem/filesystem.h>
#include "restify_build_config.h"
#include <iostream>

TEST_CASE("filesystem")
{
    using namespace restify;
   
    REQUIRE(Path::typeOf(Path::join(CPPRESTIFY_SOURCE_PATH, "inc/")) == Path::Type::Directory);
    REQUIRE(Path::typeOf(Path::join(CPPRESTIFY_SOURCE_PATH, "inc/restify/forward.h")) == Path::Type::File);
    REQUIRE(Path::typeOf(Path::join(CPPRESTIFY_SOURCE_PATH, "inc/restify/hopefully-not-here.h")) == Path::Type::NotFound);

    REQUIRE(Path::extension("inc/restify/forward.h") == ".h");
    REQUIRE(Path::extension("inc/restify/forward.HSD") == ".HSD");
    REQUIRE(Path::extension("inc/restify/forward") == "");
    REQUIRE(Path::extension("inc/restify/forward/test.js.html") == ".html");


}