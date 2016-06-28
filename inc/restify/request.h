/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_REQUEST_H
#define CPP_RESTIFY_REQUEST_H

#include <restify/interface.h>
#include <json/json.h>

namespace restify {

    class CPPRESTIFY_INTERFACE Request
    {
    public:
        struct Keys {
            static constexpr const char *params = "params";
            static constexpr const char *headers = "headers";
            static constexpr const char *method = "method";
            static constexpr const char *path = "path";
            static constexpr const char *query = "query";
            static constexpr const char *body = "body";
        };

        Request();
        Request(const Json::Value &opts);
        ~Request();

        /** Return the HTTP method. */
        std::string getMethod() const;

        /** Return the URI decoded path.*/
        std::string getPath() const;
        
        /** Return the URI decoded query string.*/
        std::string getQueryString() const;

        /** Return the message body. */
        Json::Value getBody() const;

        /** Return immutable reference to query parameters. */
        const Json::Value &getParams() const;

        /** Return immutable reference to a specific query parameter. */
        const Json::Value &getParam(const std::string &key) const;

        /** Return immutable reference to a specific query parameter. */
        const Json::Value getParam(const std::string &key, const Json::Value &defaultValue) const;

        /** Return immutable reference to HTTP headers.*/
        const Json::Value &getHeaders() const;

        /** Return immutable reference to a specific header parameter. */
        const Json::Value &getHeader(const std::string &key) const;

        /** Return immutable reference to a specific header parameter. */
        const Json::Value getHeader(const std::string &key, const Json::Value &defaultValue) const;

        /** Convert request to JSON */
        const Json::Value &toJson() const;

        /** Convert request to JSON */
        Json::Value &toJson();

    private:
        CPPRESTIFY_NO_INTERFACE_WARN(Json::Value, _root);
    };

}

#endif