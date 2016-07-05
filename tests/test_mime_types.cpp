/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/mime_types.h>

TEST_CASE("mimetypes-should-be-resolvable-from-file-extension")
{
   using namespace restify;

   REQUIRE(MimeTypes::resolveFromFileExtension(".pdf") == "application/pdf");
   REQUIRE(MimeTypes::resolveFromFileExtension(".json") == "application/json");
   REQUIRE(MimeTypes::resolveFromFileExtension(".JsON") == "application/json");
   REQUIRE(MimeTypes::resolveFromFileExtension(".html") == "text/html");
   REQUIRE(MimeTypes::resolveFromFileExtension(".HTML") == "text/html");

   REQUIRE(MimeTypes::resolveFromFileExtension(".nosuchextension", "nota/mimetype") == "nota/mimetype");

}