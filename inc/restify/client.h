/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_CLIENT_H
#define CPP_RESTIFY_CLIENT_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/non_copyable.h>
#include <json/json-forwards.h>
#include <future>

namespace restify {

    class CPPRESTIFY_INTERFACE Client : NonCopyable 
    {
    public:
        static Json::Value invoke(const Json::Value &request);
    };

}

#endif