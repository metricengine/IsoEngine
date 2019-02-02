#include "isoengine/highlevel/renderscene.h"
#include "isoengine/common/utils.h"

namespace iso
{

void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    children.push_back(child);
    child->parent = this;
}

void SceneNode::draw(Window & window, math::Transform transform) const
{
    transform *= getTransform();
    drawCurrent(window, transform);
    for (auto child : children) {
        child->draw(window, transform);
    }
}

math::Transform SceneNode::getTransform() const
{
    float tx = position.x;
    float ty = position.y;

    return math::Transform({1, 0, tx,
                            0, 1, ty,
                            0, 0, 1});
}

void RenderScene::draw(Window & window)
{
    math::Transform transform;
    for (const auto & layer : layers) {
        layer.draw(window, transform);
    }
}

RenderScene::RenderScene(std::initializer_list<HashedString> layerNames)
{
    layers.reserve(layerNames.size() == 0 ? 1 : layerNames.size());
    for (auto layerName : layerNames) {
        layers.push_back(SceneNode());
        layerRefs.insert({layerName, &layers.back()});
    }
    if (layerNames.size() == 0) {
        layers.push_back(SceneNode());
    }
}

SceneNode & RenderScene::getLayer(HashedString name)
{
    auto iter = layerRefs.find(name);
    if (iter == layerRefs.end()) {
        throw std::invalid_argument(stringify("layer not found: ", name.getCStr()));
    }
    return *iter->second;
}

SceneNode & RenderScene::topLayer()
{
    return layers[0];
}

} // namespace iso
