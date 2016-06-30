/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"


TEST_CASE("WARN_NO_CURL") {

    WARN(R"(
       **********************************
                    WARNING
       cpp-restify is built without CURL. 
       This disables many units.
       **********************************)");
}