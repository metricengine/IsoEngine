#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "isoengine/render/scene_node.h"
#include <map>

namespace iso::render
{

class RenderScene
{
public:
    RenderScene(std::initializer_list<support::HashedString> layerNames);

    void draw(render::Window & window);
    SceneNode & getLayer(support::HashedString name);
    SceneNode & topLayer();

    void setBoard(const math::Vector2i & size);
    SceneNode & getBoardCell(int row, int col);

private:
    std::vector<SceneNode> layers;
    std::map<support::HashedString, SceneNode *> layerRefs;

    // TODO: refactor
    // Board
    SceneNode* sceneBoard = nullptr;
    bool useBoard = false;
    math::Vector2i boardSize;
};

} // namespace iso

#endif
