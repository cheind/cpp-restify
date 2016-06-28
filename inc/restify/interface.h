/**
    This file is part of cpp-restify.

    Copyright(C) 2016 Christoph Heindl
    All rights reserved.

    This software may be modified and distributed under the terms
    of MIT license. See the LICENSE file for details.
*/

#ifndef CPP_RESTIFY_INTERFACE_H
#define CPP_RESTIFY_INTERFACE_H

#if defined(_WIN32) || defined(__WIN32__)
    
    #define CPPRESTIFY_WIN

    #if defined(cpp_restify_EXPORTS)
        #define CPPRESTIFY_INTERFACE __declspec(dllexport)
    #else
        #define CPPRESTIFY_INTERFACE __declspec(dllimport)
    #endif

    #define CPPRESTIFY_NO_INTERFACE_WARN(type, name) \
        __pragma(warning(push)) \
        __pragma(warning(disable : 4251)) \
        type name; \
        __pragma(warning(pop))

#else
    #define CPPRESTIFY_INTERFACE
    
    #define CPPRESTIFY_NO_INTERFACE_WARN(type, name) \
        type name;

#endif


#endif
