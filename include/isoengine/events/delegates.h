#ifndef DELEGATES_H
#define DELEGATES_H

#include <functional>
#include <vector>

template <class T>
class Delegates
{
};

template <class R, class... Args>
class Delegates<R(Args...)>
{
    using Func = std::function<R(Args...)>;

public:
    void raise(Args... args)
    {
        for (auto & handler : handlers) {
            handler(args...);
        }
    }

    void operator()(Args... args)
    {
        raise(args...);
    }

    Delegates & operator+=(const Func & f)
    {
        handlers.push_back(f);
        return *this;
    }

private:
    std::vector<Func> handlers;
};

#endif // DELEGATES_H