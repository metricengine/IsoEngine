#ifndef PROFILE_H
#define PROFILE_H

#define ENABLE_PROFILE

#ifdef ENABLE_PROFILE

#define STRINGIFY_(a) #a
#define STRINGIFY(a) STRINGIFY_(a)

#define PROFILE_FUNCTION Profiler __profiler##__COUNTER__(__PRETTY_FUNCTION__)
#define PROFILE_BLOCK(NAME) Profiler __profiler##__COUNTER__(NAME)

#include "profile_impl.h"

#else

#define PROFILE_FUNCTION
#define PROFILE_BLOCK(NAME)

#endif // ENABLE_PROFILE

#endif // PROFILE_H