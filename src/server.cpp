/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/server.h>
#include <restify/router.h>
#include <restify/request.h>
#include <restify/response.h>
#include <json/json.h>
#include "mongoose.h"

#include <iostream>

namespace restify {

    struct Server::PrivateData {
        struct mg_context *ctx;
        struct mg_callbacks callbacks;
        Router router;

        PrivateData() {
            memset(&callbacks, 0, sizeof(callbacks));
        }
    };

    static int onLogMessage(const mg_connection *conn, const char *message) {
        std::cout << message << std::endl;
        return 1;
    }
         

    Server::Server(const Json::Value & options)
        :_data(new PrivateData())
    {
        _data->callbacks.begin_request = &Server::onBeginRequestCallback;
        _data->callbacks.log_message = onLogMessage;
    }

    Server::~Server()
    {
        if (_data->ctx) {
            mg_stop(_data->ctx);
        }
    }

    bool Server::route(const Json::Value & opts, const RequestHandler & handler) {
        return _data->router.addRoute(opts, handler);
    }

    void Server::start()
    {
        const char *opts[] = { "listening_ports", "8080", NULL };
        _data->ctx = mg_start(&_data->callbacks, this, opts);
    }

    void Server::stop()
    {
        mg_stop(_data->ctx);
    }

    bool Server::handleRequest(Request & req, Response & rep) {
        return _data->router.dispatch(req, rep);

    }

    int Server::onBeginRequestCallback(mg_connection * conn) {
        const struct mg_request_info *info = mg_get_request_info(conn);
        
        if (!info) {
            return 0;
        }

        Server *server = static_cast<Server*>(info->user_data);


        // Setup request object
        Request request;
        
        request
        .path(info->uri)
        .method(info->request_method);

        if (info->query_string) {
            const int bufferSize = 2048;
            char buffer[bufferSize];
            
            mg_url_decode(info->query_string, (int)strlen(info->query_string), buffer, bufferSize, 0);
            
            request.queryString(buffer);
        }
        
        Json::Value &headers = request.headers();
        for (int i = 0; i < info->num_headers; ++i) {
            headers[info->http_headers[i].name] = info->http_headers[i].value;
        }
        

        Response response;
        if (!server->handleRequest(request, response)) {
            return 0;
        } else {
            return 1;
        }


       
           

        /*
        
        
        char content[100];

        // Prepare the message we're going to send
        int content_length = snprintf(content, sizeof(content),
            "Hello from mongoose! Remote port: %d",
            request_info->remote_port);

        // Send HTTP reply to the client
        mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"        // Always set Content-Length
            "\r\n"
            "%s",
            content_length, content);

        // Returning non-zero tells mongoose that our function has replied to
        // the client, and mongoose should not send client any more data.
        return 1;
        */
    }

}
