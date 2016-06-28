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

    Request::Request()
        :_root(Json::objectValue)
    {
        _root[Keys::params] = Json::Value(Json::objectValue);
        _root[Keys::headers] = Json::Value(Json::objectValue);
    }
    
    Request::Request(const Json::Value & opts)
        :_root(opts)
    {
        if (_root[Keys::params].isNull())
            _root[Keys::params] = Json::Value(Json::objectValue);

        if (_root[Keys::headers].isNull())
            _root[Keys::headers] = Json::Value(Json::objectValue);
    }

    Request::~Request()        
    {}
  
    std::string Request::getMethod() const
    {
        return _root.get(Keys::method, "GET").asString();
    }

    std::string Request::getPath() const
    {
        return _root.get(Keys::path, "/").asString();
    }
    
    std::string Request::getQueryString() const {
        return _root.get(Keys::query, "").asString();
    }

    Json::Value Request::getBody() const {
        return _root.get(Keys::body, "").asString();
    }

    const Json::Value & Request::getParams() const
    {
        return _root[Keys::params];
    }

    const Json::Value & Request::getParam(const std::string & key) const
    {
        return _root[Keys::params][key];
    }

    const Json::Value Request::getParam(const std::string & key, const Json::Value & defaultValue) const {
        return _root[Keys::params].get(key, defaultValue);
    }

    const Json::Value & Request::getHeaders() const
    {
        return _root[Keys::headers];
    }

    const Json::Value & Request::getHeader(const std::string & key) const {
        return _root[Keys::headers][key];
    }

    const Json::Value Request::getHeader(const std::string & key, const Json::Value & defaultValue) const {
        return _root[Keys::headers].get(key, defaultValue);
    }
    
    const Json::Value &Request::toJson() const {
        return _root;
    }

    Json::Value & Request::toJson() {
        return _root;
    }

}
