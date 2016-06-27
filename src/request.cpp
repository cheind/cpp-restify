/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/request.h>
#include <restify/helpers.h>
#include <json/json.h>

namespace restify {

    #define PARAMS_KEY "params"
    #define HEADERS_KEY "headers"
    #define METHOD_KEY "method"
    #define PATH_KEY "path"
    #define QUERY_KEY "queryString"

    struct Request::PrivateData {

        Json::Value root;

        PrivateData()
            :root(Json::objectValue)
        {
            root[PARAMS_KEY] = Json::Value(Json::objectValue);
            root[HEADERS_KEY] = Json::Value(Json::objectValue);
        }
    };

    Request::Request()
        :_data(new PrivateData())
    {}


    Request::~Request()        
    {}
  
    std::string Request::method() const
    {
        return _data->root.get(METHOD_KEY, "GET").asString();
    }

    Request &Request::method(const std::string & method)
    {
        _data->root[METHOD_KEY] = method;
        return *this;
    }

    std::string Request::path() const
    {
        return _data->root.get(PATH_KEY, "/").asString();
    }

    Request &Request::path(const std::string & path)
    {
        _data->root[PATH_KEY] = path;
        return *this;
    }
    
    std::string Request::queryString() const {
        return _data->root.get(QUERY_KEY, "").asString();
    }
    
    Request &Request::queryString(const std::string &path) {
        _data->root[QUERY_KEY] = path;
        
        // Split string at '&'
        Json::Value &keyvals = params();
        std::vector<std::string> pairs = splitString(path, '&', false);
        for (auto p : pairs) {
            std::vector<std::string> keyval = splitString(p, '=', true);
            
            if (keyval.size() != 2) {
                CPPRESTIFY_FAIL(StatusCode::BadRequest, "Query string is malformed.");
            }
            
            keyvals[keyval[0]] = keyval[1];
        }
        
        return *this;
    }

    const Json::Value & Request::params() const
    {
        return _data->root[PARAMS_KEY];
    }

    const Json::Value & Request::param(const std::string & key) const
    {
        return _data->root[PARAMS_KEY][key];
    }

    const Json::Value Request::param(const std::string & key, const Json::Value & defaultValue) const {
        return _data->root[PARAMS_KEY].get(key, defaultValue);
    }

    Json::Value & Request::params()
    {
        return _data->root[PARAMS_KEY];
    }

    const Json::Value & Request::headers() const
    {
        return _data->root[HEADERS_KEY];
    }

    Json::Value & Request::headers()
    {
        return _data->root[HEADERS_KEY];
    }

    const Json::Value & Request::header(const std::string & key) const
    {
        return _data->root[HEADERS_KEY][key];
    }

    const Json::Value Request::header(const std::string & key, const Json::Value & defaultValue) const {
        return _data->root[HEADERS_KEY].get(key, defaultValue);
    }
    
    const Json::Value Request::toJson() const {
        return _data->root;
    }

}
