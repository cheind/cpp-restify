/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/http_renderer.h>
#include <restify/response.h>
#include <restify/error.h>
#include <json/json.h>

namespace restify {
    
#define EOL "\r\n"
    
    HttpRenderer::HttpRenderer()
    {}
    
    std::string HttpRenderer::renderResponse(const Response &rep) const {
        
        const Json::Value &jroot = rep.toJson();
        
        
        std::ostringstream http;
        
        // Render body first.
        
        const Json::Value jbody = jroot.get("body", "");
        std::string body;
        
        switch (jbody.type()) {
            case Json::stringValue:
                body = jbody.asString();
            case Json::objectValue:
            {
                Json::FastWriter w;
                w.omitEndingLineFeed();
                w.dropNullPlaceholders(); // should do this?
                body = w.write(jbody);
            }
            default:
                CPPRESTIFY_FAIL(StatusCode::InternalServerError, "Failed to render body.");
        }
        
        const int contentLength = jroot["headers"].get("Content-Length", static_cast<int>(body.length())).asInt();
        
        
        return http.str();
        
    }
    
    std::string HttpRenderer::renderError(const Error &err) const {
        return "";
        
    }
    
}
