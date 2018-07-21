#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "isoengine/common/hashedstring.h"
#include "isoengine/render/texture.h"
#include <unordered_map>

namespace iso
{

class ResourceManager
{
public:
    static ResourceManager & getInstance();
    Texture & getTexture(const HashedString & filename);

private:
    ResourceManager() = default;
    std::unordered_map<size_t, Texture> textures;
};

} // namespace iso

#endif