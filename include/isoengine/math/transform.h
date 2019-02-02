#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "vector2.h"

namespace iso::math
{

class Transform
{
public:
    Transform();
    Transform(const std::array<float, 9> values);
    Transform(const Transform &) = default;
    Transform & translate(float x, float y);
    Transform & translate(const Vector2f & t);
    Transform & rotate(float angle);
    Transform & scale(float sx, float sy);
    Transform & scale(const Vector2f & s);
    Vector2f transformPoint(float x, float y);
    Vector2f transformPoint(const Vector2f & p);
    Transform & operator*=(const Transform & transform);
    const Matrix<float, 3, 3> & getMatrix() const
    {
        return matrix;
    }

    static const Transform Identity;

private:
    Matrix<float, 3, 3> matrix;
};

} // namespace iso::math

#endif
