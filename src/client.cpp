/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/client.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <regex>

#include <curl/curl.h>

namespace restify {


    struct InMemoryStruct {
        char *memory;
        size_t size;
    };

    static size_t writeCallbackCURL(void *contents, size_t size, size_t nmemb, void *userp) {
        size_t realsize = size * nmemb;
        struct InMemoryStruct *mem = (struct InMemoryStruct *)userp;

        mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
        if (mem->memory == NULL) {
            /* out of memory! */
            printf("not enough memory (realloc returned NULL)\n");
            return 0;
        }

        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;

        return realsize;
    }

    static size_t headerCallbackCURL(void *contents, size_t size, size_t nitems, void *userp) {
        size_t realsize = size * nitems;

        const char *buffer = (const char*)contents;
        std::vector<std::string> parts = restify::splitString(std::string(buffer, buffer + realsize), ':', true, true);

        if (parts.size() == 2 && parts.front().size() > 0 && parts.back().size() > 0) {
            Json::Value &headers = *(static_cast<Json::Value*>(userp));
            headers[parts.front()] = parts.back();
        }

        return realsize;
    }

    template<class Type>
    Type getCurlInfo(CURL *curl, CURLINFO info) {
        Type t;
        curl_easy_getinfo(curl, info, &t);
        return t;
    }

    template<>
    std::string getCurlInfo<std::string>(CURL *curl, CURLINFO info) {
        const char *str;
        curl_easy_getinfo(curl, info, &str);

        if (str) {
            return std::string(str);
        } else {
            return std::string();
        }
    }

    Json::Value Client::invoke(const Json::Value &req) {
        CURL *curl;
        curl = curl_easy_init();
        struct curl_slist *curlheaders = nullptr;

        // Setup options

        const bool verbose = restify::json_cast<bool>(req.get("verbose", false));
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1 : 0);

        const std::string url = restify::json_cast<std::string>(req.get("url", "http://127.0.0.1:8080"));
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        const std::string method = restify::json_cast<std::string>(req.get("method", "GET"));
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

        const int timeout = restify::json_cast<int>(req.get("timeout", 500));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);

        Json::Value jsonbody = req.get("body", "");
        const std::string body = restify::json_cast<std::string>(req.get("body", ""));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (int)body.size());

        const bool followRedirects = restify::json_cast<bool>(req.get("followRedirects", false));
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, followRedirects ? 1L : 0L);

        // Set headers
        bool hasContentTypeHeaderExplicit = false;
        for (auto h : req["headers"].getMemberNames()) {
            hasContentTypeHeaderExplicit |= (h == "Content-Type");

            std::ostringstream oss;
            oss << h << ": " << req["headers"][h].asString();

            curlheaders = curl_slist_append(curlheaders, oss.str().c_str());
        }

        if (!hasContentTypeHeaderExplicit && jsonbody.isObject())
            curlheaders = curl_slist_append(curlheaders, "Content-Type: application/json; charset=utf-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlheaders);

        Json::Value response(Json::objectValue);

        // Setup body fetcher
        InMemoryStruct responseBody;
        responseBody.memory = (char*)malloc(1);
        responseBody.size = 0;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackCURL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&responseBody);

        // Setup header fetcher
        Json::Value responseHeaders(Json::objectValue);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackCURL);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&responseHeaders);

        CURLcode res = curl_easy_perform(curl);
        response["error"] = (res == CURLE_OK) ? false : true;
        response["success"] = (res == CURLE_OK) ? true : false;
        response["code"] = (int)res;
        response["headers"] = responseHeaders;
        response["codeReason"] = curl_easy_strerror(res);
        

        if (res == CURLE_OK) {
            response["statusCode"] = getCurlInfo<int>(curl, CURLINFO_RESPONSE_CODE);
            std::string contentType = getCurlInfo<std::string>(curl, CURLINFO_CONTENT_TYPE);

            const static std::regex isJson(R"(/json)", std::regex::icase);
            std::string responseBodyStr = std::string(responseBody.memory, responseBody.memory + responseBody.size);

            if (std::regex_search(contentType.begin(), contentType.end(), isJson)) {
                std::istringstream iss(responseBodyStr);
                iss >> response["body"];
            } else {
                response["body"] = responseBodyStr;
            }
        }

        
        free(responseBody.memory);
        curl_slist_free_all(curlheaders);
        curl_easy_cleanup(curl);
        
        return response;
    }

}
