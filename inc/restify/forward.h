/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_FORWARD_H
#define CPP_RESTIFY_FORWARD_H

//#include <restify/request_handler.h>
#include <functional>

namespace restify {

    class Request;
    class Response;
    class Server;
    class Router;

    typedef std::function<bool(const Request &req, Response &rep) > RequestHandler;
}

#endif
