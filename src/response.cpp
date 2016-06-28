/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/response.h>

namespace restify {
    
#define HEADERS_KEY "headers"
#define BODY_KEY "body"
#define HTTPSTATUSCODE_KEY "statusCode"
#define HTTPVERSION_KEY "version"
         

    Response::Response()
        :_root(Json::objectValue)
    {
        _root[HEADERS_KEY] = Json::Value(Json::objectValue);
    }

    Response::~Response()
    {
    }
    
    Response &Response::code(int code) {
        _root[HTTPSTATUSCODE_KEY] = code;
        return *this;
    }

    
    Response &Response::body(const Json::Value &value) {
        _root[BODY_KEY] = value;
        return *this;
    }
    
    Response &Response::header(const std::string &key, const Json::Value &value) {
        _root[HEADERS_KEY][key] = value;
        return *this;
    }
    
    Response &Response::version(const std::string &value) {
        _root[HTTPVERSION_KEY] = value;
        return *this;
    }
    
    const Json::Value &Response::toJson() const {
        return _root;
    }

}
