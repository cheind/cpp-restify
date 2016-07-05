/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_FILEYSTEM_H
#define CPP_RESTIFY_FILEYSTEM_H


#include <restify/interface.h>
#include <restify/forward.h>
#include <string>

namespace restify {


    class CPPRESTIFY_INTERFACE Path {
    public:
        enum class Type {
            NotFound,
            File,
            Directory
        };

        static Type getType(const std::string &path);
        static std::string makeAbsolute(const std::string &path);
        static std::string join(const std::string &a, const std::string &b);
    };
 
}

#endif