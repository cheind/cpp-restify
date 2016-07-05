/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/response_writer.h>
#include <restify/connection.h>
#include <restify/response.h>
#include <restify/error.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <iostream>

#define EOL "\r\n"

namespace restify {
    
    
    void DefaultResponseWriter::writeResponse(restify::Connection &c, restify::Response &r) const
    {
        const std::string message = renderMessage(r.toJson());
        
        std::istringstream iss(message);

        if (c.writeStream(iss) != message.length()) {
            throw Error(StatusCode::BadRequest, "Message transfer not complete.");
        }
    }
    
    std::string DefaultResponseWriter::renderMessage(const Json::Value &jroot) const
    {
        std::ostringstream http;
        
        Json::Value headers(Json::objectValue);
        
        const std::string body = renderBody(jroot, headers);
        
        // Replace generated headers by headers set in response.
        jsonMerge(headers, jroot["headers"]);
        
        // Status line
        int setCode = json_cast<int>(jroot.get("statusCode", 200));
        http << "HTTP/" << jroot.get("version", "1.1").asString() << " "
        << setCode << " "
        << reasonPhraseFromStatusCode(setCode) << EOL;
        
        // Headers
        for (const auto& key : headers.getMemberNames()) {
            http << key << ": " << headers[key].asString() << EOL;
        }
        http << EOL;
        
        // Body
        http << body;
        
        return http.str();
    }
    
    std::string DefaultResponseWriter::renderBody(const Json::Value &jroot, Json::Value & generatedHeaders)  const {
        const Json::Value jbody = jroot.get("body", "");
        
        std::string body;
        
        switch (jbody.type()) {
            case Json::stringValue:
                body = jbody.asString();
                generatedHeaders["Content-Type"] = "text/plain; charset=utf-8";
                generatedHeaders["Content-Length"] = (int)body.length();
                break;
            case Json::objectValue:
            {
                Json::FastWriter w;
                w.omitEndingLineFeed();
                body = w.write(jbody);
                generatedHeaders["Content-Type"] = "application/json; charset=utf-8";
                generatedHeaders["Content-Length"] = (int)body.length();
                break;
            }
            default:
                CPPRESTIFY_FAIL(StatusCode::InternalServerError, "Failed to render body.");
        }

        return body;
        
    }
    
    std::string DefaultResponseWriter::reasonPhraseFromStatusCode(int setCode) const {
        
        if (setCode >= 100 && setCode < 200)
            return "Informational";
        else if (setCode >= 200 && setCode < 300)
            return "Success";
        else if (setCode >= 300 && setCode < 400)
            return "Redirection";
        else if (setCode >= 400 && setCode < 500)
            return "Client Error";
        else if (setCode >= 500)
            return "Server Error";
        else
            return "Unknown";
    }


}
