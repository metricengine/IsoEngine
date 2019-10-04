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

private:
    std::vector<SceneNode> layers;
    std::map<support::HashedString, SceneNode *> layerRefs;
};

} // namespace iso

#endif
