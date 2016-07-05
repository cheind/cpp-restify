/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/route.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/error.h>
#include <regex>
#include <string>

namespace restify {
    RequestHandlerRoute::RequestHandlerRoute(const RequestHandler & handler) 
        :_handler(handler)
    {}

    void RequestHandlerRoute::call(Request & request, Response & rep) const 
    {
        if (_handler) {
            _handler(request, rep);
        }
    }


    AnyRoute::AnyRoute(const RequestHandler & handler)
        :RequestHandlerRoute(handler)
    {}

    bool AnyRoute::match(const Request & request, Json::Value & extractedParams) const {
        return true;
    }

    void AnyRoute::updateRequest(Request & request, const Json::Value & extractedParams) const {
        // Nothing todo
    }


    const std::string CapturePattern = "(?:([^\\/]+?))";
    const std::regex PathRegex(":([^\\/]+)?");

    struct ParameterRoute::PrivateData {
        Json::Value cfg;
        std::regex matchRegex;
        std::vector<std::string> keys;
    };

    ParameterRoute::ParameterRoute(const Json::Value & config, const RequestHandler & handler) 
        :RequestHandlerRoute(handler), _data(new PrivateData())
    {
        json(_data->cfg)
            ("ignoreTrailingSlashes", true)
            ("methods", "GET");
        jsonMerge(_data->cfg, config);

        const bool ignoreTrailingSlashes = json_cast<bool>(_data->cfg["ignoreTrailingSlashes"]);

        std::string path = _data->cfg.get("path", "").asString();
        if (path.empty()) {
            throw Error(StatusCode::InternalServerError, "Path parameter not set.");
        }

        if (ignoreTrailingSlashes) {
            path.erase(path.find_last_not_of('/') + 1);
        }

        // Find all slugs in path
        std::sregex_token_iterator ibegin(path.begin(), path.end(), PathRegex), iend;
        while (ibegin != iend) {
            std::string key = *ibegin++;
            _data->keys.push_back(key.erase(0, 1));
        }

        // Create match regex
        std::string escaped = std::regex_replace(path, PathRegex, CapturePattern);
        
        _data->matchRegex = std::regex(std::string("^") + escaped + (ignoreTrailingSlashes ? "/*" : "") + "$");
    }

    ParameterRoute::~ParameterRoute() {
    }

    bool ParameterRoute::match(const Request & request, Json::Value & extractedParams) const {

        extractedParams = Json::Value(Json::objectValue);

        const std::string path = request.getPath();
        const std::string method = request.getMethod();


        const Json::Value &methods = _data->cfg.get("methods", "GET");

        if (methods.isArray()) {
            auto i = std::find_if(methods.begin(), methods.end(), [&method](const Json::Value &v) {
                return method == json_cast<std::string>(v);
            });

            if (i == methods.end())
                return false;
        } else if (methods.isString()) {
            // Method is string.
            if (method != methods.asString())
                return false;
        } else {
            throw Error(StatusCode::InternalServerError, "Field methods needs to be string or array of strings.");
        }


        std::smatch values;
        if (!std::regex_match(path, values, _data->matchRegex)) {
            return false;
        }

        // If route has no params, that's ok too.
        if (values.size() < 1)
            return true;

        for (auto i = 0; i < values.size() - 1; i++) {
            extractedParams[_data->keys[i]] = std::string(values[i + 1]);
        }

        return true;
    }

    void ParameterRoute::updateRequest(Request & request, const Json::Value & extractedParams) const {
        Json::Value &params = request.toJson()[Request::Keys::params];
        jsonMerge(params, extractedParams);
    }

}

