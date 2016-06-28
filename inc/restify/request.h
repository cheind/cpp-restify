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

        Request();
        ~Request();

        /** Return the HTTP method. */
        std::string method() const;

        /** Set the HTTP method. */
        Request &method(const std::string &method);

        /** Return the URI decoded path.*/
        std::string path() const;

        /** Set the URI decoded path. */
        Request &path(const std::string &path);
        
        /** Return the URI decoded query string.*/
        std::string queryString() const;
        
        /** Set the URI decoded query string. Note this updates the params() field. */
        Request &queryString(const std::string &path);

        /** Return immutable reference to query parameters. */
        const Json::Value &params() const;

        /** Return immutable reference to a specific query parameter. */
        const Json::Value &param(const std::string &key) const;

        /** Return immutable reference to a specific query parameter. */
        const Json::Value param(const std::string &key, const Json::Value &defaultValue) const;

        /** Return mutable reference to query parameters. */
        Json::Value &params();

        /** Return immutable reference to HTTP headers.*/
        const Json::Value &headers() const;

        /** Return mutable reference to HTTP headers. */
        Json::Value &headers();

        /** Return immutable reference to a specific header. */
        const Json::Value &setHeader(const std::string &key) const;

        /** Return immutable reference to a specific header. */
        const Json::Value setHeader(const std::string &key, const Json::Value &defaultValue) const;

        /** Convert request to JSON */
        const Json::Value &toJson() const;

    private:
        CPPRESTIFY_NO_INTERFACE_WARN(Json::Value, _root);
    };

}

#endif