/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_ROUTE_H
#define CPP_RESTIFY_ROUTE_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/non_copyable.h>
#include <json/json-forwards.h>
#include <memory>

namespace restify {

    class CPPRESTIFY_INTERFACE Route {
    public:
        virtual bool match(const Request &request, Json::Value &extractedParams) const = 0;
        virtual void updateRequest(Request &request, const Json::Value &extractedParams) const = 0;
        virtual void call(Request &request, Response &rep) const = 0;
    };

    class CPPRESTIFY_INTERFACE RequestHandlerRoute : public Route {
    public:
        RequestHandlerRoute(const RequestHandler &handler);

        void call(Request &request, Response &rep) const override;
    private:
        CPPRESTIFY_NO_INTERFACE_WARN(RequestHandler, _handler);
    };

    class CPPRESTIFY_INTERFACE AnyRoute : public RequestHandlerRoute {
    public:
        AnyRoute(const RequestHandler &handler);

        virtual bool match(const Request & request, Json::Value & extractedParams) const override;
        virtual void updateRequest(Request & request, const Json::Value & extractedParams) const override;
    };

    class CPPRESTIFY_INTERFACE ParameterRoute : public RequestHandlerRoute, NonCopyable {
    public:
        ParameterRoute(const Json::Value &config, const RequestHandler &handler);
        ~ParameterRoute();

        virtual bool match(const Request & request, Json::Value & extractedParams) const override;
        virtual void updateRequest(Request & request, const Json::Value & extractedParams) const override;
    private:
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif