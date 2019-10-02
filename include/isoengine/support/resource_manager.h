#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "isoengine/support/hashed_string.h"
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
    iso::render::Texture & getTexture(const HashedString & filename);
    iso::render::Font & getFont(const HashedString & filename);
    void addAnimation(const HashedString & name, iso::render::Animation animation);
    Ptr<iso::render::Animation> getAnimation(const HashedString & name);

private:
    ResourceManager() = default;
    std::unordered_map<size_t, iso::render::Texture> textures;
    std::unordered_map<size_t, iso::render::Font> fonts;
    std::unordered_map<size_t, Ptr<iso::render::Animation>> animations;
};

} // namespace iso

#endif
