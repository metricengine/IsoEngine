#include "isoengine/highlevel/engine.h"
#include "isoengine/support/resourcemanager.h"
#include <iostream>

struct MovementEvent {
    MovementEvent(iso::GameObject & player,
                  iso::GameObject & portal) : player(player), portal(portal) {}

    void operator()(const iso::Event & event)
    {
        if (event.type == iso::EventType::Key) {
            if (event.event.key.code == sf::Keyboard::Left)
                player.move({-5, 0});
            else if (event.event.key.code == sf::Keyboard::Right)
                player.move({5, 0});
            auto posDiff = player.getPosition() - portal.getPosition();
            if (posDiff.length() <= 10) {
                player.sendCommand(std::make_shared<iso::Command>("close"));
            } else {
                player.sendCommand(std::make_shared<iso::Command>("open"));
            }
        }
    }

    iso::GameObject & player;
    iso::GameObject & portal;
};

struct Portal : iso::GameObject {
    void handleCommand(iso::GameObject & sender,
                       const iso::Command & command) override
    {
        if (command.type == iso::HashedString("close"))
            setAnimation(iso::ResourceManager::getInstance().getAnimation("portal_closed"));
        else if (command.type == iso::HashedString("open"))
            setAnimation(iso::ResourceManager::getInstance().getAnimation("portal_opened"));
    }
};

struct CentererCommandListener {
    void operator()(iso::GameObject & sender, const iso::Command & command)
    {
        if (command.type == iso::HashedString("close"))
            sender.setPosition({400, 250});
    }
};

int main()
{
    auto & resManager = iso::ResourceManager::getInstance();

    iso::Texture & texture = resManager.getTexture("res/textures/dummy.png");
    iso::Texture & textureCrystal = resManager.getTexture("res/textures/crystal.png");
    iso::Texture & texturePortal = resManager.getTexture("res/textures/portal.png");
    iso::Texture & textureMage = resManager.getTexture("res/textures/mage.png");

    resManager.addAnimation("crystal", iso::Animation(textureCrystal, iso::math::Vector2i(0, 0), iso::math::Vector2i(32, 32), 8, 1, true));
    texture.setSmooth(true);
    resManager.addAnimation("portal_opened", iso::Animation(texturePortal, iso::math::Vector2i(224, 160), iso::math::Vector2i(32, 32), 1, 1, true));
    resManager.addAnimation("portal_closed", iso::Animation(texturePortal, iso::math::Vector2i(224, 192), iso::math::Vector2i(32, 32), 1, 1, true));
    resManager.addAnimation("mage", iso::Animation(textureMage, iso::math::Vector2i(0, 0), iso::math::Vector2i(160, 160), 1, 1, true));

    auto player = std::make_shared<iso::GameObject>();
    auto portal = std::make_shared<Portal>();
    player->setAnimation(resManager.getAnimation("mage"));
    portal->setAnimation(resManager.getAnimation("portal_opened"));
    player->setPosition({400, 250});
    portal->setPosition({200, 250});

    iso::Engine engine;

    engine.addGameObject(player);
    engine.addGameObject(portal);
    engine.registerCommand("close");
    engine.registerCommand("open");
    portal->setCommandTypes({"close", "open"});
    engine.addEventHandler(MovementEvent(*player, *portal));
    engine.addCommandHandler(CentererCommandListener());

    engine.run();

    return 0;
}
