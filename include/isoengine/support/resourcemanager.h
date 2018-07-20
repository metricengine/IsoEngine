#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "isoengine/render/texture.h"
#include <map>

namespace iso
{

class ResourceManager
{
public:
    static ResourceManager & getInstance();
    Texture & getTexture(const std::string & filename);

private:
    ResourceManager() = default;
    std::map<std::string, Texture> textures;
};

} // namespace iso

#endif