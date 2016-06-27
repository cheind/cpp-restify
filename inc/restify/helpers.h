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
#include <json/json-forwards.h>

namespace restify {

    CPPRESTIFY_INTERFACE
    bool jsonMerge(Json::Value& a, Json::Value& b);
}


#endif
