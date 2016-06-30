/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_MONGOOSE_CONNECTION_H
#define CPP_RESTIFY_MONGOOSE_CONNECTION_H

#include <restify/interface.h>
#include <restify/forward.h>
#include <restify/connection.h>
#include <iosfwd>
#include <cstdint>

struct mg_connection;
struct mg_request_info;

namespace restify {

    class CPPRESTIFY_INTERFACE MongooseConnection : public Connection {
    public:
        MongooseConnection(struct mg_connection *conn);

        virtual int64_t readStream(std::ostream & stream) override;
        virtual int64_t writeStream(std::istream &stream) override;
        virtual void closeConnection() override;
        
        
        const struct mg_request_info *getMongooseRequestInfo() const;

    private:
        struct mg_connection *_conn;
    };
}

#endif