#include "isoengine/support/resource_manager.h"
#include "isoengine/common/utils.h"
#include <exception>

namespace iso::support
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

Font & ResourceManager::getFont(const HashedString & filename)
{
    auto iter = fonts.insert(std::make_pair(filename.getHash(), Font{}));
    // If already exists, return
    if (!iter.second)
        return iter.first->second;
    // Inserted new, load
    Font & font = iter.first->second;
    if (!font.loadFromFile(std::string(filename.getCStr())))
        throw std::invalid_argument("ResourceManager::getFont(): invalid filename");
    return font;
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
