#ifndef HASH_H
#define HASH_H

#include <cstdlib>

namespace iso::math
{

constexpr size_t calculateHash(const char * str)
{
    size_t result = 0;
    const size_t prime = 31;
    for (; *str != '\0'; ++str) {
        result = *str + (result * prime);
    }
    return result;
}

} // namespace iso::math

#endif // HASH_H