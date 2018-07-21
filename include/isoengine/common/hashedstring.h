#ifndef HASHEDSTRING_H
#define HASHEDSTRING_H

#include "isoengine/math/hash.h"
#include <cstdlib>

namespace iso
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
    HashedString(StringWrapper sw)
    {
        hash = math::calculateHash(sw.str);
    }

    template <size_t N>
    constexpr HashedString(const char (&str)[N]) : rawStr(str)
    {
        hash = HASH(str);
    }

    constexpr size_t getHash() const { return hash; }

    const char * getCStr() const { return rawStr; }

    inline bool operator==(const HashedString & other)
    {
        return hash == other.hash;
    }

    inline bool operator!=(const HashedString & other)
    {
        return hash != other.hash;
    }

private:
    size_t hash;
    const char * rawStr;
};

} // namespace iso

#endif // HASHEDSTRING_H
