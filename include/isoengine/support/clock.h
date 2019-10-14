#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

namespace iso::support
{

template <typename Duration = std::chrono::seconds>
class Clock
{
    using ClockType = typename std::conditional<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
    using TimePoint = std::chrono::time_point<ClockType>;

public:
    Clock()
    {
        lastUpdateTime = ClockType::now();
    }
    typename Duration::rep restart()
    {
        auto dt = ClockType::now() - lastUpdateTime;
        lastUpdateTime = ClockType::now();
        return std::chrono::duration_cast<Duration>(dt).count();
    }

private:
    TimePoint lastUpdateTime;
};

} // namespace iso

#endif // CLOCK_H
