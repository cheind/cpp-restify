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
    class Client;
    class JsonBuilder;
    class Connection;
    class RequestHeaderReader;
    class RequestBodyReader;
    class ResponseWriter;
    class Route;
    class AnyRoute;
    class ParameterRoute;
    class BackendContext;
    class Backend;
    class MimeTypes;


    typedef std::function<bool(const Request &req, Response &rep)> RequestHandler;

    typedef std::function<bool(const BackendContext &ctx, Connection &c)> BackendRequestHandler;
}

#endif
