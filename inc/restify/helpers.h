/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_HELPERS_H
#define CPP_RESTIFY_HELPERS_H

#include <restify/interface.h>
#include <restify/error.h>
#include <json/json.h>
#include <string>

namespace restify {

    CPPRESTIFY_INTERFACE
    bool jsonMerge(Json::Value& a, const Json::Value& b);
    
    template <typename...>
    struct always_false { static constexpr bool value = false; };
    
    template<typename Type>
    Type json_cast(const Json::Value &v) {
        static_assert(
            always_false<Type>::value,
            "If your compiler traps here, it means that no overload is found for Type");
    }
    
    template<>
    inline std::string json_cast<std::string>(const Json::Value &v) {
        
        try {
            switch(v.type()) {
                case Json::nullValue:
                    return "null";
                case Json::intValue:
                    return Json::valueToString(v.asLargestInt());
                case Json::uintValue:
                    return Json::valueToString(v.asLargestUInt());
                case Json::realValue:
                    return Json::valueToString(v.asDouble());
                case Json::stringValue:
                    return v.asString();
                case Json::booleanValue:
                    return Json::valueToString(v.asBool());
                case Json::arrayValue:
                case Json::objectValue:
                    return v.toStyledString();
            }
        } catch (std::exception &) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to string");
        
    }
    
    template<>
    inline int json_cast<int>(const Json::Value &v) {
        try {
            switch(v.type()) {
                case Json::intValue:
                case Json::uintValue:
                case Json::realValue:
                case Json::booleanValue:
                case Json::nullValue:
                    return v.asInt();
                case Json::stringValue: {
                    size_t next = 0;
                    int val = std::stoi(v.asString(), &next);
                    if (next == v.asString().length())
                        return val;
                }
                default:
                    break;
            }
        } catch (std::exception &) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to int");
        
    }
    
    template<>
    inline bool json_cast<bool>(const Json::Value &v) {
        try {
            switch(v.type()) {
                case Json::intValue:
                case Json::uintValue:
                case Json::booleanValue:
                    return v.asBool();
                case Json::stringValue: {
                    const std::string str = v.asString();
                    if (str == "true") return true;
                    else if (str == "false") return false;
                }
                default:
                    break;
            }
        } catch (Json::Exception &) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to bool.");
        
    }
    
    template<>
    inline float json_cast<float>(const Json::Value &v) {
        try {
            switch(v.type()) {
                case Json::intValue:
                case Json::uintValue:
                case Json::booleanValue:
                case Json::realValue:
                case Json::nullValue:
                    return v.asFloat();
                case Json::stringValue: {
                    size_t next = 0;
                    float val = std::stof(v.asString(), &next);
                    if (next == v.asString().length())
                        return val;
                }
                default:
                    break;
            }
        } catch (std::exception &) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to float");
    }
    
    template<>
    inline double json_cast<double>(const Json::Value &v) {
        try {
            switch(v.type()) {
                case Json::intValue:
                case Json::uintValue:
                case Json::booleanValue:
                case Json::realValue:
                case Json::nullValue:
                    return v.asDouble();
                case Json::stringValue: {
                    size_t next = 0;
                    double val = std::stod(v.asString(), &next);
                    if (next == v.asString().length())
                        return val;
                }
                default:
                    break;
            }
        } catch (Json::Exception &e) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to double");
    }



    
}


#endif
