#include "isoengine/support/resourcemanager.h"
#include <exception>

namespace iso
{

ResourceManager & ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

Texture & ResourceManager::getTexture(const HashedString & filename)
{
    auto iter = textures.insert(std::make_pair(filename.getHash(), Texture{}));
    // If already exists, return
    if (!iter.second)
        return iter.first->second;
    // Inserted new, load
    Texture & texture = iter.first->second;
    if (!texture.loadFromFile(std::string(filename.getCStr())))
        throw std::invalid_argument("ResourceManager::getTexture(): invalid filename");
    return texture;
}

} // namespace iso