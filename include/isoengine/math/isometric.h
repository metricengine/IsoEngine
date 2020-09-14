#ifndef ISOMETRIC_H
#define ISOMETRIC_H

#include "isoengine/math/vector2.h"

namespace iso::math {

Vector2f toIso(const Vector2f & v);
Vector2f fromIso(const Vector2f & v);

} // namespace iso::math

#endif
