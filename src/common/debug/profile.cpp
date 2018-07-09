#include "isoengine/common/debug/profile.h"

#ifdef ENABLE_PROFILE

void ProfileManager::showResult()
{
    for (auto iter : result) {
        std::cout << iter.first << " count: " << iter.second.count;
        std::cout << " total duration (ms): " << iter.second.duration.count() << std::endl;
    }
}

ProfileManager & ProfileManager::getInstance()
{
    static ProfileManager instance;
    return instance;
}

void ProfileManager::addItem(const Profiler & profiler)
{
    auto iter = result.lower_bound(profiler.name);
    if (iter != result.end()) {
        ++iter->second.count;
        iter->second.duration += profiler.duration;
    } else {
        ProfileItem item(1, profiler.duration);
        result.insert(iter, std::make_pair(profiler.name, item));
    }
}

Profiler::Profiler(const char * name) : name(name)
{
    begin = std::chrono::steady_clock::now();
}

Profiler::~Profiler()
{
    auto end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    ProfileManager::getInstance().addItem(*this);
}

#endif