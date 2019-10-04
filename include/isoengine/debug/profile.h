#ifndef PROFILE_IMPL_H
#define PROFILE_IMPL_H

#include <chrono>
#include <iostream>
#include <map>
#include <string.h>

#ifdef PROFILE_ENABLED
#define _PROFILE_FUNCTION Profiler __profiler_##__FUNCTION__(__PRETTY_FUNCTION__)
#define _PROFILE_BLOCK(NAME) Profiler __profiler##__COUNTER__(NAME)
#else
#define _PROFILE_FUNCTION
#define _PROFILE_BLOCK(NAME)
#endif

using Duration = std::chrono::microseconds;
using ClockType = typename std::conditional<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
using TimePoint = std::chrono::time_point<ClockType>;

class Profiler;

class ProfileManager
{
    friend Profiler;

    struct ProfileItem {
        ProfileItem(int count = 0, Duration duration = {})
            : count(count), total(duration), min(duration), max(duration) {}
        int count;
        Duration total;
        Duration min;
        Duration max;
    };
    struct CompareCStr {
        inline bool operator()(const char * first, const char * second) const
        {
            return strcmp(first, second) < 0;
        }
    };
    using ResultMap = std::map<const char *, ProfileItem, CompareCStr>;

public:
    static ProfileManager & getInstance();
    void start();
    void stop();
    void showResult();
    const ResultMap getResult() const { return result; }

    ProfileManager(const ProfileManager &) = delete;
    void operator=(const ProfileManager &) = delete;

private:
    ResultMap result;
    TimePoint globalBegin, globalEnd;

    ProfileManager() {}
    void addItem(const Profiler & profiler);
};

class Profiler
{
    friend ProfileManager;

public:
    Profiler(const char * name);
    ~Profiler();

private:
    const char * name;
    TimePoint begin;
    Duration duration;
};

#endif