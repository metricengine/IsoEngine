#include "isoengine/support/resource_manager.h"
#include "isoengine/debug/utils.h"
#include <exception>

namespace iso::support
{

ResourceManager & ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

iso::render::Texture & ResourceManager::getTexture(const HashedString & filename)
{
    auto iter = textures.insert(std::make_pair(filename.getHash(), iso::render::Texture{}));
    // If already exists, return
    if (!iter.second)
        return iter.first->second;
    // Inserted new, load
    iso::render::Texture & texture = iter.first->second;
    if (!texture.loadFromFile(std::string(filename.getCStr())))
        throw std::invalid_argument("ResourceManager::getTexture(): invalid filename");
    return texture;
}

iso::render::Font & ResourceManager::getFont(const HashedString & filename)
{
    auto iter = fonts.insert(std::make_pair(filename.getHash(), iso::render::Font{}));
    // If already exists, return
    if (!iter.second)
        return iter.first->second;
    // Inserted new, load
    iso::render::Font & font = iter.first->second;
    if (!font.loadFromFile(std::string(filename.getCStr())))
        throw std::invalid_argument("ResourceManager::getFont(): invalid filename");
    return font;
}

void ResourceManager::addAnimation(const HashedString & name, iso::render::Animation animation)
{
    auto iter = animations.insert(std::make_pair(name, std::make_shared<iso::render::Animation>(animation)));
    ASSERT(iter.second, "ResourceManager::addAnimation duplicated animation name");
}

Ptr<iso::render::Animation> ResourceManager::getAnimation(const HashedString & name)
{
    auto iter = animations.find(name.getHash());
    if (iter == animations.end())
        return nullptr;
    return iter->second;
}

} // namespace iso
