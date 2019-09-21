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
    resManager.addAnimation("zombie-left", iso::Animation(textureZombie, {256, 256}, {0, 0}, 4, 1, true));
    resManager.addAnimation("zombie-up", iso::Animation(textureZombie, {256, 256}, {0, 512}, 4, 1, true));
    resManager.addAnimation("zombie-right", iso::Animation(textureZombie, {256, 256}, {0, 1024}, 4, 1, true));
    resManager.addAnimation("zombie-down", iso::Animation(textureZombie, {256, 256}, {0, 1536}, 4, 1, true));
}

enum class Direction {
    Left,
    Up,
    Right,
    Down
};

Direction getDir(const iso::math::Vector2f & dir)
{
    if (std::abs(dir.x) >= std::abs(dir.y)) {
        if (dir.x < 0)
            return Direction::Left;
        else
            return Direction::Right;
    } else {
        if (dir.y < 0)
            return Direction::Up;
        else
            return Direction::Down;
    }
}

using ZombieInfo = std::pair<std::shared_ptr<iso::GameObject>, Direction>;

struct Game {
    Game();
    void loop(float dt);
    void run();
    void respawn();
    void setRespawnLocations();
    void createZombie(const iso::math::Vector2f & location);
    void moveZombies(float dt);
    void updatePlayer(float dt);
    void onKey(const iso::Event & event);

    std::unique_ptr<iso::Engine> engine;
    float timeElapsed = float{};
    const float respawnTime = 5.f;
    const float gameSpeed = 2000.f / 30.f;
    std::shared_ptr<iso::GameObject> player;
    iso::Vector2f playerDir;
    std::vector<iso::math::Vector2f> respawns;
    std::vector<ZombieInfo> zombies;
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

    setRespawnLocations();

    engine->setGameLoop(std::bind(&Game::loop, this, std::placeholders::_1));
    engine->addEventHandler(std::bind(&Game::onKey, this, std::placeholders::_1));
}

void Game::loop(float dt)
{
    timeElapsed += dt;

    if (timeElapsed >= respawnTime) {
        timeElapsed -= respawnTime;
        respawn();
    }

    updatePlayer(dt);
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
    zombie->setAnimation(resManager.getAnimation("zombie-left"));
    zombie->setPosition(location);
    zombie->getSprite().setScale(0.5f, 0.5f);
    engine->addGameObject(zombie, "objects");
    zombies.emplace_back(zombie, Direction::Left);
}

void Game::moveZombies(float dt)
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto speed = gameSpeed * dt;
    for (auto & zombieInfo : zombies) {
        auto & zombie = zombieInfo.first;
        auto & dir = zombieInfo.second;
        auto v = (player->getPosition() - zombie->getPosition()).normalize();
        zombie->move(v * speed);
        auto newDir = getDir(v);
        if (newDir != dir) {
            dir = newDir;
            if (dir == Direction::Left)
                zombie->setAnimation(resManager.getAnimation("zombie-left"));
            else if (dir == Direction::Up)
                zombie->setAnimation(resManager.getAnimation("zombie-up"));
            else if (dir == Direction::Right)
                zombie->setAnimation(resManager.getAnimation("zombie-right"));
            else
                zombie->setAnimation(resManager.getAnimation("zombie-down"));
        }
    }
}

void Game::updatePlayer(float dt)
{
    auto speed = gameSpeed * dt * 4;
    player->move(playerDir * speed);
    playerDir = {0, 0};
}

void Game::onKey(const iso::Event & event)
{
    if (event.type == iso::EventType::Key) {
        switch (event.event.key.code) {
        case sf::Keyboard::Left:
            playerDir = {-1, 0};
            break;
        case sf::Keyboard::Up:
            playerDir = {0, -1};
            break;
        case sf::Keyboard::Right:
            playerDir = {1, 0};
            break;
        case sf::Keyboard::Down:
            playerDir = {0, 1};
            break;
        default:
            break;
        }
    }
}

int main()
{
    Game game;
    game.run();
    return 0;
}
