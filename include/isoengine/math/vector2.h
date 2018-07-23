#ifndef VECTOR2_H
#define VECTOR2_H

namespace iso::math
{
template <typename T>
class Vector2
{
public:
    T x;
    T y;

    Vector2() : x(T{}), y(T{}) {}

    Vector2(T x, T y) : x{x}, y{y} {}

    template <typename U>
    Vector2(const Vector2<U> & other) : x{other.x}, y{other.y} {}

    inline Vector2<T> operator+(const Vector2<T> & rhs)
    {
        return {x + rhs.x, y + rhs.y};
    }

    inline Vector2<T> operator-(const Vector2<T> & rhs)
    {
        return {x - rhs.x, y - rhs.y};
    }

    inline Vector2<T> operator*(T mul)
    {
        return {x * mul, y * mul};
    }

    inline Vector2<T> operator/(T div)
    {
        return {x / div, y / div};
    }

    inline Vector2<T> & operator+=(const Vector2<T> & rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    inline Vector2<T> & operator-=(const Vector2<T> & rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline Vector2<T> & operator*=(T mul)
    {
        x *= mul;
        y *= mul;
        return *this;
    }

    inline Vector2<T> & operator/=(T div)
    {
        x /= div;
        y /= div;
        return *this;
    }

    friend inline Vector2<T> & operator+(const Vector2<T> & left, const Vector2<T> & right)
    {
        return {left.x + right.x, left.y + right.y};
    }

    friend inline Vector2<T> & operator-(const Vector2<T> & left, const Vector2<T> & right)
    {
        return {left.x - right.x, left.y - right.y};
    }

    friend inline bool operator==(const Vector2<T> & lhs, const Vector2<T> & rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend inline bool operator!=(const Vector2<T> & lhs, const Vector2<T> & rhs)
    {
        return lhs.x != rhs.x || lhs.y != rhs.y;
    }

    // Cast to allow narrowing explicitly
    template <typename U, typename V>
    static inline Vector2<U> cast(const Vector2<V> & vector)
    {
        return Vector2<U>(static_cast<U>(vector.x), static_cast<U>(vector.y));
    }
};

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;
using Vector2l = Vector2<long>;
using Vector2ul = Vector2<unsigned long>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

} // namespace iso::math

#endif
