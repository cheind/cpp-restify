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
    class RequestReader;
    class ResponseWriter;
    class MongooseHttpRequestReader;
    class Route;
    class AnyRoute;
    class ParameterRoute;


    typedef std::function<bool(const Request &req, Response &rep)> RequestHandler;
}

#endif
