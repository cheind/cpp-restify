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
#include <restify/http_renderer.h>
#include <restify/error.h>
#include <json/json.h>
#include "mongoose.h"

#include <iostream>

namespace restify {

    struct Server::PrivateData {
        struct mg_context *ctx;
        struct mg_callbacks callbacks;
        Router router;
        HttpRenderer defaultRenderer;
        ResponseRenderer renderResponse;
        ErrorRenderer renderError;

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

        // Default rendering
        _data->renderResponse = std::bind(&HttpRenderer::renderResponse, &_data->defaultRenderer, std::placeholders::_1);
        _data->renderError = std::bind(&HttpRenderer::renderError, &_data->defaultRenderer, std::placeholders::_1);
    }

    Server::~Server()
    {
        if (_data->ctx) {
            mg_stop(_data->ctx);
        }
    }

    Server &Server::route(const Json::Value & opts, const RequestHandler & handler) {
        _data->router.addRoute(opts, handler);
        return *this;
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

    int Server::onBeginRequestCallback(mg_connection * conn) {
        const struct mg_request_info *info = mg_get_request_info(conn);
        
        if (!info) {
            return 0;
        }

        Server *server = static_cast<Server*>(info->user_data);
        try {
            return server->handleRequest(conn, info) ? 1 : 0;
        } catch (...) {
            return 0;
        }
    }

    bool Server::handleRequest(mg_connection * conn, const mg_request_info * info) {

        try {
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

            // Route request
            Response response;
            if (!_data->router.dispatch(request, response)) {
                std::ostringstream oss;
                oss << "Route not found " << request.path();
                throw Error(StatusCode::NotFound, oss.str().c_str());                
            }

            // Format response
            const std::string responseString = _data->renderResponse(response);
            mg_write(conn, responseString.c_str(), responseString.size());

            return true;

        } catch (const Error &error) {
            const std::string responseString = _data->renderError(error);
            mg_write(conn, responseString.c_str(), responseString.size());
            return true;
        } catch (const std::exception &error) {
            Error myError(StatusCode::InternalServerError, error.what());
            const std::string responseString = _data->renderError(myError);
            mg_write(conn, responseString.c_str(), responseString.size());
            return true;
        } catch (...) {
            Error myError(StatusCode::InternalServerError, "Unknown error occurred. That's all we know.");
            const std::string responseString = _data->renderError(myError);
            mg_write(conn, responseString.c_str(), responseString.size());
            return true;
        }
    }

}
