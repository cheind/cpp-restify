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

}
