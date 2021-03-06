#include "game.h"
#include "isoengine/support/resource_manager.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std::placeholders;

void loadResources(iso::support::ResourceManager & resManager)
{
    auto & textureWall = resManager.getTexture("res/textures/wall.jpg");
    auto & textureGrass = resManager.getTexture("res/textures/grass.jpg");
    auto & textureCave = resManager.getTexture("res/textures/cave.jpg");
    auto & texturePortal = resManager.getTexture("res/textures/portalRings.png");
    auto & textureZombie = resManager.getTexture("res/textures/zombie.png");
    auto & textureMage = resManager.getTexture("res/textures/magician.png");
    auto & textureFireball = resManager.getTexture("res/textures/fireball.png");

    resManager.addAnimation("wall", iso::render::StaticAnimation(textureWall, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("grass", iso::render::StaticAnimation(textureGrass, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("cave", iso::render::StaticAnimation(textureCave, iso::math::Vector2i(128, 128)));
    resManager.addAnimation("portal", iso::render::Animation(texturePortal, {32, 32}, {0, 0}, {4, 4}, 1, true));

    float as = Zombie::AnimationTime;

    resManager.addAnimation("zombie-left", iso::render::Animation(textureZombie, {128, 128}, {64, 64}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-left-up", iso::render::Animation(textureZombie, {128, 128}, {64, 320}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-up", iso::render::Animation(textureZombie, {128, 128}, {64, 576}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-right-up", iso::render::Animation(textureZombie, {128, 128}, {64, 832}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-right", iso::render::Animation(textureZombie, {128, 128}, {64, 1088}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-right-down", iso::render::Animation(textureZombie, {128, 128}, {64, 1344}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-down", iso::render::Animation(textureZombie, {128, 128}, {64, 1600}, {4, 1}, as, true, {128, 128}));
    resManager.addAnimation("zombie-left-down", iso::render::Animation(textureZombie, {128, 128}, {64, 1856}, {4, 1}, as, true, {128, 128}));

    resManager.addAnimation("mage-left", iso::render::Animation(textureMage, {128, 128}, {64, 64}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-left-up", iso::render::Animation(textureMage, {128, 128}, {64, 320}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-up", iso::render::Animation(textureMage, {128, 128}, {64, 576}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-right-up", iso::render::Animation(textureMage, {128, 128}, {64, 832}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-right", iso::render::Animation(textureMage, {128, 128}, {64, 1088}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-right-down", iso::render::Animation(textureMage, {128, 128}, {64, 1344}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-down", iso::render::Animation(textureMage, {128, 128}, {64, 1600}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("mage-left-down", iso::render::Animation(textureMage, {128, 128}, {64, 1856}, {4, 1}, 1, true, {128, 128}));

    resManager.addAnimation("fb-left", iso::render::Animation(textureFireball, {64, 64}, {0, 0}, {8, 1}, 1, true));
    resManager.addAnimation("fb-left-up", iso::render::Animation(textureFireball, {64, 64}, {0, 64}, {8, 1}, 1, true));
    resManager.addAnimation("fb-up", iso::render::Animation(textureFireball, {64, 64}, {0, 128}, {8, 1}, 1, true));
    resManager.addAnimation("fb-right-up", iso::render::Animation(textureFireball, {64, 64}, {0, 192}, {8, 1}, 1, true));
    resManager.addAnimation("fb-right", iso::render::Animation(textureFireball, {64, 64}, {0, 256}, {8, 1}, 1, true));
    resManager.addAnimation("fb-right-down", iso::render::Animation(textureFireball, {64, 64}, {0, 320}, {8, 1}, 1, true));
    resManager.addAnimation("fb-down", iso::render::Animation(textureFireball, {64, 64}, {0, 384}, {8, 1}, 1, true));
    resManager.addAnimation("fb-left-down", iso::render::Animation(textureFireball, {64, 64}, {0, 448}, {8, 1}, 1, true));
}

Game::Game()
{
    fireballCb = [this](const Fireball * fireball, const Zombie * zombie) {
        onFireball(fireball, zombie);
    };
    playerReachedCb = [this]() {
        onPlayerReached();
    };

    // Render scene, layers
    // Empty -> one layer, default
    engine.reset(new iso::Engine(iso::WindowOptions({Entity::SpriteSize * LevelWidth, Entity::SpriteSize * LevelHeight}, iso::ResizeStrategy::FIXED_RES_STRETCH, {4, 3}), {"background", "objects"}));
    engine->onUpdate += std::bind(&Game::onUpdate, this, _1);
    engine->onKey += std::bind(&Game::onKey, this, _1);

    auto & resManager = iso::support::ResourceManager::getInstance();
    loadResources(resManager);
    loadMap();
    reset();
}

void Game::clear()
{
    for (auto fireball : fireballs) {
        engine->removeGameObject(fireball.get());
    }
    for (auto & zombie : zombies) {
        engine->removeGameObject(zombie.get());
    }
    if (player != nullptr) {
        engine->removeGameObject(player.get());
    }

    fireballs.clear();
    fbToRemove.clear();
    zombies.clear();
    player.reset();

    state = State::Playing;
    timeElapsed = float{};

    if (gameOverText != nullptr) {
        engine->removeSceneNode(gameOverText.get());
        gameOverText.reset();
    }
}

void Game::reset()
{
    clear();

    auto & resManager = iso::support::ResourceManager::getInstance();
    auto portalCb = std::bind(&Game::onPortal, this, _1);
    player = std::make_shared<Player>(portalCb);

    // Create player sprite and add to engine
    player->setAnimation(resManager.getAnimation("mage-left"));
    player->setPosition({400, 200});
    player->getSprite().setScale(0.25f, 0.25f);
    engine->addGameObject(player, "objects");
    engine->addRigidBody(player, playerBoundingBox);
    // Equivalent to the previous, no string = top layer
    // engine.addGameObject(player);
}

void Game::loadMap()
{
    std::ifstream file("demos/zombies/map.txt");
    if (!file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    for (unsigned i = 0; i < LevelHeight; ++i) {
        std::string row;
        file >> row;
        for (unsigned j = 0; j < LevelWidth; ++j) {
            unsigned tile = row[j] - '0';
            map[i * LevelWidth + j] = Tile(tile);
            if (Tile(tile) == Tile::Portal) {
                addTile(Tile::Grass, j, i);
            }
            addTile(Tile(tile), j, i);
        }
    }
}

void Game::addTile(Tile tile, int x, int y)
{
    auto & resManager = iso::support::ResourceManager::getInstance();
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
    obj->setPosition({float(x * Entity::SpriteSize), float(y * Entity::SpriteSize)});
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
    respawns.emplace_back(float(x * Entity::SpriteSize), float(y * Entity::SpriteSize));
}

void Game::createZombie(const iso::math::Vector2f & location)
{
    auto zombie = std::make_shared<Zombie>(map, player.get(), playerReachedCb);
    zombie->setPosition(location);
    zombie->getSprite().setScale(0.25f, 0.25f);
    engine->addGameObject(zombie, "objects");
    engine->addRigidBody(zombie, zombieBoundingBox);
    zombies.emplace_back(zombie);
}

void Game::updatePlayer(float dt)
{
    iso::math::Vector2f newDir;
    if (iso::events::Keyboard::isKeyPressed(iso::events::KeyCode::Left)) {
        newDir += {-1, 0};
    }
    if (iso::events::Keyboard::isKeyPressed(iso::events::KeyCode::Up)) {
        newDir += {0, -1};
    }
    if (iso::events::Keyboard::isKeyPressed(iso::events::KeyCode::Right)) {
        newDir += {1, 0};
    }
    if (iso::events::Keyboard::isKeyPressed(iso::events::KeyCode::Down)) {
        newDir += {0, 1};
    }
    if (newDir != iso::math::Vector2f()) {
        player->faceDirection(newDir);
    }
    if (iso::events::Keyboard::isKeyPressed(iso::events::KeyCode::Space)) {
        shootFireball();
    }
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
    for (auto & fireball : fireballs) {
        fireball->update(gameSpeed, dt);
    }

    for (auto remove : fbToRemove) {
        for (auto iter = fireballs.begin(); iter != fireballs.end(); ++iter) {
            if (iter->get() == remove) {
                fireballs.erase(iter);
                break;
            }
        }
    }

    fbToRemove.clear();
}

void Game::shootFireball()
{
    if (!player->canShoot())
        return;

    auto fb = std::make_shared<Fireball>(player->getFacingDir(), fireballCb);

    fb->setPosition(player->getPosition() + player->getFacingDir() * Entity::SpriteSize * 0.5f);
    fb->getSprite().setScale(0.5f, 0.5f);
    engine->addGameObject(fb, "objects");
    engine->addRigidBody(fb, {8, 8, 16, 16});
    fireballs.push_back(fb);
    player->shoot();
}

void Game::onUpdate(float dt)
{
    if (state == State::Over) {
        return;
    }

    timeElapsed += dt;

    if (timeElapsed >= respawnTime) {
        timeElapsed -= respawnTime;
        respawn();
    }

    updatePlayer(dt);
    updateZombies(dt);
    updateFireballs(dt);
}

void Game::onKey(iso::events::KeyEvent event)
{
    if (event.eventType == iso::events::KeyEventType::KeyPressed) {
        switch (event.keyCode) {
        case iso::events::KeyCode::Enter:
            if (state == State::Over) {
                reset();
            }
            break;
        default:
            break;
        }
    }
}

void Game::onPortal(const Entity * portal)
{
    if (portal == portalTop.get()) {
        player->setPosition(portalBottom->getPosition() - iso::math::Vector2f(Entity::SpriteSize, 0));
    } else {
        player->setPosition(portalTop->getPosition() + iso::math::Vector2f(Entity::SpriteSize, 0));
    }
}

void Game::onFireball(const Fireball * fireball, const Zombie * zombie)
{
    if (zombie != nullptr) {
        engine->removeGameObject(zombie);
        for (auto iter = zombies.begin(); iter != zombies.end(); ++iter) {
            if (iter->get() == zombie) {
                zombies.erase(iter);
                break;
            }
        }
    }

    engine->removeGameObject(fireball);
    for (auto iter = fireballs.begin(); iter != fireballs.end(); ++iter) {
        if (iter->get() == fireball) {
            fbToRemove.push_back(fireball);
        }
    }
}

void Game::onPlayerReached()
{
    auto & resManager = iso::support::ResourceManager::getInstance();

    state = State::Over;
    gameOverText = std::make_shared<iso::render::SceneNodeObject<iso::render::Text>>();
    gameOverText->getObject().setFont(resManager.getFont("res/fonts/Arial.ttf"));
    gameOverText->getObject().setString("Game over! Press enter to restart!");
    float x = float(LevelWidth * Entity::SpriteSize / 2) - gameOverText->getObject().getSize().x / 2;
    float y = float(LevelHeight * Entity::SpriteSize / 2) - gameOverText->getObject().getSize().y / 2;
    gameOverText->setPosition({x, y});
    engine->addSceneNode(gameOverText);
}
