#include "isoengine/render/scene_node.h"

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

void SceneNode::draw(render::Window & window, math::Transform transform) const
{
    transform *= getTransform();
    drawCurrent(window, transform);
    for (auto child : children) {
        child->draw(window, transform);
    }
}

} // namespace iso
