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

void loadResources(iso::ResourceManager & resManager);

class Game
{
public:
    Game();
    void run();

private:
    void loadMap();
    void addTile(Tile tile, int x, int y);
    void addRespawnLocation(int x, int y);
    void respawn();
    void createZombie(const iso::math::Vector2f & location);
    void updatePlayer(float dt);
    void updateZombies(float dt);
    void updateFireballs(float dt);
    void shootFireball();

    // Events
    void onUpdate(float dt);
    void onKey(iso::KeyEvent event);
    void onPortal(const Entity * portal);
    void onFireball(const Fireball * fireball, const Zombie * zombie);

    std::unique_ptr<iso::Engine> engine;

    unsigned spriteSize = 32;
    unsigned levelWidth = 24;
    unsigned levelHeight = 18;
    const iso::math::Rectf playerBoundingBox = {13, 5, 10, 22};
    const iso::math::Rectf zombieBoundingBox = {13, 5, 10, 22};

    float timeElapsed = float{};
    const float respawnTime = 5.f;
    const float gameSpeed = 2000.f / 30.f;
    std::shared_ptr<Player> player;
    std::shared_ptr<Entity> portalTop, portalBottom;
    std::vector<iso::math::Vector2f> respawns;
    std::vector<std::shared_ptr<Zombie>> zombies;
    std::vector<std::shared_ptr<Fireball>> fireballs;
    std::vector<const Fireball *> fbToRemove;
    std::function<void(const Fireball *, const Zombie *)> fireballCb;
};

#endif // GAME_H
