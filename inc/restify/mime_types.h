/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_MIME_TYPES_H
#define CPP_RESTIFY_MIME_TYPES_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>

namespace restify {

    class CPPRESTIFY_INTERFACE MimeTypes
    {
    public:

        static std::string resolveFromFileExtension(const std::string &ext, const std::string &defaultType = "application/octet-stream");

    };

}

#endif