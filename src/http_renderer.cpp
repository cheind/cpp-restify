/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/http_renderer.h>
#include <json/json.h>

namespace restify {
    
#define EOL "\r\n"
    
    HttpRenderer::HttpRenderer()
    {}
    
    std::string HttpRenderer::renderResponse(const Response &rep) const {
        
        return "";
        
    }
    
    std::string HttpRenderer::renderError(const Error &err) const {
        return "";
        
    }
    
}
