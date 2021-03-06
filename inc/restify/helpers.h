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
#include <restify/non_copyable.h>
#include <json/json.h>
#include <vector>
#include <string>
#include <memory>

namespace restify {

    struct JsonMergeFlags {
        static constexpr int IgnoreNewFields = 1 << 1;
        static constexpr int IgnoreNewType = 1 << 2;
        static constexpr int IgnoreNewValues = 1 << 3;

        static constexpr int MergeEverything = 0;
        static constexpr int MergeNewElements = IgnoreNewType | IgnoreNewValues;
        static constexpr int MergeExistingElements = IgnoreNewFields;
        static constexpr int MergeExistingElementsWithSameType = IgnoreNewFields | IgnoreNewType;
    };

    CPPRESTIFY_INTERFACE
    bool jsonMerge(Json::Value& a, const Json::Value& b, int flags = JsonMergeFlags::MergeEverything);
    
    CPPRESTIFY_INTERFACE
    std::string trimString(const std::string &str, const std::string &ws = " \t");

    CPPRESTIFY_INTERFACE
    std::string removeLineBreaksInString(const std::string &str, const std::string &lb = "\r\n");
    
    CPPRESTIFY_INTERFACE
    std::vector<std::string> splitString(const std::string &str, const char delim, bool trim, bool removeLineBreaks);

    CPPRESTIFY_INTERFACE
    std::string toLowerCase(const std::string &str);

    
    // Explicit Json conversion

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
                case Json::objectValue: {
                    Json::StyledWriter sw;
                    return sw.write(v);                    
                }

                    
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
        } catch (Json::Exception &) {}
        
        CPPRESTIFY_FAIL(StatusCode::BadRequest, "Cannot convert value to double");
    }


    class CPPRESTIFY_INTERFACE JsonBuilder {
    public:
        JsonBuilder();
        JsonBuilder(Json::Value &adapt);
        JsonBuilder(std::istream &textStream);

        JsonBuilder &set(const std::string &key, const Json::Value &value);
        JsonBuilder &set(const std::string &key1, const std::string &key2, const Json::Value &value);
        JsonBuilder &operator()(const std::string &key, const Json::Value &value);
        JsonBuilder &operator()(const std::string &key1, const std::string &key2, const Json::Value &value);
        JsonBuilder &mergeFrom(const Json::Value &object, int mergeFlags);
        JsonBuilder &mergeFrom(const Json::Value &object, int mergeFlags, const std::nothrow_t& tag);
        operator const Json::Value&() const;

        const Json::Value &toJson() const;

        static void nullDelete(Json::Value * val);
        static void defaultDelete(Json::Value * val);


    private:
        typedef std::shared_ptr<Json::Value> JsonPtr;
        CPPRESTIFY_NO_INTERFACE_WARN(JsonPtr, _root);
    };


    /** Return a new Json builder. */
    CPPRESTIFY_INTERFACE
    JsonBuilder json();

    /** Return a new Json builder. */
    CPPRESTIFY_INTERFACE
    JsonBuilder json(Json::Value &adaptTo);

    /** Return a new Json builder. */
    CPPRESTIFY_INTERFACE
    JsonBuilder json(const std::string &jsonStr);

    /** Return a new Json builder. */
    CPPRESTIFY_INTERFACE
    JsonBuilder json(std::istream &is);

    /** Return a new Json builder. */
    CPPRESTIFY_INTERFACE
    JsonBuilder json(Request &request);

    CPPRESTIFY_INTERFACE
    JsonBuilder json(Response &response);

}


#endif
