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

class SceneNode
{
    friend class RenderScene;
    friend class Window;

public:
    virtual ~SceneNode() {}
    void addChild(std::shared_ptr<SceneNode> child);

    const math::Vector2f & getPosition() const
    {
        return position;
    }

    void setPosition(const math::Vector2f & pos)
    {
        position = pos;
    }

private:
    virtual void drawCurrent(Window & window, math::Transform transform) const {}
    void draw(Window & window, math::Transform transform) const;
    math::Transform getTransform() const;

    SceneNode * parent = nullptr;
    std::vector<std::shared_ptr<SceneNode>> children;
    math::Vector2f position;
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
