#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/render/drawable.h"
#include <map>
#include <memory>
#include <vector>

namespace iso
{

class RenderScene;

class SceneNode : public Drawable
{
    friend class RenderScene;

public:
    void addChild(std::shared_ptr<SceneNode> child);

private:
    virtual void drawCurrent(Window & window) const {}
    virtual void draw(Window & window) const;

    SceneNode * parent = nullptr;
    std::vector<std::shared_ptr<SceneNode>> children;
};

class RenderScene
{
public:
    RenderScene(std::initializer_list<HashedString> layerNames);

    void draw(Window & window);
    SceneNode & getLayer(HashedString name);
    SceneNode & topLayer();

private:
    std::vector<SceneNode> layers;
    std::map<HashedString, SceneNode *> layerRefs;
};

} // namespace iso

#endif
