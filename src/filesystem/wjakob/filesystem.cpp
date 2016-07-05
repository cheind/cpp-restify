/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/filesystem/filesystem.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#include "restify_build_config.h"

namespace restify {

    namespace fs = filesystem;

    Path::Type Path::getType(const std::string &path) {
        const fs::path p(path);
        if (p.is_file()) return Path::Type::File;
        else if (p.is_directory()) return Path::Type::Directory;
        else return Path::Type::NotFound;
    }

    std::string Path::makeAbsolute(const std::string &path) {
        return fs::path(path).make_absolute().str();
    }

    std::string Path::join(const std::string &a, const std::string &b) {
        return (fs::path(a) / b).str();
    }
}

