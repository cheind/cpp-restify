/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_BACKEND_H
#define CPP_RESTIFY_BACKEND_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/non_copyable.h>
#include <json/json-forwards.h>
#include <memory>

namespace restify {



    class CPPRESTIFY_INTERFACE BackendContext {
    public:

        virtual const RequestHeaderReader &getRequestHeaderReader() const = 0;
        virtual const RequestBodyReader &getRequestBodyReader() const = 0;
    };

    class CPPRESTIFY_INTERFACE Backend {
    public:
        virtual bool setConfig(const Json::Value &options) = 0;
        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool setRequestCallback(const BackendRequestHandler &handler) = 0;

    };

   

}

#endif