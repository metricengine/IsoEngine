#include "isoengine/highlevel/engine.h"
#include "isoengine/support/resourcemanager.h"
#include <iostream>

struct MovementEvent {
    MovementEvent(iso::Engine & engine,
                  iso::GameObject & player,
                  iso::GameObject & portal) : engine(engine), player(player), portal(portal) {}

    void operator()(const iso::Event & event)
    {
        if (event.type == iso::EventType::Key) {
            if (event.event.key.code == sf::Keyboard::Left)
                player.move({-5, 0});
            else if (event.event.key.code == sf::Keyboard::Right)
                player.move({5, 0});
            else if (event.event.key.code == sf::Keyboard::Space) {
                following = !following;
                if (following) {
                    engine.cameraStopFollowing();
                } else {
                    engine.cameraFollowObject(&player);
                }
            }

            // auto posDiff = player.getPosition() - portal.getPosition();
            // if (posDiff.length() <= 5) {
            //     player.sendCommand(std::make_shared<iso::Command>("close"));
            // } else {
            //     player.sendCommand(std::make_shared<iso::Command>("open"));
            // }
        }
    }

    bool following = false;
    iso::Engine & engine;
    iso::GameObject & player;
    iso::GameObject & portal;
};

struct MouseEvent {
    MouseEvent(iso::Engine & engine) : engine(engine) {}

    void operator()(const iso::Event & event)
    {
        if (event.type == iso::EventType::Mouse) {
            auto obj = std::make_shared<iso::GameObject>();
            iso::Vector2f coords(static_cast<float>(event.event.mouseButton.x), static_cast<float>(event.event.mouseButton.y));
            obj->getSprite().setPosition(coords);
            obj->setAnimation(iso::ResourceManager::getInstance().getAnimation("crystal"));
            obj->setAnimationSpeed((rand() % 200) / 200.f + 0.5f);
            engine.addGameObject(obj, "objects");
        }
    }

    iso::Engine & engine;
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

    iso::Texture & textureCrystal = resManager.getTexture("res/textures/crystal.png");
    iso::Texture & texturePortal = resManager.getTexture("res/textures/portal.png");
    iso::Texture & textureMage = resManager.getTexture("res/textures/mage.png");
    iso::Texture & textureBg = resManager.getTexture("res/textures/red_hexagon_tile.jpg");

    resManager.addAnimation("crystal", iso::Animation(textureCrystal, iso::math::Vector2i(0, 0), iso::math::Vector2i(32, 32), 8, 1, true));
    resManager.addAnimation("portal_opened", iso::Animation(texturePortal, iso::math::Vector2i(224, 160), iso::math::Vector2i(32, 32), 1, 1, true));
    resManager.addAnimation("portal_closed", iso::Animation(texturePortal, iso::math::Vector2i(224, 192), iso::math::Vector2i(32, 32), 1, 1, true));
    resManager.addAnimation("mage", iso::Animation(textureMage, iso::math::Vector2i(0, 0), iso::math::Vector2i(160, 160), 1, 1, true));
    resManager.addAnimation("bg", iso::StaticAnimation(textureBg, iso::math::Vector2i(50, 50)));

    auto player = std::make_shared<iso::GameObject>();
    auto portal = std::make_shared<Portal>();

    player->setAnimation(resManager.getAnimation("mage"));
    portal->setAnimation(resManager.getAnimation("portal_opened"));
    player->setPosition({400, 250});
    portal->setPosition({200, 250});

    auto mageCrystal = std::make_shared<iso::GameObject>();
    mageCrystal->setAnimation(resManager.getAnimation("crystal"));
    player->addChild(mageCrystal);      // Object and relative position, automatically update children when moving
    mageCrystal->setPosition({123, 8}); // Relative position

    // Render scene, layers
    // Empty -> one layer, default
    iso::Engine engine({"background", "objects"});

    engine.addGameObject(player, "objects");
    // engine.addGameObject(player);
    // Equivalent to the previous, no string = top layer
    engine.addGameObject(portal, "objects");
    engine.registerGameObject(mageCrystal);

    for (int i = 0; i < 4; ++i) {
        auto bgTile = std::make_shared<iso::GameObject>();
        bgTile->setAnimation(resManager.getAnimation("bg"));
        bgTile->setPosition({(i % 2) * 50.f - 25.f + player->getPosition().x, (i / 2) * 50.f - 25.f + player->getPosition().y});
        engine.addGameObject(bgTile, "background");
    }

    engine.registerCommand("close");
    engine.registerCommand("open");
    portal->setCommandTypes({"close", "open"});
    engine.addEventHandler(MovementEvent(engine, *player, *portal));
    engine.addEventHandler(MouseEvent(engine));
    engine.addCommandHandler(CentererCommandListener());

    engine.run();

    return 0;
}
