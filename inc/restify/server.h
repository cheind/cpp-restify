/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_SERVER_H
#define CPP_RESTIFY_SERVER_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/non_copyable.h>
#include <json/json-forwards.h>
#include <memory>

struct mg_connection;
struct mg_request_info;

namespace restify {

    class CPPRESTIFY_INTERFACE Server : NonCopyable 
    {
    public:

        Server();
        Server(const Json::Value &options);
        ~Server();
        
        Server &setConfig(const Json::Value &options);
        
        Server &route(const Json::Value &opts, const RequestHandler &handler);
        Server &otherwise(const RequestHandler &handler);

        void start();
        void stop();


    private:
        static int onBeginRequestCallback(struct mg_connection *conn);

        bool handleRequest(struct mg_connection *conn, const struct mg_request_info *info);
        void readRequestHeaders(const struct mg_request_info *info, Request &request) const;
        void readRequestMethod(const struct mg_request_info *info, Request &request) const;
        void readRequestPath(const struct mg_request_info *info, Request &request) const;
        void readRequestBody(struct mg_connection *conn, Request &request) const;
        void readQueryString(const struct mg_request_info *info, Request &request) const;
        
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif