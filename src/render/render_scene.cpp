#include "isoengine/render/render_scene.h"
#include "isoengine/debug/utils.h"

namespace iso::render
{

math::Transform SceneNode::getTransform() const
{
    float tx = position.x;
    float ty = position.y;

    return math::Transform({1, 0, tx,
                            0, 1, ty,
                            0, 0, 1});
}

void RenderScene::draw(render::Window & window)
{
    math::Transform transform;

    // if (useBoard) {
    //     // for (int i = 0; i < boardSize.y; ++i) {
    //     //     for (int j = 0; j < boardSize.x; ++j) {
    //     //         sceneBoard[i * boardSize.x + j].draw(window, transform);
    //     //     }
    //     // }
    // } else {
    // }
    for (const auto & layer : layers) {
        layer.draw(window, transform, useBoard);
    }
}

RenderScene::RenderScene(std::initializer_list<support::HashedString> layerNames)
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

SceneNode & RenderScene::getLayer(support::HashedString name)
{
    auto iter = layerRefs.find(name);
    if (iter == layerRefs.end()) {
        throw std::invalid_argument(stringify("layer not found: ", name.getCStr()));
    }
    return *iter->second;
}

SceneNode & RenderScene::topLayer()
{
    return layers.back();
}

void RenderScene::setBoard(const math::Vector2i & size)
{
    useBoard = true;
    boardSize = size;

    if (sceneBoard != nullptr) {
        delete[] sceneBoard;
    }

    sceneBoard = new SceneNode[size.x * size.y];
}

SceneNode & RenderScene::getBoardCell(int row, int col)
{
    if (!useBoard) {
        throw std::runtime_error("RenderScene::getBoardCell: board is not initailized");
    }

    return sceneBoard[row * boardSize.x + col];
}

} // namespace iso
