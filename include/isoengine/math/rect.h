#ifndef RECT_H
#define RECT_H

#include "vector2.h"

namespace iso::math
{

template <typename T>
class Rect
{
public:
    T x, y, width, height;

    Rect() : x(T{}), y(T{}), width(T{}), height(T{}) {}
    Rect(const Vector2<T> & topLeft, const Vector2<T> & size)
        : x{topLeft.x}, y{topLeft.y}, width{size.x}, height{size.y} {}
    Rect(T x, T y, T width, T height)
        : x(x), y(y), width(width), height(height) {}
};

using Recti = Rect<int>;
using Rectu = Rect<unsigned>;
using Rectl = Rect<long>;
using Rectul = Rect<unsigned long>;
using Rectf = Rect<float>;
using Rectd = Rect<double>;

} // namespace iso::math

#endif // RECT_H
