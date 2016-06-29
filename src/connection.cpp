/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#include <restify/connection.h>
#include <ostream>
#include "mongoose.h"

namespace restify {


    MongooseConnection::MongooseConnection(mg_connection * conn)
        :_conn(conn)
    {}

    void MongooseConnection::read() {
    }

    const mg_request_info * MongooseConnection::getMongooseRequestInfo() const {
        return mg_get_request_info(_conn);
    }


    int64_t MongooseConnection::readStream(std::ostream & stream) {
        const int chunkSize = 2048;
        char chunk[chunkSize];

        int64_t total = 0;
        int read = 0;
        do {
            read = mg_read(_conn, chunk, chunkSize);
            if (read > 0) {
                total += read;
                stream.write(chunk, read);
            }
        } while (read > 0 && stream.good());

        if (read == -1 || !stream.good()) {
            return -1;
        } else {
            return total;
        }
    }
}
