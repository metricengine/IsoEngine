#ifndef SCENENODE_H
#define SCENENODE_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/render/window.h"
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
    void removeChild(const SceneNode * child);

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

template <typename T>
class SceneNodeObject : public SceneNode
{
public:
    SceneNodeObject() : SceneNode()
    {
    }

    T & getObject()
    {
        return object;
    }

    const T & getObject() const
    {
        return object;
    }

private:
    void drawCurrent(Window & window, math::Transform transform) const override
    {
        window.draw(object, transform);
    }

    T object;
};

} // namespace iso

#endif // SCENENODE_H
