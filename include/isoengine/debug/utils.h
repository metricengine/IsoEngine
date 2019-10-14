#ifndef UTILS_H
#define UTILS_H

#define _DEBUG
#define LOG_LEVEL 5
#define PROFILE_ENABLED

#include <sstream>

#include "isoengine/debug/assert.h"
#include "isoengine/debug/profile.h"

#define PROFILE_FUNCTION _PROFILE_FUNCTION
#define PROFILE_BLOCK(name) _PROFILE_BLOCK(name)
#define DEBUG_LOG(msg, level) _DEBUG_LOG(msg, level)
#define ASSERT(expr, msg) _ASSERT(expr, msg)
#define UNUSED(x) _UNUSED(x)

template <typename... Args>
std::string stringify(Args const &... args)
{
    std::ostringstream oss;
    int a[] = {0, ((void)(oss << args), 0)...};
    // To suppress "unused" warning
    (void)(a);
    return oss.str();
}

// Macro implementations

#ifdef _DEBUG

#define _UNUSED(x)       \
    do {                 \
        (void)sizeof(x); \
    } while (0)

// Debug information
#define _DEBUG_LOG(msg, level)             \
    do {                                   \
        if (level <= LOG_LEVEL)            \
            std::cerr << msg << std::endl; \
    } while (0)

#else

#define _UNUSED(x)
#define _DEBUG_LOG(msg, level)

#endif // _DEBUG

#endif // UTILS_H
