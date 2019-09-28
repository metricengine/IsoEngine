#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "isoengine/highlevel/engine.h"
#include "isoengine/support/resourcemanager.h"

enum class Tile {
    Grass = 0,
    Wall,
    Portal,
    Cave
};

enum class Direction {
    Left,
    Left_Up,
    Up,
    Right_Up,
    Right,
    Right_Down,
    Down,
    Left_Down
};

using ZombieInfo = std::pair<std::shared_ptr<iso::GameObject>, Direction>;

void loadResources(iso::ResourceManager & resManager);
Direction getDir(const iso::math::Vector2f & dir);

struct Game {
    Game();
    void loadMap();
    void addTile(Tile tile, int x, int y);
    void loop(float dt);
    void run();
    void addRespawnLocation(int x, int y);
    void respawn();
    void createZombie(const iso::math::Vector2f & location);
    void moveZombies(float dt);
    void updatePlayer(float dt);
    void onKey(iso::KeyEvent event);
    void onPortal(const Entity * portal);

    std::unique_ptr<iso::Engine> engine;

    unsigned spriteSize = 32;
    unsigned width = 24;
    unsigned height = 18;
    const iso::math::Rectf playerBoundingBox = {13, 5, 10, 22};
    const iso::math::Rectf zombieBoundingBox = {13, 5, 10, 22};

    float timeElapsed = float{};
    const float respawnTime = 5.f;
    const float gameSpeed = 2000.f / 30.f;
    std::shared_ptr<PlayerEntity> player;
    std::shared_ptr<Entity> portalTop, portalBottom;
    iso::Vector2f playerDir;
    std::vector<iso::math::Vector2f> respawns;
    std::vector<ZombieInfo> zombies;
};

#endif // GAME_H
