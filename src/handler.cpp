/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/response.h>
#include <restify/request.h>
#include <restify/helpers.h>
#include <restify/handler.h>

namespace restify {

    RedirectRequestHandler::RedirectRequestHandler(Json::Value cfg) 
    {
        _cfg = JsonBuilder()
            .set("url", "/")
            .set("statusCode", 301)
            .toJson();
        jsonMerge(_cfg, cfg);
    }

    bool RedirectRequestHandler::operator()(const Request & req, Response & rep) const {

        rep.setRedirectTo(
            json_cast<std::string>(_cfg["url"].asString()), 
            json_cast<int>(_cfg["statusCode"].asInt()));

        return true;
    }

    }
