#include "isoengine/render/scene_node.h"
#include "isoengine/math/isometric.h"

#include <numeric>
#include "isoengine/support/resource_manager.h"

namespace iso::render
{

void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    children.push_back(child);
    child->parent = this;
}

void SceneNode::removeChild(const SceneNode * child)
{
    for (auto iter = children.begin(); iter != children.end(); ++iter) {
        if (iter->get() == child) {
            iter->get()->parent = nullptr;
            children.erase(iter);
            break;
        }
    }
}

void SceneNode::draw(render::Window & window, math::Transform transform, bool presort) const
{
    transform *= getTransform();
    drawCurrent(window, transform);

    if (presort) {
        std::vector<int> indices(children.size());
        std::iota(indices.begin(), indices.end(), 0);

        std::sort(indices.begin(), indices.end(), [this](int a, int b) {
            auto posA = toIso(children[a]->getPosition());
            auto posB = toIso(children[b]->getPosition());
            // TODO: refactor tile size!!
            if (int(posA.y / 64) < int(posB.y / 64))
                return true;
            if (int(posA.y / 64) == int(posB.y / 64))
                return posA.x < posB.x;
            return false;
        });

        for (auto idx : indices) {
            children[idx]->draw(window, transform);
        }
    } else {
        for (auto& child : children) {
            child->draw(window, transform);
        }
    }
}

} // namespace iso
