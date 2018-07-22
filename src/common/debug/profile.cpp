#include "isoengine/common/debug/profile.h"
#include "isoengine/common/consoletable.h"
#include <array>
#include <set>
#include <sstream>

namespace
{
std::string durationToString(const Duration & duration)
{
    std::ostringstream oss;
    auto durMicro = duration.count();
    // 1 - 1000 us
    if (durMicro < 1000)
        oss << durMicro << " us";
    // 1 - 1000 ms
    else if (durMicro < 1000 * 1000)
        oss << (long double)durMicro / 1000 << " ms";
    // 1 - 60 s
    else if (durMicro < 60 * 1000 * 1000)
        oss << (long double)durMicro / (1000 * 1000) << " s";
    // 1 - 60 m
    else if (durMicro < 60l * 60 * 1000 * 1000)
        oss << (long double)durMicro / (60 * 1000 * 1000) << " m";
    return oss.str();
}
} // namespace

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

    std::cout << "Global time duration (ms): " << durationToString(globalDuration) << std::endl;

    ConsoleTable table{"Name", "Count", "Average", "Min", "Max", "Total", "%"};
    table.reserve(resultByValue.size());

    for (auto iter : resultByValue) {
        std::vector<std::string> row;
        row.reserve(table.headerSize());
        row.push_back(iter.first);
        row.push_back(std::to_string(iter.second.count));
        row.push_back(durationToString(iter.second.total / iter.second.count));
        row.push_back(durationToString(iter.second.min));
        row.push_back(durationToString(iter.second.max));
        row.push_back(durationToString(iter.second.total));
        row.push_back(std::to_string(static_cast<long double>(iter.second.total.count()) / globalDuration.count() * 100));
        table.addRow(row);
    }

    table.display(std::cout);
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
        if (globalBegin == TimePoint{} || profiler.begin < globalBegin) {
            globalBegin = profiler.begin;
        }
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