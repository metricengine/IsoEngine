#include "isoengine/math/transform.h"
#include <cmath>

namespace iso::math
{

Transform::Transform() : matrix(Matrix<float, 3, 3>::Identity())
{
}

Transform::Transform(const std::array<float, 9> values) : matrix(values)
{
}

Transform & Transform::translate(float x, float y)
{
    Matrix<float, 3, 3> translation({1, 0, x,
                                     0, 1, y,
                                     0, 0, 1});
    matrix *= translation;
    return *this;
}

Transform & Transform::translate(const Vector2f & t)
{
    return translate(t.x, t.y);
}

Transform & Transform::rotate(float angle)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Matrix<float, 3, 3> rotation({cos, -sin, 0,
                                  sin, cos, 0,
                                  0, 0, 1});
    matrix *= rotation;
    return *this;
}

Transform & Transform::scale(float sx, float sy)
{
    Matrix<float, 3, 3> scaling({sx, 0, 0,
                                 0, sy, 0,
                                 0, 0, 1});
    matrix *= scaling;
    return *this;
}

Transform & Transform::scale(const Vector2f & s)
{
    return scale(s.x, s.y);
}

Vector2f Transform::transformPoint(float x, float y)
{
    return {matrix(0, 0) * x + matrix(0, 1) * y + matrix(0, 2),
            matrix(1, 0) * x + matrix(1, 1) * y + matrix(1, 2)};
}

Vector2f Transform::transformPoint(const Vector2f & p)
{
    return transformPoint(p.x, p.y);
}

Transform & Transform::operator*=(const Transform & transform)
{
    matrix *= transform.getMatrix();
    return *this;
}

} // namespace iso::math
