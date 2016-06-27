/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_RESPONSE_H
#define CPP_RESTIFY_RESPONSE_H

#include <restify/interface.h>
#include <restify/codes.h>
#include <restify/non_copyable.h>
#include <json/json-forwards.h>
#include <memory>

namespace restify {

    class CPPRESTIFY_INTERFACE Response : NonCopyable 
    {
    public:

        Response();
        ~Response();
        
        Response &code(int code);
        Response &body(const Json::Value &value);
        Response &header(const std::string &key, const Json::Value &value);
        Response &version(const std::string &value);
        
        const Json::Value &toJson() const;
        

    private:
        struct PrivateData;
        CPPRESTIFY_NO_INTERFACE_WARN(std::unique_ptr<PrivateData>, _data);
    };

}

#endif