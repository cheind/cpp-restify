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
    
        

    Response::Response()
        :_root(Json::objectValue)
    {
        _root[Keys::headers] = Json::Value(Json::objectValue);
    }

    Response::Response(const Json::Value & opts) 
        : _root(opts)
    {
        if (_root[Keys::headers].isNull())
            _root[Keys::headers] = Json::Value(Json::objectValue);
    }

    Response::~Response()
    {
    }
    
    Response &Response::setCode(int setCode) {
        _root[Keys::statusCode] = setCode;
        return *this;
    }

    
    Response &Response::setBody(const Json::Value &value) {
        _root[Keys::body] = value;
        return *this;
    }
    
    Response &Response::setHeader(const std::string &key, const Json::Value &value) {
        _root[Keys::headers][key] = value;
        return *this;
    }
    
    Response &Response::setVersion(const std::string &value) {
        _root[Keys::version] = value;
        return *this;
    }

    Response & Response::setRedirectTo(const std::string & location, int code) {
        setCode(code);
        setHeader("Location", location);
        return *this;
    }

    Response & Response::setFile(const std::string & path) {
        // TODO: insert return statement here
        return *this;
    }

    Response::JsonBodyBuilder Response::beginBody() {
        return JsonBodyBuilder(*this);
    }
    
    const Json::Value &Response::toJson() const {
        return _root;
    }

    Json::Value & Response::toJson() {
        return _root;
    }

    Response::JsonBodyBuilder::JsonBodyBuilder(Response & response)
        :_response(response), _builder(response.toJson()[Keys::body])
    {
    }

    Response::JsonBodyBuilder & Response::JsonBodyBuilder::set(const std::string & key, const Json::Value & value) {
        _builder.set(key, value);
        return *this;
    }

    Response & Response::JsonBodyBuilder::endBody() {
        return _response;
    }

}
