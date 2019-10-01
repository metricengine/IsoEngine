#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/render/animation.h"
#include "isoengine/render/font.h"
#include "isoengine/render/texture.h"
#include <memory>
#include <unordered_map>

namespace iso::support
{

template <typename T>
using Ptr = std::shared_ptr<T>;

class ResourceManager
{
public:
    static ResourceManager & getInstance();
    Texture & getTexture(const HashedString & filename);
    Font & getFont(const HashedString & filename);
    void addAnimation(const HashedString & name, Animation animation);
    Ptr<Animation> getAnimation(const HashedString & name);

private:
    ResourceManager() = default;
    std::unordered_map<size_t, Texture> textures;
    std::unordered_map<size_t, Font> fonts;
    std::unordered_map<size_t, Ptr<Animation>> animations;
};

} // namespace iso

#endif
