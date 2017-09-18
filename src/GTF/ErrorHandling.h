//
//  ErrorHandling.h
//  gtf
//
//  Created by David Gallardo on 01/06/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <stdio.h>

#if defined(DEBUG)
    /*
     * GTF_DEBUG_BREAK - Breaks the execution of code and jumps to the debugger.
     */
    #if defined(_MSC_VER)
        #define GTF_DEBUG_BREAK()       asm{ int 3 }
    #elif defined(__GNUC__)
        #if defined(__APPLE__)
            #define GTF_DEBUG_BREAK()   __builtin_trap()
        #else
            #define GTF_DEBUG_BREAK()   abort()
        #endif
    #endif

    /*
     * GTF_ASSERT - Evaluate the expression and abort the execution if false.
     */
    #define GTF_ASSERT(expr, msg) \
    if(expr){} \
    else { \
        printf("Assertion evaluating (%s) \"%s\" in %s(%d)\n", #expr, msg, __FILE__, __LINE__); \
        GTF_DEBUG_BREAK(); \
    }
#else
    #define GTF_ASSERT(expr, msg)
#endif
