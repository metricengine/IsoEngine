#ifndef PROFILE_IMPL_H
#define PROFILE_IMPL_H

#include <chrono>
#include <iostream>
#include <map>
#include <string.h>

using Duration = std::chrono::microseconds;

class Profiler;

class ProfileManager
{
    friend Profiler;

    struct ProfileItem {
        ProfileItem(int count, Duration duration) : count(count), duration(duration) {}
        int count;
        Duration duration;
    };
    struct CompareCStr {
        inline bool operator()(const char * first, const char * second)
        {
            return strcmp(first, second) < 0;
        }
    };
    using ResultMap = std::map<const char *, ProfileItem, CompareCStr>;

public:
    static ProfileManager & getInstance();
    void showResult();
    const ResultMap getResult() const { return result; }

    ProfileManager(const ProfileManager &) = delete;
    void operator=(const ProfileManager &) = delete;

private:
    ResultMap result;

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
    std::chrono::steady_clock::time_point begin;
    Duration duration;
};

#endif