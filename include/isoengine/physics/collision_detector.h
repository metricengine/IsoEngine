#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "isoengine/math/rect.h"
#include <memory>
#include <vector>

namespace iso {
class GameObject;
}

namespace iso::physics
{

class CollisionDetector
{
public:
    CollisionDetector(math::Rectf globalBounds) : globalBounds(globalBounds)
    {
    }
    void addRigidBody(std::shared_ptr<GameObject> object);
    void removeRigidBody(const GameObject * object);
    bool checkCollisions(GameObject * object, const math::Vector2f & pos);
    void setBoard(bool useBoard, math::Vector2i boardSize = {}, const math::Vector2i & tileSize = {});
    void setTileRigidness(bool rigid, size_t col, size_t row);

private:
    math::Vector2i getBoardPosition(const math::Vector2f & worldPos);

    math::Rectf globalBounds;
    std::vector<std::shared_ptr<GameObject>> rigidBodies;

    bool usingBoard = false;
    math::Vector2i boardSize, tileSize;
    // TODO: could optimize or make more general (sides)
    bool* collisionBoard = nullptr;
};

} // namespace iso

#endif // COLLISION_DETECTOR_H
