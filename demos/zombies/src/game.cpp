#include "game.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std::placeholders;

void loadResources(iso::ResourceManager & resManager)
{
    auto & textureWall = resManager.getTexture("res/textures/wall.jpg");
    auto & textureGrass = resManager.getTexture("res/textures/grass.jpg");
    auto & textureCave = resManager.getTexture("res/textures/cave.jpg");
    auto & texturePortal = resManager.getTexture("res/textures/portalRings.png");
    auto & textureZombie = resManager.getTexture("res/textures/zombie.png");
    auto & textureMage = resManager.getTexture("res/textures/magician.png");
    auto & textureFireball = resManager.getTexture("res/textures/fireball.png");

    resManager.addAnimation("wall", iso::StaticAnimation(textureWall, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("grass", iso::StaticAnimation(textureGrass, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("cave", iso::StaticAnimation(textureCave, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("portal", iso::Animation(texturePortal, {32, 32}, {0, 0}, {4, 4}, 1, true));

    resManager.addAnimation("zombie-left", iso::Animation(textureZombie, {128, 128}, {64, 64}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-left-up", iso::Animation(textureZombie, {128, 128}, {64, 320}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-up", iso::Animation(textureZombie, {128, 128}, {64, 576}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-right-up", iso::Animation(textureZombie, {128, 128}, {64, 832}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-right", iso::Animation(textureZombie, {128, 128}, {64, 1088}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-right-down", iso::Animation(textureZombie, {128, 128}, {64, 1344}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-down", iso::Animation(textureZombie, {128, 128}, {64, 1600}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("zombie-left-down", iso::Animation(textureZombie, {128, 128}, {64, 1856}, {4, 1}, 1, true, {128, 128}));

    resManager.addAnimation("mage-left", iso::Animation(textureMage, {128, 128}, {64, 64}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-up", iso::Animation(textureMage, {128, 128}, {64, 576}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-right", iso::Animation(textureMage, {128, 128}, {64, 1088}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-down", iso::Animation(textureMage, {128, 128}, {64, 1600}, {4, 1}, 1, true, {128, 128}));

    resManager.addAnimation("fb-left", iso::Animation(textureFireball, {64, 64}, {0, 0}, {8, 1}, 1, true));
    resManager.addAnimation("fb-up", iso::Animation(textureFireball, {64, 64}, {0, 128}, {8, 1}, 1, true));
    resManager.addAnimation("fb-right", iso::Animation(textureFireball, {64, 64}, {0, 256}, {8, 1}, 1, true));
    resManager.addAnimation("fb-down", iso::Animation(textureFireball, {64, 64}, {0, 384}, {8, 1}, 1, true));
}

Game::Game()
{
    auto & resManager = iso::ResourceManager::getInstance();
    loadResources(resManager);

    auto portalCb = std::bind(&Game::onPortal, this, _1);
    player = std::make_shared<Player>(portalCb);

    // Render scene, layers
    // Empty -> one layer, default
    engine.reset(new iso::Engine(iso::WindowOptions({spriteSize * levelWidth, spriteSize * levelHeight}, iso::ResizeStrategy::FIXED_RES_STRETCH, {4, 3}), {"background", "objects"}));

    // Create player sprite and add to engine
    player->setAnimation(resManager.getAnimation("mage-left"));
    player->setPosition({400, 200});
    player->getSprite().setScale(0.25f, 0.25f);
    loadMap();
    engine->addGameObject(player, "objects");
    engine->addRigidBody(player, playerBoundingBox);
    // Equivalent to the previous, no string = top layer
    // engine.addGameObject(player);

    // Can use lambdas instead
    engine->onUpdate += std::bind(&Game::onUpdate, this, _1);
    engine->onKey += std::bind(&Game::onKey, this, _1);
}

void Game::onUpdate(float dt)
{
    timeElapsed += dt;

    if (timeElapsed >= respawnTime) {
        timeElapsed -= respawnTime;
        respawn();
    }

    updatePlayer(dt);
    updateZombies(dt);
    updateFireballs(dt);
}

void Game::loadMap()
{
    std::ifstream file("demos/zombies/map.txt");
    if (!file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    for (unsigned i = 0; i < levelHeight; ++i) {
        std::string row;
        file >> row;
        for (unsigned j = 0; j < levelWidth; ++j) {
            unsigned tile = row[j] - '0';
            if (Tile(tile) == Tile::Portal) {
                addTile(Tile::Grass, j, i);
            }
            addTile(Tile(tile), j, i);
        }
    }
}

void Game::addTile(Tile tile, int x, int y)
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto obj = std::make_shared<Entity>(Entity::Type::Tile);
    switch (tile) {
    case Tile::Grass:
        obj->setAnimation(resManager.getAnimation("grass"));
        break;
    case Tile::Wall:
        obj->setAnimation(resManager.getAnimation("wall"));
        break;
    case Tile::Portal:
        obj->type = Entity::Type::Portal;
        obj->setAnimation(resManager.getAnimation("portal"));
        if (portalTop == nullptr) {
            portalTop = obj;
        } else {
            portalBottom = obj;
        }
        break;
    case Tile::Cave:
        obj->setAnimation(resManager.getAnimation("cave"));
        break;
    }
    obj->setPosition({float(x * spriteSize), float(y * spriteSize)});
    if (tile != Tile::Portal) {
        obj->getSprite().setScale(0.25f, 0.25f);
    }
    engine->addGameObject(obj, "objects");
    if (tile == Tile::Cave) {
        addRespawnLocation(x, y);
    }
    if (tile != Tile::Grass && tile != Tile::Cave) {
        engine->addRigidBody(obj);
    }
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

void Game::addRespawnLocation(int x, int y)
{
    respawns.emplace_back(float(x * spriteSize), float(y * spriteSize));
}

void Game::createZombie(const iso::math::Vector2f & location)
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto zombie = std::make_shared<Zombie>(player.get());
    zombie->setPosition(location);
    zombie->getSprite().setScale(0.25f, 0.25f);
    engine->addGameObject(zombie, "objects");
    engine->addRigidBody(zombie, zombieBoundingBox);
    zombies.emplace_back(zombie);
}

void Game::updatePlayer(float dt)
{
    player->update(gameSpeed, dt);
}

void Game::updateZombies(float dt)
{
    for (auto & zombie : zombies) {
        zombie->update(gameSpeed, dt);
    }
}

void Game::updateFireballs(float dt)
{
    for (auto & fb : fireballs) {
        fb->update(gameSpeed, dt);
    }
}

void Game::shootFireball()
{
    auto & resManager = iso::ResourceManager::getInstance();
    auto fb = std::make_shared<Fireball>(player->getFacingDir());

    fb->setPosition(player->getPosition() + player->getFacingDir() * spriteSize);
    fb->getSprite().setScale(0.5f, 0.5f);
    engine->addGameObject(fb, "objects");
    engine->addRigidBody(fb);
    fireballs.push_back(fb);
}

void Game::onKey(iso::KeyEvent event)
{
    switch (event.keyCode) {
    case iso::KeyCode::Left:
        player->faceDirection({-1, 0});
        break;
    case iso::KeyCode::Up:
        player->faceDirection({0, -1});
        break;
    case iso::KeyCode::Right:
        player->faceDirection({1, 0});
        break;
    case iso::KeyCode::Down:
        player->faceDirection({0, 1});
        break;
    case iso::KeyCode::Space:
        shootFireball();
        break;
    default:
        break;
    }
}

void Game::onPortal(const Entity * portal)
{
    if (portal == portalTop.get()) {
        player->setPosition(portalBottom->getPosition() - iso::math::Vector2f(spriteSize, 0));
    } else {
        player->setPosition(portalTop->getPosition() + iso::math::Vector2f(spriteSize, 0));
    }
}
