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
#include <string>

namespace restify {

    class Request;
    class Response;
    class Server;
    class Router;
    class Error;
    class HttpRenderer;
    class Client;
    template<class EnclosingType> class JsonByPath;

    typedef std::function<bool(const Request &req, Response &rep)> RequestHandler;
    typedef std::function<std::string (const Response &rep)> ResponseRenderer;
    typedef std::function<std::string (const Error &err)> ErrorRenderer;
}

#endif
