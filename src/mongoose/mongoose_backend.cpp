/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/mongoose/mongoose_backend.h>
#include <restify/mongoose/mongoose_connection.h>
#include <restify/mongoose/mongoose_request_reader.h>
#include <restify/connection.h>
#include <restify/request_reader.h>
#include <restify/response_writer.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <regex>
#include <iostream>

#include "mongoose.h"

namespace restify {

    struct MongooseBackendContext::PrivateData {
        std::unique_ptr<RequestHeaderReader> _headerReader;
        std::unique_ptr<RequestBodyReader> _bodyReader;

        PrivateData()
            :_headerReader(new MongooseRequestHeaderReader())
            , _bodyReader(new DefaultRequestBodyReader()) 
        {
        }        
    };


    MongooseBackendContext::MongooseBackendContext() 
        :_data(new PrivateData())
    {}

    MongooseBackendContext::~MongooseBackendContext() {
    }

    const RequestHeaderReader & MongooseBackendContext::getRequestHeaderReader() const {
        return *_data->_headerReader;
    }

    const RequestBodyReader & MongooseBackendContext::getRequestBodyReader() const {
        return *_data->_bodyReader;
    }


    struct MongooseBackend::PrivateData {
        struct mg_context *ctx;
        struct mg_callbacks callbacks;
        Json::Value config;
        BackendRequestHandler handler;
        MongooseBackendContext context;
        bool isRunning;
        
        PrivateData()
            :ctx(nullptr), isRunning(false)
        {
            memset(&callbacks, 0, sizeof(callbacks));
        }
    };

    static int onLogMessage(const mg_connection *conn, const char *message) {
        return 1;
    }
    
    MongooseBackend::MongooseBackend()
    : _data(new PrivateData)
    {
        _data->callbacks.begin_request = &MongooseBackend::onBeginRequestCallback;
        _data->callbacks.log_message = onLogMessage;
        
        json(_data->config)
            ("listening_ports", "127.0.0.1:8080")
            ("num_threads", 50);
    }

    MongooseBackend::~MongooseBackend()
    {
        stop();
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

    bool MongooseBackend::start()
    {
        std::vector<std::string> strings;
        createMongooseOptionStrings(_data->config, strings);
        
        std::vector<const char*> cstrings;
        for (auto &s : strings) {
            cstrings.push_back(s.c_str());
        }
        cstrings.push_back(nullptr);
    
        _data->ctx = mg_start(&_data->callbacks, this, &cstrings.at(0));
        _data->isRunning = _data->ctx != 0;
        return _data->isRunning;
    }

    bool MongooseBackend::stop()
    {
        if (_data->ctx) {
            mg_stop(_data->ctx);
            _data->ctx = 0;
        }
        _data->isRunning = false;
        return true;
    }

    bool MongooseBackend::setConfig(const Json::Value & options) {
        return jsonMerge(_data->config, options);
    }

    bool MongooseBackend::setRequestCallback(const BackendRequestHandler & handler) {
        if (_data->isRunning)
            return false;

        _data->handler = handler;
        return true;
    }

    int MongooseBackend::onBeginRequestCallback(mg_connection * conn) {
        const struct mg_request_info *info = mg_get_request_info(conn);
        
        if (!info) {
            return 0;
        }

        MongooseBackend *backend = static_cast<MongooseBackend*>(info->user_data);
        try {
            return backend->handleRequest(conn, info) ? 1 : 0;
        } catch (...) {
            return 0;
        }
    }

    bool MongooseBackend::handleRequest(mg_connection * conn, const mg_request_info * info) {
        if (_data->handler) {
            MongooseConnection mconn(conn);
            return _data->handler(_data->context, mconn);
        } else {
            return false;
        }
    }

}
