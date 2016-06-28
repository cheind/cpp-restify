/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_ROUTER_H
#define CPP_RESTIFY_ROUTER_H

#include <restify/interface.h>
#include <restify/non_copyable.h>
#include <restify/forward.h>
#include <json/json-forwards.h>
#include <memory>
#include <string>

namespace restify {

    class CPPRESTIFY_INTERFACE Router : NonCopyable 
    {
    public:

        Router();
        ~Router();

        /** Add a new route. */
        bool addRoute(const Json::Value &opts, const RequestHandler &handler);

        /** Set route handler that is invoked when no other route is matched. */
        void setDefaultRoute(const RequestHandler &handler);
        
        /** Dispatch a request. */
        bool dispatch(Request &req, Response &rep) const;

       
    private:
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif