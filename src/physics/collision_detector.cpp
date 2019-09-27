#include "isoengine/physics/collision_detector.h"
#include "isoengine/highlevel/gameobject.h"

#include <iostream>

namespace iso
{

void CollisionDetector::addRigidBody(std::shared_ptr<GameObject> object)
{
    rigidBodies.push_back(object);
}

bool CollisionDetector::checkCollisions(GameObject * o, const math::Vector2f & pos)
{
    for (const auto & x : rigidBodies) {
        if (x.get() != o) {
            auto & oSprite = o->getSprite();
            auto & xSprite = x->getSprite();

            float oLeft = pos.x;
            float oRight = pos.x + oSprite.getSize().x;
            float oTop = pos.y;
            float oBottom = pos.y + oSprite.getSize().y;
            float xLeft = x->getPosition().x;
            float xRight = x->getPosition().x + xSprite.getSize().x;
            float xTop = x->getPosition().y;
            float xBottom = x->getPosition().y + xSprite.getSize().y;

            if (oRight > xLeft && oLeft < xRight && oBottom > xTop && oTop < xBottom) {
                if (o->collide(*x.get())) {
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace iso
