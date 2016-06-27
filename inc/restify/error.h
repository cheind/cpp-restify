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
#include <json/json.h>
#include <restify/codes.h>
#include <stdexcept>

namespace restify {

    class CPPRESTIFY_INTERFACE Error : public std::runtime_error
    {
    public:

        Error(StatusCode code);
        Error(StatusCode code, const char *message);
        Error(StatusCode code, const char *message, int line, const char *file);

    private:
        Json::Value _details;
    };
    
    
#define CPPRESTIFY_FAIL(code, message) \
    throw Error(code, message, __LINE__, __FILE__);
 
}

#endif