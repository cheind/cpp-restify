/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_MONGOOSE_BACKEND_H
#define CPP_RESTIFY_MONGOOSE_BACKEND_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/non_copyable.h>
#include <restify/backend.h>
#include <json/json-forwards.h>
#include <memory>

struct mg_connection;
struct mg_request_info;

namespace restify {

    class CPPRESTIFY_INTERFACE MongooseBackendContext : public BackendContext, NonCopyable {
    public:
        MongooseBackendContext();
        ~MongooseBackendContext();

        // Inherited via BackendContext
        virtual const RequestHeaderReader & getRequestHeaderReader() const override;
        virtual const RequestBodyReader & getRequestBodyReader() const override;

    private:
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

    class CPPRESTIFY_INTERFACE MongooseBackend : public Backend, NonCopyable
    {
    public:

        MongooseBackend();
        ~MongooseBackend();
        
        // Inherited via Backend
        virtual bool setConfig(const Json::Value & options) override;
        virtual bool start() override;
        virtual bool stop() override;
        virtual bool setRequestCallback(const BackendRequestHandler & handler) override;


    private:
        static int onBeginRequestCallback(struct mg_connection *conn);

        bool handleRequest(struct mg_connection *conn, const struct mg_request_info *info);
        
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif