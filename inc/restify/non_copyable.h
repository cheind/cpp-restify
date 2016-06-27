/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_NON_COPYABLE
#define CPP_RESTIFY_NON_COPYABLE

#include <restify/interface.h>

namespace restify {

    class CPPRESTIFY_INTERFACE NonCopyable {
    public:
        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator=(NonCopyable const&) = delete;
        inline NonCopyable() {}
    };

}

#endif