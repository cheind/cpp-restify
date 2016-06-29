/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_HTTP_REQUEST_READER_H
#define CPP_RESTIFY_HTTP_REQUEST_READER_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>
#include <memory>


struct mg_request_info;

namespace restify {

    class CPPRESTIFY_INTERFACE HttpRequestReader {
    public:
        virtual void readRequestHeader(Connection &c, Request &r) const = 0;
        virtual void readRequestBody(Connection &c, Request &r) const = 0;
    };
    
    class CPPRESTIFY_INTERFACE DefaultHttpRequestReader : public HttpRequestReader {
    public:
        virtual void readRequestBody(Connection & c, Request & r) const override;
    private:
    };

    class CPPRESTIFY_INTERFACE MongooseHttpRequestReader : public DefaultHttpRequestReader {
    public:
        virtual void readRequestHeader(Connection & c, Request & r) const override;
    private:

        void readHeaders(const struct mg_request_info *info, Request &request) const;
        void readMethod(const struct mg_request_info *info, Request &request) const;
        void readPath(const struct mg_request_info *info, Request &request) const;
        void readQueryString(const struct mg_request_info *info, Request &request) const;
    };
}

#endif