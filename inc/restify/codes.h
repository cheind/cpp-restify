/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_CODES_H
#define CPP_RESTIFY_CODES_H


namespace restify {
    
    enum class StatusCode {
        Ok                  = 200,
        Created             = 201,
        Accepted            = 202,
        NoContent           = 204,
        
        Moved               = 301,
        SeeOther            = 303,
        
        
        BadRequest          = 400,
        Unauthorized        = 401,
        Forbidden           = 403,
        NotFound            = 404,
        MethodNotAllowed    = 405,
        NotAcceptable       = 406,
        
        
        InternalServerError = 500
        
    };


}

#endif