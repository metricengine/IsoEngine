#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "isoengine/math/vector2.h"
#include <memory>
#include <vector>

namespace iso
{

class GameObject;

class CollisionDetector
{
public:
    void addRigidBody(std::shared_ptr<GameObject> object);
    bool checkCollisions(GameObject * object, const math::Vector2f & pos);

private:
    std::vector<std::shared_ptr<GameObject>> rigidBodies;
};

} // namespace iso

#endif // COLLISION_DETECTOR_H
