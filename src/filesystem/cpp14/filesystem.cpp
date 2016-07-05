/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/filesystem/filesystem.h>
#include <experimental/filesystem>

#include "restify_build_config.h"

namespace restify {
    
    namespace fs = std::experimental::filesystem;

    Path::Type Path::typeOf(const std::string &path) 
    {
        const fs::path p(path);
        if (fs::is_regular_file(p)) return Path::Type::File;
        else if (fs::is_directory(p)) return Path::Type::Directory;
        else return Path::Type::NotFound;        
    }

    std::string Path::makeAbsolute(const std::string &path) {
        return fs::system_complete(fs::canonical(path)).string();
    }
       
    std::string Path::join(const std::string &a, const std::string &b) {
        return (fs::path(a) / b).string();
    }

    std::string Path::extension(const std::string &path) 
    {
        return fs::path(path).extension().string();
    }

    std::string Path::filename(const std::string &path) 
    {
        return fs::path(path).filename().string();
    }
}

