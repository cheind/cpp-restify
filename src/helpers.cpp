/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/helpers.h>
#include <json/json.h>

namespace restify {

    bool jsonMerge(Json::Value & a, const Json::Value & b) {

        if (!a.isObject() || !b.isObject())
            return false;

        bool ok = true;
        for (const auto& key : b.getMemberNames()) {
            if (a[key].isObject()) {
                ok &= jsonMerge(a[key], b[key]);
            } else {
                a[key] = b[key];
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
    
    std::vector<std::string> splitString(const std::string &str, const char delim, bool trim) {
        
        std::istringstream split(str);
        
        std::vector<std::string> tokens;
        std::string elem;
        while(std::getline(split, elem, delim)) {
            
            if (trim)
                elem = trimString(elem);
            
            tokens.push_back(elem);
        }
        return tokens;
    }

    JsonBuilder::JsonBuilder()
        :_root(Json::objectValue)
    {}

    JsonBuilder & JsonBuilder::set(const std::string & path, const Json::Value & value) {
        Json::Path(path).make(_root) = value;
        return *this;
    }

    const Json::Value & JsonBuilder::toJson() const {
        return _root;
    }



}
