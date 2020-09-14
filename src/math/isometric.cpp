#include "isoengine/math/isometric.h"

namespace iso::math {

Vector2f fromIso(const Vector2f & v) {
    auto x = v.x - v.y;
    auto y = (v.x + v.y) / 2;
    return { x, y };
}

Vector2f toIso(const Vector2f & v) {
    auto x = (2 * v.y + v.x) / 2;
    auto y = (2 * v.y - v.x) / 2;
    return { x, y };
};

} // namespace iso::math
