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

namespace restify {

    class CPPRESTIFY_INTERFACE Server : NonCopyable 
    {
    public:

        Server();
        Server(const Json::Value &options);
        ~Server();
        
        Server &setBackend(std::shared_ptr<Backend> backend);
        Server &setConfig(const Json::Value &options);        
        Server &route(const Json::Value &opts, const RequestHandler &handler);
        Server &otherwise(const RequestHandler &handler);

        Server &start();
        Server &stop();


    private:

        bool onBackendRequest(const BackendContext &ctx, Connection &conn) const;


        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif