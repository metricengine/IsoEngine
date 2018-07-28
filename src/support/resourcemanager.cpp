#include "isoengine/support/resourcemanager.h"
#include "isoengine/common/utils.h"
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

void ResourceManager::addAnimation(const HashedString & name, Animation animation)
{
    auto iter = animations.insert(std::make_pair(name, std::make_shared<Animation>(animation)));
    ASSERT(iter.second, "ResourceManager::addAnimation duplicated animation name");
}

Ptr<Animation> ResourceManager::getAnimation(const HashedString & name)
{
    auto iter = animations.find(name.getHash());
    if (iter == animations.end())
        return nullptr;
    return iter->second;
}

} // namespace iso