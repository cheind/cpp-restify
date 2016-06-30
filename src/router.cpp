/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/router.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/route.h>
#include <json/json.h>
#include <vector>
#include <algorithm>

namespace restify {
   

    typedef std::shared_ptr<Route const> RouteConstPtr;

    struct Router::PrivateData {        
        typedef std::vector<RouteConstPtr> ArrayOfRoutes;
        ArrayOfRoutes routes;
        
        PrivateData() {}
    };

    Router::Router()
        :_data(new PrivateData())
    {}

    Router::~Router()
    {}

    void Router::addRoute(std::shared_ptr<const Route> route) {
        _data->routes.push_back(route);
    }

    bool Router::route(Request & req, Response & rep) const {
        // Loop over routes until the first one handles the request

        auto i = std::find_if(_data->routes.begin(), _data->routes.end(), [&req, &rep](const RouteConstPtr &r) {

            Json::Value extractedParams(Json::objectValue);

            // Test if request path is compatible with route path template.
            if (!r->match(req, extractedParams))
                return false;

            // Merge in extracted parameters into request.
            r->updateRequest(req, extractedParams);
            
            // Invoke handler
            r->call(req, rep);

            return true;
        });

        return i != _data->routes.end();
    }

}
