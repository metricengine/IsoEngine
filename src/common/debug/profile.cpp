#include "isoengine/common/debug/profile.h"
#include "isoengine/common/consoletable.h"
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

    std::cout << "Global time duration (ms): " << globalDuration.count() << std::endl;

    ConsoleTable table{"Name", "Count", "Average", "Min", "Max", "Total", "%"};
    table.reserve(resultByValue.size());

    for (auto iter : resultByValue) {
        std::vector<std::string> row;
        row.reserve(table.headerSize());
        row.push_back(iter.first);
        row.push_back(std::to_string(iter.second.count));
        row.push_back(std::to_string(static_cast<long double>(iter.second.total.count() / iter.second.count)));
        row.push_back(std::to_string(iter.second.min.count()));
        row.push_back(std::to_string(iter.second.max.count()));
        row.push_back(std::to_string(iter.second.total.count()));
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