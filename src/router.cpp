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
#include <restify/helpers.h>
#include <json/json.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <regex>
#include <iostream>

namespace restify {
    const std::string CapturePattern = "(?:([^\\/]+?))";
    const std::regex PathRegex(":([^\\/]+)?");

    class Route {
    public:
        Route(const Json::Value &cfg, const RequestHandler &handler)
            :_cfg(cfg), _handler(handler)
        {
            const std::string path = cfg["path"].asString();

            // Find all slugs in path
            std::sregex_token_iterator ibegin(path.begin(), path.end(), PathRegex), iend;
            while (ibegin != iend) {
                std::string key = *ibegin++;
                _keys.push_back(key.erase(0, 1));
            }

            // Create match regex
            std::string escaped = std::regex_replace(path, PathRegex, CapturePattern);
            _matchRegex = std::regex(std::string("^") + escaped + "$");
        }

        const std::string pathTemplate() const {
            return _cfg["path"].asString();
        }

        bool handle(Request & req, Response & rep) const {
            return _handler(req, rep);
        }


        /** Match */
        bool match(Request & req, Json::Value &extractedParams) const {
            const std::string path = req.getPath();
            const std::string method = req.getMethod();


            const Json::Value &methods = _cfg["methods"];

            if (methods.isArray()) {
                auto i = std::find_if(methods.begin(), methods.end(), [&method](const Json::Value &v) {
                    return method == v.asString();
                });
                
                if (i == methods.end())
                    return false;
            } else {
                // Method is string.
                if (method != methods.asString())
                    return false;
            }
            
            
            std::smatch values;
            if (!std::regex_match(path, values, _matchRegex)) {
                return false;
            }

            if (values.size() < 1) 
                return true;
            
            for (auto i = 0; i < values.size() - 1; i++) {
                extractedParams[_keys[i]] = std::string(values[i + 1]);
            }
            
            return true;
        }


        Json::Value _cfg;
        RequestHandler _handler;
        std::regex _matchRegex;
        std::vector<std::string> _keys;    
    };

    struct Router::PrivateData {
        typedef std::vector<Route> ArrayOfRoute;
        ArrayOfRoute routes;
        RequestHandler defaultRoute;
        
        PrivateData() {}
    };


         

    Router::Router()
        :_data(new PrivateData())
    {

    }

    Router::~Router()
    {
    }

    bool Router::addRoute(const Json::Value & cfg, const RequestHandler & handler)
    {
        const Json::Value &path = cfg["path"];
        const Json::Value &methods = cfg["methods"];

        if (!path.isString())
            return false;

        if (!(methods.isString() || methods.isArray()))
            return false;


        auto i = std::find_if(_data->routes.begin(), _data->routes.end(), [&path](const Route &r) {
            return r.pathTemplate() == path.asString();
        });

        if (i == _data->routes.end()) {
            _data->routes.emplace_back(cfg, handler);
        } else {
            *i = Route(cfg, handler);
        }

        return true;
    }

    void Router::setDefaultRoute(const RequestHandler & handler) {
        _data->defaultRoute = handler;
    }

    bool Router::dispatch(Request & req, Response & rep) const
    {
        // Loop over routes until the first one handles the request

        auto i = std::find_if(_data->routes.begin(), _data->routes.end(), [&req, &rep](const Route &r) {
            Json::Value pathParams(Json::objectValue);
            
            // Test if request path is compatible with route path template.
            if (!r.match(req, pathParams))
                return false;
            
            Json::Value &getParams = req.toJson()[Request::Keys::params];
            if (!jsonMerge(getParams, pathParams)) {
                CPPRESTIFY_FAIL(StatusCode::InternalServerError, "Failed to merge parameters.");
            }
            
            // Invoke handler
            return r.handle(req, rep);
        });

        if (i == _data->routes.end()) {
            // No route matched, invoke default handle if available.
            if (_data->defaultRoute) {
                return _data->defaultRoute(req, rep);
            } else {
                return false;
            }
        } else {
            return true;
        }
        
    }

}
