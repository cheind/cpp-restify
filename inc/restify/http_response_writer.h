/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_HTTP_RESPONSE_WRITER_H
#define CPP_RESTIFY_HTTP_RESPONSE_WRITER_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>
#include <memory>

namespace restify {

    class CPPRESTIFY_INTERFACE HttpResponseWriter {
    public:
        virtual void writeResponse(Connection &c, Response &r) const = 0;
    };
    
    class CPPRESTIFY_INTERFACE DefaultHttpResponseWriter : public HttpResponseWriter {
    public:
        virtual void writeResponse(Connection &c, Response &r) const;
    private:
        virtual std::string renderMessage(const Json::Value &jroot) const;
        virtual std::string renderBody(const Json::Value &jroot, Json::Value &generatedHeaders) const;
        virtual std::string reasonPhraseFromStatusCode(int setCode) const;
    };
}

#endif