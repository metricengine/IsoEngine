#ifndef VECTOR2_H
#define VECTOR2_H

namespace iso::math
{
class Vector2
{
public:
    long x;
    long y;

    Vector2(long x, long y) : x(x), y(y) {}

    Vector2 operator/(long div)
    {
        return {x / div, y / div};
    }
};
} // namespace iso::math

#endif
