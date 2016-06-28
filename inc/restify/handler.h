/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_HANDLER_H
#define CPP_RESTIFY_HANDLER_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>

namespace restify {

    class CPPRESTIFY_INTERFACE RedirectRequestHandler
    {
    public:

        RedirectRequestHandler(Json::Value cfg = Json::Value(Json::nullValue));

        bool operator()(const Request &req, Response &rep) const;

    private:
        CPPRESTIFY_NO_INTERFACE_WARN(Json::Value, _cfg);
    };

}

#endif