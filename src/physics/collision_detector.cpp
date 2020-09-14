#include "isoengine/physics/collision_detector.h"
#include "isoengine/game_object.h"
#include "isoengine/math/isometric.h"

namespace iso::physics
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
    float oLeft = pos.x + o->boundingBox.left();
    float oRight = pos.x + o->boundingBox.right();
    float oTop = pos.y + o->boundingBox.top();
    float oBottom = pos.y + o->boundingBox.bottom();

    // if ((globalBounds != math::Rectf{}) &&
    //     (oLeft < globalBounds.x ||
    //      oRight > globalBounds.x + globalBounds.width ||
    //      oTop < globalBounds.y ||
    //      oBottom > globalBounds.y + globalBounds.height)) {

    //     if (o->collide()) {
    //         return true;
    //     }
    // }

    if (usingBoard) {
        auto boardPos = getBoardPosition(pos);

        // std::cout << "boardPos.x " << boardPos.x << "\n";
        // std::cout << "boardPos.y " << boardPos.y << "\n";

        if (collisionBoard[boardPos.y * boardSize.x + boardPos.x]) {
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

void CollisionDetector::setBoard(bool useBoard, math::Vector2i size, const math::Vector2i & tile)
{
    usingBoard = useBoard;

    if (useBoard) {
        boardSize = size;
        tileSize = tile;

        if (collisionBoard)
            delete[] collisionBoard;

        collisionBoard = new bool[size.x * size.y];
        std::fill(collisionBoard, collisionBoard + (size.x * size.y), false);
    }
}

void CollisionDetector::setTileRigidness(bool rigid, size_t row, size_t col)
{
    collisionBoard[row * boardSize.x + col] = rigid;
}

math::Vector2i CollisionDetector::getBoardPosition(const math::Vector2f & screenPos)
{
    auto worldPos = toIso(screenPos);

    auto x = int(worldPos.x / tileSize.x);
    auto y = int(worldPos.y / tileSize.y);

    return { x, y };
}

} // namespace iso
