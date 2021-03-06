/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/mongoose/mongoose_request_reader.h>
#include <restify/mongoose/mongoose_connection.h>
#include <restify/request.h>
#include <restify/error.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <regex>

#include "mongoose.h"

namespace restify {

      
    void MongooseRequestHeaderReader::readRequestHeader(Connection &c, Request & request) const {
        
        try {
            MongooseConnection &mc = dynamic_cast<MongooseConnection&>(c);
            
            const mg_request_info * info = mc.getMongooseRequestInfo();
            
            readMethod(info, request);
            readPath(info, request);
            readHeaders(info, request);
            readQueryString(info, request);
            
        } catch (std::bad_cast) {
            CPPRESTIFY_FAIL(StatusCode::InternalServerError, "Expected MongooseConnection.");
        }
    }

    void MongooseRequestHeaderReader::readMethod(const mg_request_info * info, Request & request) const {
        Json::Value &root = request.toJson();
        root[Request::Keys::method] = info->request_method;
    }

    void MongooseRequestHeaderReader::readPath(const mg_request_info * info, Request & request) const {
        Json::Value &root = request.toJson();
        root[Request::Keys::path] = info->uri;
    }

    void MongooseRequestHeaderReader::readHeaders(const mg_request_info * info, Request & request) const {
        Json::Value &root = request.toJson();
        Json::Value &headers = root[Request::Keys::headers];
        for (int i = 0; i < info->num_headers; ++i) {
            headers[info->http_headers[i].name] = info->http_headers[i].value;
        }
    }

    void MongooseRequestHeaderReader::readQueryString(const mg_request_info * info, Request & request) const {
        Json::Value &root = request.toJson();

        if (info->query_string) {

            // Decode query string.
            const int bufferSize = 2048;
            char buffer[bufferSize];
            mg_url_decode(info->query_string, (int)strlen(info->query_string), buffer, bufferSize, 0);
            const std::string decodedQueryString(buffer);

            // Update request params.
            Json::Value &getParams = root[Request::Keys::params];
            std::vector<std::string> pairs = splitString(decodedQueryString, '&', false, false);
            for (auto p : pairs) {
                std::vector<std::string> keyval = splitString(p, '=', true, false);

                if (keyval.size() != 2 || keyval.front().empty() || keyval.back().empty()) {
                    CPPRESTIFY_FAIL(StatusCode::BadRequest, "Query string is malformed.");
                }

                getParams[keyval[0]] = keyval[1];
            }

            // Set query string
            root[Request::Keys::query] = decodedQueryString;
        }
    }


}
