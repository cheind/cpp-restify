/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/response.h>
#include <restify/helpers.h>

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
    
    Response &Response::setCode(int setCode) {
        _root[HTTPSTATUSCODE_KEY] = setCode;
        return *this;
    }

    
    Response &Response::setBody(const Json::Value &value) {
        _root[BODY_KEY] = value;
        return *this;
    }
    
    Response &Response::setHeader(const std::string &key, const Json::Value &value) {
        _root[HEADERS_KEY][key] = value;
        return *this;
    }
    
    Response &Response::setVersion(const std::string &value) {
        _root[HTTPVERSION_KEY] = value;
        return *this;
    }

    Response & Response::setRedirectTo(const std::string & location, int code) {
        setCode(code);
        setHeader("Location", location);
        return *this;
    }

    JsonByPath<Response> Response::beginBody() {
        _root[BODY_KEY] = Json::objectValue;
        return JsonByPath<Response>(_root[BODY_KEY], *this);
    }
    
    const Json::Value &Response::toJson() const {
        return _root;
    }

}
