#include "isoengine/highlevel/engine.h"
#include "isoengine/support/resourcemanager.h"
#include <iostream>
#include <memory>

void loadResources(iso::ResourceManager & resManager)
{
    iso::Texture & textureMage = resManager.getTexture("res/textures/mage.png");
    iso::Texture & textureWall = resManager.getTexture("res/textures/wall.jpg");
    iso::Texture & textureGrass = resManager.getTexture("res/textures/grass.jpg");
    iso::Texture & textureZombie = resManager.getTexture("res/textures/zombie.png");

    resManager.addAnimation("mage", iso::Animation(textureMage, iso::math::Vector2i(160, 160), iso::math::Vector2i(0, 0), 1, 1, true));
    resManager.addAnimation("wall", iso::StaticAnimation(textureWall, iso::math::Vector2i(64, 64)));
    resManager.addAnimation("grass", iso::StaticAnimation(textureGrass, iso::math::Vector2i(64, 64)));
    resManager.addAnimation("zombie", iso::StaticAnimation(textureZombie, iso::math::Vector2i(128, 128), iso::math::Vector2i(830, 320)));
}

struct Game {
    Game();
    void loop(float dt);
    void run();
    void respawn();
    void setRespawnLocations();
    void createZombie(const iso::math::Vector2f & location);
    void moveZombies(float dt);

    std::unique_ptr<iso::Engine> engine;
    float timeElapsed = float{};
    const float respawnTime = 5.f;
    const float gameSpeed = 1000.f / 30.f;
    std::shared_ptr<iso::GameObject> player;
    std::vector<iso::math::Vector2f> respawns;
    std::vector<std::shared_ptr<iso::GameObject>> zombies;
};

Game::Game()
{
    auto & resManager = iso::ResourceManager::getInstance();
    loadResources(resManager);

    player = std::make_shared<iso::GameObject>();

    // Render scene, layers
    // Empty -> one layer, default
    engine.reset(new iso::Engine(iso::WindowOptions({800, 600}, iso::ResizeStrategy::FIXED_ASPECT_RATIO_EXPAND_RES, {4, 3}), {"background", "objects"}));

    // Create player sprite and add to engine
    player->setAnimation(resManager.getAnimation("mage"));
    player->setPosition({400, 250});
    player->getSprite().setScale(0.5f, 0.5f);
    engine->addGameObject(player, "objects");
    // engine.addGameObject(player);
    // Equivalent to the previous, no string = top layer

    auto cb = [this](float dt) {
        loop(dt);
    };

    setRespawnLocations();

    engine->setGameLoop(cb);
}

void Game::loop(float dt)
{
    timeElapsed += dt;

    if (timeElapsed >= respawnTime) {
        timeElapsed -= respawnTime;
        respawn();
    }

    moveZombies(dt);
}

void Game::respawn()
{
    for (auto & loc : respawns) {
        createZombie(loc);
    }
}

void Game::run()
{
    engine->run();
}

void Game::setRespawnLocations()
{
    respawns.emplace_back(300.f, 20.f);
    respawns.emplace_back(100.f, 400.f);
    respawns.emplace_back(500.f, 500.f);
}

void Game::createZombie(const iso::math::Vector2f & location)
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto zombie = std::make_shared<iso::GameObject>();
    zombie->setAnimation(resManager.getAnimation("zombie"));
    zombie->setPosition(location);
    zombie->getSprite().setScale(0.5f, 0.5f);
    engine->addGameObject(zombie, "objects");
    zombies.push_back(zombie);
}

void Game::moveZombies(float dt)
{
    auto speed = gameSpeed * dt;
    for (auto & zombie : zombies) {
        zombie->move((player->getPosition() - zombie->getPosition()).normalize() * speed);
    }
}

int main()
{
    Game game;
    game.run();
    return 0;
}
