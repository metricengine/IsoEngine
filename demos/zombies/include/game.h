#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "isoengine/highlevel/engine.h"
#include "isoengine/render/text.h"
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
    enum class State {
        Playing,
        Over
    };

public:
    Game();
    void run();

private:
    void clear();
    void reset();
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
    void onPlayerReached();

    std::unique_ptr<iso::Engine> engine;

    const unsigned spriteSize = 32;
    const unsigned levelWidth = 24;
    const unsigned levelHeight = 18;
    const iso::math::Rectf playerBoundingBox = {13, 5, 10, 22};
    const iso::math::Rectf zombieBoundingBox = {13, 5, 10, 22};
    const float respawnTime = 5.f;
    const float gameSpeed = 2000.f / 30.f;

    State state = State::Playing;
    float timeElapsed = float{};
    std::shared_ptr<Player> player;
    std::shared_ptr<Entity> portalTop, portalBottom;
    std::vector<iso::math::Vector2f> respawns;
    std::vector<std::shared_ptr<Zombie>> zombies;
    std::vector<std::shared_ptr<Fireball>> fireballs;
    std::vector<const Fireball *> fbToRemove;
    std::function<void(const Fireball *, const Zombie *)> fireballCb;
    std::function<void()> playerReachedCb;

    std::shared_ptr<iso::SceneNodeObject<iso::Text>> gameOverText;
};

#endif // GAME_H
