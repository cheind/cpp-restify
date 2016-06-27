/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/response.h>

namespace restify {

    struct Response::PrivateData {

        PrivateData() {
        }
    };
         

    Response::Response()
        :_data(new PrivateData())
    {
    }

    Response::~Response()
    {
    }

}
