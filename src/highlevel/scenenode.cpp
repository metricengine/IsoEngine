#include "isoengine/highlevel/scenenode.h"

namespace iso
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

void SceneNode::draw(Window & window, math::Transform transform) const
{
    transform *= getTransform();
    drawCurrent(window, transform);
    for (auto child : children) {
        child->draw(window, transform);
    }
}

} // namespace iso
