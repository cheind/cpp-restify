/**
This file is part of cpp-restify.

Copyright(C) 2016 Christoph Heindl
All rights reserved.

This software may be modified and distributed under the terms
of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_SERVER_FIXTURE_H
#define CPP_RESTIFY_SERVER_FIXTURE_H

#include <restify/server.h>

class ServerFixture {
protected:
    restify::Server _server;
};

#endif