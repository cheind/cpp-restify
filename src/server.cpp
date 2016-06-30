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
#include <restify/route.h>
#include <restify/error.h>
#include <restify/helpers.h>
#include <restify/connection.h>
#include <restify/request_reader.h>
#include <restify/response_writer.h>
#include <json/json.h>
#include <regex>

#include "mongoose.h"

namespace restify {

    struct Server::PrivateData {
        struct mg_context *ctx;
        struct mg_callbacks callbacks;
        Router router;
        Json::Value config;
        
        PrivateData()
        :ctx(nullptr)
        {
            memset(&callbacks, 0, sizeof(callbacks));
        }
    };

    static int onLogMessage(const mg_connection *conn, const char *message) {
        return 1;
    }
    
    Server::Server()
    :_data(new PrivateData)
    {
        _data->callbacks.begin_request = &Server::onBeginRequestCallback;
        _data->callbacks.log_message = onLogMessage;
        
        json(_data->config)
            ("listening_ports", "127.0.0.1:8080")
            ("num_threads", 50);
        
        
    }
    

    Server::Server(const Json::Value & options)
        : Server()
    {
        setConfig(options);
    }
    
    Server &Server::setConfig(const Json::Value &options) {
        jsonMerge(_data->config, options);
        return *this;
    }

    Server::~Server()
    {
        if (_data->ctx) {
            mg_stop(_data->ctx);
        }
    }

    Server &Server::route(const Json::Value & opts, const RequestHandler & handler) {
        _data->router.addRoute(std::make_shared<ParameterRoute>(opts, handler));
        return *this;
    }

    Server & Server::otherwise(const RequestHandler & handler) {
        _data->router.addRoute(std::make_shared<AnyRoute>(handler));
        return *this;
    }
    
    void createMongooseOptionStrings(const Json::Value &obj, std::vector<std::string> &dst) {
        if (!obj.isObject())
            return;
        
        for (auto &e : obj.getMemberNames()) {
            const Json::Value &v = obj[e];
            if (v.isObject())
                createMongooseOptionStrings(v, dst);
            dst.push_back(e);
            dst.push_back(json_cast<std::string>(v));
        }
    }

    void Server::start()
    {
        std::vector<std::string> strings;
        createMongooseOptionStrings(_data->config, strings);
        
        std::vector<const char*> cstrings;
        for (auto &s : strings) {
            cstrings.push_back(s.c_str());
        }
        cstrings.push_back(nullptr);
    
        _data->ctx = mg_start(&_data->callbacks, this, &cstrings.at(0));
    }

    void Server::stop()
    {
        if (_data->ctx) {
            mg_stop(_data->ctx);
            _data->ctx = 0;
        }
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

        MongooseConnection mconn(conn);
        DefaultResponseWriter writer;
        MongooseRequestHeaderReader headerReader;
        DefaultRequestBodyReader bodyReader;
        
        try {
            // Setup request object
            Request request;

            // Read request.
            headerReader.readRequestHeader(mconn, request);
            bodyReader.readRequestBody(mconn, request);

            // Route request
            Response response;
            if (!_data->router.route(request, response)) {
                std::ostringstream oss;
                oss << "Route not found " << request.getPath();
                throw Error(StatusCode::NotFound, oss.str().c_str());                
            }

            writer.writeResponse(mconn, response);
        
            return true;

        } catch (const Error &error) {
            Response rep(error.toJson());
            writer.writeResponse(mconn, rep);
            return true;
        } catch (const std::exception &error) {
            Error myError(StatusCode::InternalServerError, error.what());
            Response rep(myError.toJson());
            writer.writeResponse(mconn, rep);
            return true;
        } catch (...) {
            Error myError(StatusCode::InternalServerError, "Unknown error occurred. That's all we know.");
            Response rep(myError.toJson());
            writer.writeResponse(mconn, rep);
            return true;
        }
    }

}
