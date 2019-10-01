#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "scenenode.h"
#include <map>

namespace iso
{

class RenderScene
{
public:
    RenderScene(std::initializer_list<HashedString> layerNames);

    void draw(render::Window & window);
    SceneNode & getLayer(HashedString name);
    SceneNode & topLayer();

private:
    std::vector<SceneNode> layers;
    std::map<HashedString, SceneNode *> layerRefs;
};

} // namespace iso

#endif
