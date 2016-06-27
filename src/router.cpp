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

    typedef std::shared_ptr<RequestHandler> RequestHandlerPtr;
    typedef std::vector<std::pair<std::string, std::string> > ArrayOfKeyValues;

    const std::string CapturePattern = "(?:([^\\/]+?))";
    const std::regex PathRegex(":([^\\/]+)?");

    class Route {
    public:
        Route(const Json::Value &cfg, RequestHandlerPtr handler)
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
            return (*_handler)(req, rep);
        }


        /** Match */
        bool match(Request & req, Json::Value &extractedParams) const {
            const std::string path = req.path();
            const std::string method = req.method();


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

            if (!_cfg["params"].isNull()) {
                for (auto i = 0; i < values.size() - 1; i++) {
                    if (!convertValue(_keys[i], values[i + 1], extractedParams))
                        return false;
                }
            } else {
                for (auto i = 0; i < values.size() - 1; i++) {
                    extractedParams[_keys[i]] = std::string(values[i + 1]);
                }
            }
            

            return true;

        }

    private:

        bool convertValue(const std::string &key, const std::string &value, Json::Value &target) const
        {
            try {
                const Json::Value keyOpts = _cfg["params"][key];
                if (keyOpts.isNull()) {
                    target[key] = value;
                    return true;
                }

                const Json::Value &jsontype = keyOpts["type"];
                if (jsontype.isString()) {
                    const std::string type = jsontype.asString();
                    std::size_t next = value.length();

                    if (type == "int") {
                        target[key] = std::stoi(value, &next);
                    } else if (type == "float") {
                        target[key] = std::stof(value, &next);
                    } else if (type == "double") {
                        target[key] = std::stod(value, &next);
                    } else if (type == "boolean") {
                        target[key] = (value == "yes" || value == "Yes" || value == "YES") ? true : false;
                    } else {
                        // Unknown type.
                        target[key] = value;
                    }

                    return next == value.length();

                } else {
                    target[key] = value;
                    return true;
                }

                

            } catch (std::invalid_argument &) {
                return false;
            }
        }

        Json::Value _cfg;
        RequestHandlerPtr _handler;
        std::regex _matchRegex;
        std::vector<std::string> _keys;    
    };

    struct Router::PrivateData {
        typedef std::vector<Route> ArrayOfRoute;

        ArrayOfRoute routes;
        
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
            _data->routes.emplace_back(cfg, std::make_shared<RequestHandler>(handler));
        }
        else {
            *i = Route(cfg, std::make_shared<RequestHandler>(handler));
        }

        return true;
    }

    bool Router::dispatch(Request & req, Response & rep) const
    {
        // Find first matching route

        Json::Value pathParams(Json::objectValue);
        auto i = std::find_if(_data->routes.begin(), _data->routes.end(), [&req, &pathParams](const Route &r) {
            pathParams.clear();
            return r.match(req, pathParams);
        });

        // Return when no route found
        if (i == _data->routes.end()) {
            return false;
        }

        // Copy all slug parameters to request object. Note,

        Json::Value &params = req.params();
        if (!jsonMerge(params, pathParams)) {
            return false;
        }        
        // Invoke handler
        return i->handle(req, rep);
    }

}
