#include "isoengine/physics/collision_detector.h"
#include "isoengine/highlevel/gameobject.h"

namespace iso
{

void CollisionDetector::addRigidBody(std::shared_ptr<GameObject> object)
{
    rigidBodies.push_back(object);
}

void CollisionDetector::removeRigidBody(const GameObject * object)
{
    for (auto iter = rigidBodies.begin(); iter != rigidBodies.end(); ++iter) {
        if (iter->get() == object) {
            rigidBodies.erase(iter);
            break;
        }
    }
}

bool CollisionDetector::checkCollisions(GameObject * o, const math::Vector2f & pos)
{
    float oLeft = pos.x + o->boundingBox.x;
    float oRight = pos.x + o->boundingBox.x + o->boundingBox.width;
    float oTop = pos.y + o->boundingBox.y;
    float oBottom = pos.y + o->boundingBox.y + o->boundingBox.height;

    if (oLeft < globalBounds.x ||
        oRight > globalBounds.x + globalBounds.width ||
        oTop < globalBounds.y ||
        oBottom > globalBounds.y + globalBounds.height) {

        if (o->collide()) {
            return true;
        }
    }

    for (const auto & x : rigidBodies) {
        if (x.get() != o) {
            float xLeft = x->getPosition().x + x->boundingBox.x;
            float xRight = x->getPosition().x + x->boundingBox.x + x->boundingBox.width;
            float xTop = x->getPosition().y + x->boundingBox.y;
            float xBottom = x->getPosition().y + x->boundingBox.y + x->boundingBox.height;

            if (oRight > xLeft && oLeft < xRight && oBottom > xTop && oTop < xBottom) {
                if (o->collide(x.get())) {
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace iso
