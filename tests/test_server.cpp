/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#include "catch.hpp"

#include <restify/server.h>
#include <restify/request.h>
#include <restify/response.h>
#include <restify/helpers.h>
#include <json/json.h>
#include <future>
#include <chrono>
#include <iostream>

#include <curl/curl.h>

class ServerFixture {
public:
    
    void run() {
        _server.start();
    }
    
    void stop(){
        _server.stop();
    }
    
    ~ServerFixture() {
        _server.stop();
    }
protected:
    restify::Server _server;
};

class Client {
public:
    Client() {
        _curl = curl_easy_init();
        _response.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
        _response.size = 0;
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&_response);
    }

    
    ~Client() {
        if (_curl)
            curl_easy_cleanup(_curl);
        free(_response.memory);
    }
    
    bool perform() {
        _response.size = 0;
        CURLcode res = curl_easy_perform(_curl);
        
        return res == CURLE_OK;
    }
    
    void resetOptions() {
        curl_easy_reset(_curl);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&_response);
    }
    
    
    template<class ValueType>
    void setOption(CURLoption opt, ValueType value) {
        curl_easy_setopt(_curl, opt, value);
    }
    
    template<class ValueType>
    void getInfo(CURLINFO opt, ValueType &value) {
        curl_easy_getinfo(_curl, opt, &value);
    }
    
    template<class ValueType>
    ValueType getInfo(CURLINFO opt) {
        ValueType v;
        curl_easy_getinfo(_curl, opt, &v);
        return v;
    }
    
    std::string body() const {
        return std::string(_response.memory, _response.memory + _response.size);
    }
    
private:
    
    struct MemoryStruct {
        char *memory;
        size_t size;
    };
    
    static size_t
    writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        struct MemoryStruct *mem = (struct MemoryStruct *)userp;
        
        mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
        if(mem->memory == NULL) {
            /* out of memory! */
            printf("not enough memory (realloc returned NULL)\n");
            return 0;
        }
        
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
        
        return realsize;
    }
    
    CURL *_curl;
    MemoryStruct _response;
};

const int defaultRespondTimeoutMS = 500;

#define CPPRESTIFY_REQUIRE_RESPOND(future, timeout) \
    REQUIRE((future).wait_for(std::chrono::milliseconds(timeout)) == std::future_status::ready);

#define CPPRESTIFY_REQUIRE_TIMEOUT(future, timeout) \
    REQUIRE((future).wait_for(std::chrono::milliseconds(timeout)) == std::future_status::timeout);


TEST_CASE_METHOD(ServerFixture, "server-listen-on-multiple-addresses")
{
    _server.setConfig(
                     restify::json()
                     ("listening_ports", "127.0.0.1:8080, 127.0.0.1:8081")
    );
    _server.otherwise([](const restify::Request &req, restify::Response &rep) {
        rep.setCode(200).setBody("hello world");
        return true;
    });
    run();
    
    Client c;
    c.setOption(CURLOPT_URL, "http://127.0.0.1:8080");
    c.setOption(CURLOPT_TIMEOUT_MS, defaultRespondTimeoutMS);
    c.setOption(CURLOPT_HTTPGET, 1);
    REQUIRE(c.perform());
    REQUIRE(c.getInfo<int>(CURLINFO_RESPONSE_CODE) == 200);
    REQUIRE(std::string(c.getInfo<const char*>(CURLINFO_CONTENT_TYPE)) == "text/plain; charset=utf-8");
    REQUIRE(c.body() == "hello world");
    
    c.resetOptions();
    c.setOption(CURLOPT_URL, "http://127.0.0.1:8081");
    c.setOption(CURLOPT_TIMEOUT_MS, defaultRespondTimeoutMS);
    c.setOption(CURLOPT_HTTPGET, 1);
    REQUIRE(c.perform());
    REQUIRE(c.getInfo<int>(CURLINFO_RESPONSE_CODE) == 200);
    REQUIRE(std::string(c.getInfo<const char*>(CURLINFO_CONTENT_TYPE)) == "text/plain; charset=utf-8");
    REQUIRE(c.body() == "hello world");
}
