/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_CONNECTION_H
#define CPP_RESTIFY_CONNECTION_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <json/json-forwards.h>
#include <iosfwd>
#include <cstdint>

namespace restify {

    class CPPRESTIFY_INTERFACE Connection {
    public:
        virtual int64_t readStream(std::ostream &stream) = 0;
        virtual int64_t writeStream(std::istream &stream) = 0;
        virtual void closeConnection() = 0;
    };
}

#endif