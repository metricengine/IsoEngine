#ifndef HASHEDSTRING_H
#define HASHEDSTRING_H

#include "isoengine/math/hash.h"
#include <cstdlib>

namespace iso::support
{

template <size_t N>
constexpr size_t HASH(const char (&str)[N])
{
    return math::calculateHash(str);
}

class HashedString
{
    struct StringWrapper {
        inline StringWrapper(const char * str) : str(str) {}
        const char * str;
    };

public:
    HashedString(StringWrapper sw) : hash(math::calculateHash(sw.str)), rawStr(sw.str) {}

    template <size_t N>
    constexpr HashedString(const char (&str)[N]) : hash(HASH(str)), rawStr(str) {}

    constexpr size_t getHash() const
    {
        return hash;
    }

    const char * getCStr() const
    {
        return rawStr;
    }

    inline bool operator==(const HashedString & other) const
    {
        return hash == other.hash;
    }

    inline bool operator!=(const HashedString & other) const
    {
        return hash != other.hash;
    }

    constexpr operator size_t() const
    {
        return hash;
    }

private:
    const size_t hash;
    const char * rawStr;
};

} // namespace iso::support

#endif // HASHEDSTRING_H
