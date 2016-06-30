/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/request_reader.h>
#include <restify/request.h>
#include <restify/connection.h>
#include <restify/error.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <regex>

#include "mongoose.h"

namespace restify {

    void DefaultRequestBodyReader::readRequestBody(Connection & c, Request & request) const {
        Json::Value &root = request.toJson();

        // See if Content-Length is provided.
        if (json_cast<int>(request.getHeader("Content-Length", 0)) <= 0) {
            root[Request::Keys::body] = "";
            return;
        }

        std::ostringstream oss;
        if (c.readStream(oss) < 0) {
            throw Error(StatusCode::BadRequest, "Message transfer not complete.");
        }

        const static std::regex isContentJsonRegex(R"(/json)", std::regex::icase);

        std::string contentType = json_cast<std::string>(request.getHeader("Content-Type", ""));

        if (std::regex_search(contentType.begin(), contentType.end(), isContentJsonRegex)) {

            std::istringstream iss(oss.str());

            Json::CharReaderBuilder b;
            std::string errs;

            root[Request::Keys::body] = Json::Value(Json::objectValue);

            if (!Json::parseFromStream(b, iss, &root[Request::Keys::body], &errs)) {
                throw Error(StatusCode::BadRequest, errs.c_str());
            }

        } else {
            root[Request::Keys::body] = oss.str();
        }
    }

}
