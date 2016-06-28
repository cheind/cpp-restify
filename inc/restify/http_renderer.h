/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_HTTP_RENDERER_H
#define CPP_RESTIFY_HTTP_RENDERER_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>
#include <memory>

namespace restify {

    class CPPRESTIFY_INTERFACE HttpRenderer
    {
    public:

        HttpRenderer();
        
        std::string renderResponse(const Response &rep) const;
        std::string renderError(const Error &err) const;

    private:
        std::string renderMessage(const Json::Value &jroot) const;
        std::string renderBody(const Json::Value &jroot, Json::Value &generatedHeaders) const;
        std::string reasonPhraseFromStatusCode(int setCode) const;
    };

}

#endif