#include "isoengine/common/debug/profile.h"
#include <array>
#include <set>

void ProfileManager::start()
{
    globalBegin = ClockType::now();
}

void ProfileManager::stop()
{
    globalEnd = ClockType::now();
}

void ProfileManager::showResult()
{
    using ValType = ResultMap::value_type;

    if (globalEnd == TimePoint{})
        globalEnd = ClockType::now();

    Duration globalDuration{std::chrono::duration_cast<Duration>(ClockType::now() - globalBegin)};

    auto cmp = [](const ValType & a, const ValType & b) { return a.second.total > b.second.total; };
    std::set<ValType, decltype(cmp)> resultByValue(result.begin(), result.end(), cmp);

    std::array<std::string, 7> header = {"Name", "Count", "Average", "Min", "Max", "Total", "%"};
    std::array<int, header.size()> colLengths;
    for (size_t i = 0; i < colLengths.size(); ++i)
        colLengths[i] = header[i].size();

    std::cout << "Global time duration (ms): " << globalDuration.count() << std::endl;

    for (auto iter : resultByValue) {
        std::cout << iter.first << " count: " << iter.second.count;
        std::cout << " average: " << iter.second.total.count() / iter.second.count;
        std::cout << " min: " << iter.second.min.count();
        std::cout << " max: " << iter.second.max.count();
        std::cout << " total: " << iter.second.total.count();
        std::cout << " %: " << iter.second.total.count() / globalDuration.count() << std::endl;
    }
}

ProfileManager & ProfileManager::getInstance()
{
    static ProfileManager instance;
    return instance;
}

void ProfileManager::addItem(const Profiler & profiler)
{
    auto iter = result.find(profiler.name);
    if (iter != result.end()) {
        ++iter->second.count;
        iter->second.total += profiler.duration;
        if (profiler.duration > iter->second.max)
            iter->second.max = profiler.duration;
        else if (profiler.duration < iter->second.min)
            iter->second.min = profiler.duration;
    } else {
        ProfileItem item(1, profiler.duration);
        result.insert(std::make_pair(profiler.name, item));
        // If start was not used, implicitly start tracking
        if (globalBegin == TimePoint{})
            globalBegin = profiler.begin;
    }
}

Profiler::Profiler(const char * name) : name(name)
{
    begin = ClockType::now();
}

Profiler::~Profiler()
{
    auto end = ClockType::now();
    duration = std::chrono::duration_cast<Duration>(end - begin);
    ProfileManager::getInstance().addItem(*this);
}