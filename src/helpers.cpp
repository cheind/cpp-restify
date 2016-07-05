/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/helpers.h>
#include <restify/request.h>
#include <restify/response.h>
#include <json/json.h>
#include <algorithm>

namespace restify {

    inline void updateFieldIfNot(Json::Value & a, const Json::Value & b, const std::string &key, int condition) {
        if (condition == 0) {
            a[key] = b[key];
        }
    }

    bool jsonMerge(Json::Value & a, const Json::Value & b, int ignoreFlags) {

        if (!a.isObject() || !b.isObject()) {
            return false;
        }
            
        // For each key in b
        bool ok = true;
        for (const auto& key : b.getMemberNames()) {

            if (a[key].isObject() && b[key].isObject()) {
                // Both are objects, recurse.
                ok &= jsonMerge(a[key], b[key], ignoreFlags);
            } else {
                // Either of both is not object.
                if (a[key].type() == b[key].type()) {
                    // Both have the same type
                    updateFieldIfNot(a, b, key, ignoreFlags & JsonMergeFlags::IgnoreNewValues);
                } else {
                    // Type of both is different
                    if (a[key].isNull()) {
                        // field is not present in a
                        updateFieldIfNot(a, b, key, ignoreFlags & JsonMergeFlags::IgnoreNewFields);
                    } else {
                        // field is already present in a (we also know from above that they cannot be of same type.
                        updateFieldIfNot(a, b, key, ignoreFlags & JsonMergeFlags::IgnoreNewType);
                    }
                }
            }
        }

        return ok;
    }
    
    std::string trimString(const std::string &str, const std::string &ws) {

        const auto strBegin = str.find_first_not_of(ws);
        if (strBegin == std::string::npos)
            return std::string();
        
        const auto strEnd = str.find_last_not_of(ws);
        const auto strRange = strEnd - strBegin + 1;
        
        return str.substr(strBegin, strRange);
    }

    std::string removeLineBreaksInString(const std::string & str, const std::string & lb) {
        std::string result = str;
        std::string::size_type pos = 0;
        while ((pos = result.find(lb, pos)) != std::string::npos) {
            result.erase(pos, 2);
        }
        result.shrink_to_fit();
        return result;
    }
    
    std::vector<std::string> splitString(const std::string &str, const char delim, bool trim, bool removeLineBreaks) {
        
        std::istringstream split(str);
        
        std::vector<std::string> tokens;
        std::string elem;
        while(std::getline(split, elem, delim)) {
            
            if (trim)
                elem = trimString(elem);
            if (removeLineBreaks)
                elem = removeLineBreaksInString(elem);
            
            tokens.push_back(elem);
        }
        return tokens;
    }

    std::string toLowerCase(const std::string & str) {

        std::string result;
        result.resize(str.size());

        std::transform(str.begin(),
                       str.end(),
                       result.begin(),
                       ::tolower);

        return result;
    }

    JsonBuilder::JsonBuilder()
        :_root(new Json::Value(), JsonBuilder::defaultDelete)
    {}

    JsonBuilder::JsonBuilder(Json::Value & adapt) 
        :_root(&adapt, JsonBuilder::nullDelete)
    {}

    JsonBuilder::JsonBuilder(std::istream &textStream)
        : _root(new Json::Value(), JsonBuilder::defaultDelete)
    {
        Json::CharReaderBuilder b;
        std::string errs;

        if (!Json::parseFromStream(b, textStream, _root.get(), &errs)) {
            throw Error(StatusCode::InternalServerError, errs.c_str());
        }
    }
    
    JsonBuilder & JsonBuilder::set(const std::string & key, const Json::Value & value) {
        Json::Path(key).make(*_root) = value;
        return *this;
    }

    JsonBuilder & JsonBuilder::set(const std::string & key1, const std::string & key2, const Json::Value & value) {
        Json::Path(key1 + "." + key2).make(*_root) = value;
        return *this;
    }

    JsonBuilder &JsonBuilder::operator()(const std::string & key, const Json::Value & value) {
        return set(key, value);
        return *this;
    }

    JsonBuilder &JsonBuilder::operator()(const std::string & key1, const std::string & key2, const Json::Value & value) {
        return set(key1, key2, value);
        return *this;
    }

    JsonBuilder & JsonBuilder::mergeFrom(const Json::Value & object, int mergeFlags) {
        if (!jsonMerge(*_root, object, mergeFlags))
            CPPRESTIFY_FAIL(StatusCode::InternalServerError, "Failed to merge.");
        return *this;
    }

    JsonBuilder & JsonBuilder::mergeFrom(const Json::Value & object, int mergeFlags, const std::nothrow_t & tag) {
        jsonMerge(*_root, object, mergeFlags);
        return *this;
    }

    JsonBuilder::operator const Json::Value&() const {
        return *_root;
    }

    const Json::Value & JsonBuilder::toJson() const {
        return *_root;
    }

    void JsonBuilder::nullDelete(Json::Value * val) {

    }

    void JsonBuilder::defaultDelete(Json::Value * val) {
        if (val)
            delete val;
    }

    JsonBuilder json() {
        return JsonBuilder();
    }

    JsonBuilder json(Json::Value &adaptTo) {
        return JsonBuilder(adaptTo);
    }

    JsonBuilder json(const std::string & jsonStr) {
        std::istringstream iss(jsonStr);
        return JsonBuilder(iss);
    }

    JsonBuilder json(std::istream & is) {
        return JsonBuilder(is);
    }

     JsonBuilder json(Request & request) {
        return JsonBuilder(request.toJson());
    }

     JsonBuilder json(Response & response) {
        return JsonBuilder(response.toJson());
    }


}
