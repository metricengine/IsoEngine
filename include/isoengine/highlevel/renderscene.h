#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/render/window.h"
#include <map>
#include <memory>
#include <vector>

namespace iso
{

class RenderScene;

class SceneNode : public sf::Transformable
{
    friend class RenderScene;
    friend class Window;

public:
    virtual ~SceneNode() {}
    void addChild(std::shared_ptr<SceneNode> child);

    std::vector<std::shared_ptr<SceneNode>> children;

private:
    virtual void drawCurrent(Window & window, sf::Transform transform) const {}
    void draw(Window & window, sf::Transform transform) const;

    SceneNode * parent = nullptr;
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
