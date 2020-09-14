#include "isoengine/directed_game_object.h"
#include "isoengine/engine.h"
#include "isoengine/support/resource_manager.h"
#include "isoengine/math/isometric.h"

int main()
{
    auto & resManager = iso::support::ResourceManager::getInstance();

    auto & textureHero = resManager.getTexture("res/textures/male_heavy.png");
    auto & textureFloor = resManager.getTexture("res/textures/iso_floor.png");
    auto & textureWall = resManager.getTexture("res/textures/iso_wall.png");
    // auto & textureWallLeft = resManager.getTexture("res/textures/leftwall.tga");
    // auto & textureWallRight = resManager.getTexture("res/textures/rightwall.tga");

    resManager.addAnimation("hero-left", iso::render::Animation(textureHero, {128, 128}, {64, 64}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-left-up", iso::render::Animation(textureHero, {128, 128}, {64, 320}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-up", iso::render::Animation(textureHero, {128, 128}, {64, 576}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-right-up", iso::render::Animation(textureHero, {128, 128}, {64, 832}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-right", iso::render::Animation(textureHero, {128, 128}, {64, 1088}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-right-down", iso::render::Animation(textureHero, {128, 128}, {64, 1344}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-down", iso::render::Animation(textureHero, {128, 128}, {64, 1600}, {4, 1}, 1, true, {128, 128}));
    resManager.addAnimation("hero-left-down", iso::render::Animation(textureHero, {128, 128}, {64, 1856}, {4, 1}, 1, true, {128, 128}));

    resManager.addAnimation("floor", iso::render::StaticAnimation(textureFloor, {128, 64}));
    // resManager.addAnimation("wall", iso::render::StaticAnimation(textureFloor, {128, 64}));
    resManager.addAnimation("wall", iso::render::StaticAnimation(textureWall, {128, 128}));
    // resManager.addAnimation("wall-left", iso::render::StaticAnimation(textureWallLeft, {64, 256}));
    // resManager.addAnimation("wall-right", iso::render::StaticAnimation(textureWallRight, {64, 256}));

    constexpr int W = 8;
    constexpr int H = 8;
    constexpr float speed = 1.f;

    iso::Engine engine(iso::WindowOptions({500, 500}, iso::ResizeStrategy::DYNAMIC_RES, {1, 1}),
                       {"ground", "above"});
    // E.g.
    engine.setBoard(true, {H, W}, {64, 64});
    // engine.setBoard(H, W, size: 64, isomorphic: true, color: red);
    // engine.setBoardPosition(obj, iso::math::Vector2f(3, 4));
    // engine.setBoardPosition(obj, iso::math::Vector2f(3.5, 4));

    std::vector<std::shared_ptr<iso::render::Animation>> animations = {
        resManager.getAnimation("hero-right"),
        resManager.getAnimation("hero-up"),
        resManager.getAnimation("hero-left"),
        resManager.getAnimation("hero-down")};

    auto dirAnimations = iso::DirectedGameObject::createDirectedAnimations(animations);
    auto obj = std::make_shared<iso::DirectedGameObject>(dirAnimations, iso::math::Vector2f(1.f, 0.f));
    // auto obj = std::make_shared<iso::DirectedGameObject>();
    // obj->setAnimations(animations);
    // obj->move(iso::math::Vector2f(1.f, 0.f), 0.f);

    int map[H][W] = {{1,1,1,1,1,1,1,1},
                     {1,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,1},
                     {1,1,1,1,1,1,1,1}};

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            auto tile = std::make_shared<iso::GameObject>();

            if (map[i][j] == 0) {
                tile->setAnimation(resManager.getAnimation("floor"));
                tile->getSprite().setOrigin({64.f, 0.f});
                engine.addGameObject(tile, "ground");
            }
            if (map[i][j] == 1) {
                tile->setAnimation(resManager.getAnimation("wall"));
                tile->getSprite().setOrigin({64.f, 64.f});
                // tile->getSprite().setOrigin({64.f, 0.f});
                engine.addGameObject(tile, "above");
                engine.setTileRigidness(true, i, j);
            }
            // if (map[i][j] == 1 || map[i][j] == 3) {
            //     tile->setAnimation(resManager.getAnimation("wall-left"));
            //     tile->getSprite().setOrigin({64.f, 256.f});
            //     engine.addGameObject(tile, "above");
            //     engine.setTileRigidness(true, i, j);
            // }
            // if (map[i][j] == 2 || map[i][j] == 3) {
            //     tile->setAnimation(resManager.getAnimation("wall-right"));
            //     tile->getSprite().setOrigin({0.f, 256.f});
            //     engine.addGameObject(tile, "above");
            //     engine.setTileRigidness(true, i, j);
            // }

            engine.setBoardPosition(tile, {float(j), float(i)});
        }
    }

    // TODO: add collision detection based on tiles
    engine.addGameObject(obj, "above");
    engine.addRigidBody(obj, {1, 1, 1, 1});
    engine.cameraFollowObject(obj.get());
    engine.setBoardPosition(obj, {3, 3});
    // obj->getSprite().setScale(0.5f, 0.5f);
    obj->getSprite().setOrigin({64.f, 102.f});

    engine.onKey += [&obj, &engine](iso::events::KeyEvent keyEvent) {
        if (keyEvent.keyCode == iso::events::KeyCode::Right) {
            obj->move({1.f, 0.f}, speed);
        } else if (keyEvent.keyCode == iso::events::KeyCode::Left) {
            obj->move({-1.f, 0.f}, speed);
        } else if (keyEvent.keyCode == iso::events::KeyCode::Up) {
            obj->move({0.f, -1.f}, speed);
        } else if (keyEvent.keyCode == iso::events::KeyCode::Down) {
            obj->move({0.f, 1.f}, speed);
        }

        // TODO: this is a hack!
        // engine.removeGameObjectFromBoard(obj.get());
        // New position

        // auto isoCoords = toIso(obj->getPosition());
        // iso::math::Vector2i boardCoords;
        // boardCoords.x = int(isoCoords.x / 64);
        // boardCoords.y = int(isoCoords.y / 64);

        // std::cout << "Player on tile " << boardCoords.x << ":" << boardCoords.y << std::endl;

        // engine.addGameObjectToBoard(obj, boardCoords.y, boardCoords.x);
    };

    engine.onMouse += [&engine, &obj](iso::events::MouseEvent mouseEvent) {
        int x = mouseEvent.x;
        int y = mouseEvent.y;
        auto screenCoords = engine.screenToWorldCoords({x, y});

        auto& objCoords = obj->getPosition();

        auto moveDir = (screenCoords - objCoords).normalize();

        if (mouseEvent.eventType == iso::events::MouseEventType::MouseButtonPressed) {
            obj->move(moveDir, speed);
        }

        // auto screenCoords = engine.screenToWorldCoords({x, y});
        // auto isoCoords = toIso(screenCoords);
        // iso::math::Vector2i boardCoords;
        // boardCoords.x = int(isoCoords.x / 64);
        // boardCoords.y = int(isoCoords.y / 64);

        // std::cout << "Mouse on tile " << boardCoords.x << ":" << boardCoords.y << std::endl;
    };

    engine.run();
    return 0;
}
