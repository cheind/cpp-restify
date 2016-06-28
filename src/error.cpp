/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/error.h>
#include <restify/response.h>

namespace restify {

    Error::Error(StatusCode setCode)
    :std::runtime_error("cpp-restify error")
    {
        Response r;
        r.setCode(int(setCode));
        _details = r.toJson();
    }

    Error::Error(const Response & response) 
        :std::runtime_error("cpp-restify error"), _details(response.toJson())
    {}
    
    Error::Error(StatusCode setCode, const char *message)
    :std::runtime_error(message)
    {
        Json::Value setBody;
        setBody["statusCode"] = int(setCode);
        setBody["message"] = message;

        Response r;
        r.setCode(int(setCode)).setBody(setBody);
        _details = r.toJson();
    }
    
    Error::Error(StatusCode setCode, const char *message, int line, const char *file)
    : std::runtime_error(message)
    {
        Json::Value setBody;
        setBody["statusCode"] = int(setCode);
        setBody["message"] = message;
        setBody["lineNumber"] = line;
        setBody["fileName"] = file;

        Response r;
        r.setCode(int(setCode)).setBody(setBody);

        _details = r.toJson();
    }

    Error::Error(const Error & other)
        :runtime_error(other.what())
    {
        _details = other._details;
    }

    Error::~Error() {}

    const Json::Value & Error::toJson() const {
        return _details;
    }
}
