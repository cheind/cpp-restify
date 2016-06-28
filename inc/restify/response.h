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
#include <restify/helpers.h>
#include <json/json.h>

namespace restify {

    class CPPRESTIFY_INTERFACE Response 
    {
    public:

        Response();
        ~Response();
        
        Response &setCode(int setCode);
        Response &setBody(const Json::Value &value);
        Response &setHeader(const std::string &key, const Json::Value &value);
        Response &setVersion(const std::string &value);
        Response &setRedirectTo(const std::string &location, int code = (int)StatusCode::Moved);

        class CPPRESTIFY_INTERFACE JsonBodyBuilder {
        public:
            JsonBodyBuilder(Response &response);
            JsonBodyBuilder &set(const std::string &path, const Json::Value &value);
            Response &endBody();
        private:
            Response &_response;
            JsonBuilder _builder;
        };

        JsonBodyBuilder beginBody();
        
        const Json::Value &toJson() const;
        

    private:

        friend class JsonBodyBuilder;

        CPPRESTIFY_NO_INTERFACE_WARN(Json::Value, _root);
    };

}

#endif