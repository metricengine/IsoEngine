#ifndef DELEGATES_H
#define DELEGATES_H

#include <functional>
#include <vector>

namespace iso::events
{

template <class T>
class Delegates
{
};

template <class R, class... Args>
class Delegates<R(Args...)>
{
    using Func = std::function<R(Args...)>;

public:
    template <typename... T>
    void raise(T &&... args)
    {
        for (auto & handler : handlers) {
            handler(std::forward<T>(args)...);
        }
    }

    template <typename... T>
    void operator()(T &&... args)
    {
        raise(std::forward<T>(args)...);
    }

    Delegates & operator+=(const Func & f)
    {
        handlers.push_back(f);
        return *this;
    }

private:
    std::vector<Func> handlers;
};

} // namespace iso::events

#endif // DELEGATES_H
