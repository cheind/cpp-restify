/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_ERROR_H
#define CPP_RESTIFY_ERROR_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json.h>
#include <restify/codes.h>
#include <stdexcept>

namespace restify {

#ifdef CPPRESTIFY_WIN
    #pragma warning (push)
    #pragma warning (disable : 4275)
#endif

    class CPPRESTIFY_INTERFACE Error : public std::runtime_error
    {
    public:

        Error(StatusCode code);
        Error(const Response &response);
        Error(StatusCode code, const char *message);
        Error(StatusCode code, const char *message, int line, const char *file);
        Error(const Error &other);
        ~Error();

        const Json::Value &toJson() const;

    private:
        CPPRESTIFY_NO_INTERFACE_WARN(Json::Value, _details);
    };
    
#ifdef CPPRESTIFY_WIN
    #pragma warning (pop)
#endif

    
#define CPPRESTIFY_FAIL(code, message) \
    throw ::restify::Error(code, message, __LINE__, __FILE__);
 
}

#endif