/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/error.h>

namespace restify {

    Error::Error(StatusCode code)
    :std::runtime_error("cpp-restify error"), _details(Json::objectValue)
    {
        _details["statusCode"] = (int)code;
    }
    
    Error::Error(StatusCode code, const char *message)
    :std::runtime_error(message), _details(Json::objectValue)
    {
        _details["statusCode"] = (int)code;
        _details["message"] = message;
    }
    
    Error::Error(StatusCode code, const char *message, int line, const char *file)
    : Error(code, message)
    {
        _details["lineNumber"] = line;
        _details["fileName"] = file;
    }
}
