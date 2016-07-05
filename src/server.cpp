/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/server.h>
#include <restify/backend.h>
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
#include <iostream>

#include <restify/mongoose/mongoose_backend.h>

namespace restify {

    struct Server::PrivateData {
        std::shared_ptr<Backend> backend;
        Router router;
        Json::Value config;
        
        PrivateData()
        {}
    };
    
    Server::Server()
    :_data(new PrivateData)
    {
        // Currently we only support Mongoose.
        setBackend(std::make_shared<MongooseBackend>());
    }
    

    Server::Server(const Json::Value & options)
        : Server()
    {
        setConfig(options);
    }
    
    Server & Server::setBackend(std::shared_ptr<Backend> backend) {
        if (_data->backend) {
            _data->backend->stop();
            _data->backend->setRequestCallback(BackendRequestHandler());
        }
        _data->backend = backend;
        _data->backend->setRequestCallback(std::bind(&Server::onBackendRequest, this, std::placeholders::_1, std::placeholders::_2));
        return *this;
    }

    Server &Server::setConfig(const Json::Value &options) {
        if (_data->backend) {
            const Json::Value &backendCfg = options["backend"];
            if (!backendCfg.isNull()) {
                _data->backend->setConfig(backendCfg);
            }
        }
        jsonMerge(_data->config, options);
        return *this;
    }

    Server::~Server()
    {
        if (_data->backend)
            _data->backend->stop();
    }

    Server &Server::route(const Json::Value & opts, const RequestHandler & handler) {
        _data->router.addRoute(std::make_shared<ParameterRoute>(opts, handler));
        return *this;
    }

    Server & Server::otherwise(const RequestHandler & handler) {
        _data->router.addRoute(std::make_shared<AnyRoute>(handler));
        return *this;
    }

    Server & Server::start()
    {
        if (_data->backend)
            _data->backend->start();
        return *this;
    }

    Server & Server::stop()
    {
        if (_data->backend)
            _data->backend->stop();
        return *this;
    }

    bool Server::onBackendRequest(const BackendContext & ctx, Connection & conn) const {
        
        DefaultResponseWriter writer;

        try {
            // Setup request object
            Request request;

            // Read request.
            ctx.getRequestHeaderReader().readRequestHeader(conn, request);
            ctx.getRequestBodyReader().readRequestBody(conn, request);

            // Route request
            Response response;
            if (!_data->router.route(request, response)) {
                std::ostringstream oss;
                oss << "Route not found " << request.getPath();
                throw Error(StatusCode::NotFound, oss.str().c_str());
            }

			// Enable cors for now.
			response.setHeader("Access-Control-Allow-Origin", "*");
            writer.writeResponse(conn, response);

            return true;

        } catch (const Error &error) {
            Response rep(error.toJson());
            writer.writeResponse(conn, rep);
            return true;
        } catch (const std::exception &error) {
            Error myError(StatusCode::InternalServerError, error.what());
            Response rep(myError.toJson());
            writer.writeResponse(conn, rep);
            return true;
        } catch (...) {
            Error myError(StatusCode::InternalServerError, "Unknown error occurred. That's all we know.");
            Response rep(myError.toJson());
            writer.writeResponse(conn, rep);
            return true;
        }

        return false;
    }
}
