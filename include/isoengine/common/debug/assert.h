#ifndef ASSERT_IMPL_H
#define ASSERT_IMPL_H

#include <exception>
#include <string>

#ifdef _DEBUG

// Assert
#define _ASSERT(expr, msg)                                       \
    do {                                                         \
        if (!(expr)) {                                           \
            throw AssertFailure(#expr, __FILE__, __LINE__, msg); \
        }                                                        \
    } while (0)

#else
#define _ASSERT(expr, msg)
#endif

// AssertFailutre exception
class AssertFailure : public std::exception
{
private:
    std::string report;

public:
    AssertFailure(const char * expression, const char * file, int line, const char * message);
    inline const char * what() const throw()
    {
        return report.c_str();
    }
};

#endif