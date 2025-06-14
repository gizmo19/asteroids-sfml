#include "../../include/Scenes/GameplayScene.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Actors/WeaponPickup.hpp"
#include "../../include/Controllers/ShipController.hpp"
#include "../../include/Controllers/AsteroidController.hpp"
#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Controllers/WeaponPickupController.hpp"
#include "../../include/Controllers/CollisionController.hpp"
#include "../../include/Controllers/ExplosionController.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include "../../include/Utils/WeaponSystem.hpp"
#include "../../include/Utils/MessageData.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Core/Core.hpp"
#include "../../include/Controllers/AudioController.hpp"
#include "../../include/Utils/AudioManager.hpp"
#include "../../include/Core/Core.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>

GameplayScene::GameplayScene(Core* core, sf::RenderWindow* window) : core(core), score(0), gameOver(false), gameWindow(window), text(sf::Text(font)){
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameplayScene::initialize() {

    if (!font.openFromFile("assets/fonts/jersey.ttf")) {
        return;
    }
    sf::Text text(font);

    loadTextures();
    setupBackground();

    gameTimer.restart();
    printf("Game started! Timer running...\n");

    ship = createShip();
    addActor(ship);

    createInitialAsteroids();

    auto collisionController = std::make_shared<CollisionController>();
    collisionController->setShip(ship);
    collisionController->setBullets(&bullets);
    collisionController->setAsteroids(&asteroids);
    collisionController->setWeaponPickups(&weaponPickups);
    addController(collisionController);

    explosionController = std::make_shared<ExplosionController>();
    explosionController->setAsteroids(&asteroids);
    addController(explosionController);
    auto audioController = std::make_shared<AudioController>(Constants::Audio::SOUNDTRACK_PATH);
    addController(audioController);

    MessageBus::subscribe(MessageType::BulletFired, [this](const Message& msg) {
        BulletData data = std::any_cast<BulletData>(msg.payload);

        float angleRad = (data.angle - Constants::BULLET_ANGLE_OFFSET) * Constants::DEGREES_TO_RADIANS;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        auto bullet = createBullet(data.position, direction, data.angle, data.weaponType);
        bullet->velocity = direction * data.speed + ship->velocity * Constants::BULLET_VELOCITY_INFLUENCE;

        AudioManager::getInstance().playSound(Constants::Audio::LASER_SOUND_PATH);

        bullets.push_back(bullet);
        addActor(bullet);
        });

    MessageBus::subscribe(MessageType::AsteroidDestroyed, [this](const Message& msg) {
        int points = std::any_cast<int>(msg.payload);
        score += points;
        spawnNewAsteroid();
        });

    MessageBus::subscribe(MessageType::GameOver, [this](const Message& msg) {
        if (!gameOver) {
            gameOver = true;
          
            float survivalTime = gameTimer.getElapsedTime().asSeconds();
            core->switchToGameOverScene(score, survivalTime);
            printf("\n=== GAME OVER ===\n");
            printf("You survived for: %.2f seconds\n", survivalTime);
            printf("Final score: %d\n", score);
            printf("================\n");
        }
    });
}

void GameplayScene::update(float deltaTime) {
    if (gameOver) return;

    Scene::update(deltaTime);

    if (weaponSpawnClock.getElapsedTime().asSeconds() > Constants::WEAPON_SPAWN_INTERVAL) {
        spawnWeaponPickup();
        weaponSpawnClock.restart();
    }

    Scene::cleanupInactiveActors();
    cleanupInactiveActors();

    while (asteroids.size() < Constants::MIN_ASTEROID_COUNT) {
        spawnNewAsteroid();
    }

    explosionController->update(deltaTime);
}

void GameplayScene::render(sf::RenderWindow& window) {
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    }
    Scene::render(window);

    explosionController->render(window);

    if (gameOver) {
        sf::RectangleShape line(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::GAME_OVER_LINE_HEIGHT));
        line.setPosition({ 0.0f, Constants::GAME_OVER_LINE_Y });
        line.setFillColor(sf::Color::Red);
        window.draw(line);
    }

    text.setCharacterSize(48);
    text.setOutlineThickness(3);
    text.setPosition({ 32.f, 16.f });
    text.setFillColor(sf::Color::Yellow);
    text.setString("Score: " + std::to_string(score));
    window.draw(text);
}

std::shared_ptr<Actor> GameplayScene::createShip() {
    auto actor = std::make_shared<Actor>();
    actor->position = { Constants::SHIP_START_X, Constants::SHIP_START_Y };
    actor->radius = Constants::SHIP_RADIUS;

    actor->setTexture(shipTexture);
    sf::Vector2u size = shipTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

    auto controller = std::make_shared<ShipController>();
    controller->setWindow(gameWindow);
    actor->addController(controller);

    return actor;
}

std::shared_ptr<Actor> GameplayScene::createAsteroid() {
    auto actor = std::make_shared<Actor>();
    actor->radius = Constants::MIN_ASTEROID_RADIUS + std::rand() % Constants::ASTEROID_RADIUS_RANGE;
    actor->position = {
        static_cast<float>(std::rand() % (Constants::WINDOW_WIDTH - Constants::ASTEROID_SPAWN_MARGIN) + Constants::ASTEROID_SPAWN_MIN_X),
        -actor->radius
    };
    actor->velocity = {
        (static_cast<float>(std::rand() % Constants::ASTEROID_VELOCITY_RANGE) - 100.0f) / Constants::ASTEROID_VELOCITY_DIVISOR,
        Constants::BASE_ASTEROID_SPEED + static_cast<float>(std::rand() % Constants::ASTEROID_SPEED_RANGE) / Constants::ASTEROID_SPEED_DIVISOR
    };

    actor->setTexture(asteroidTexture);
    sf::Vector2u size = asteroidTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * Constants::ASTEROID_SCALE_MULTIPLIER) / static_cast<float>(size.x);
    actor->setScale({ scale, scale });

    auto controller = std::make_shared<AsteroidController>();
    actor->addController(controller);

    return actor;
}

std::shared_ptr<Actor> GameplayScene::createBullet(sf::Vector2f position, sf::Vector2f direction, float angle, WeaponType weaponType) {
    auto actor = std::make_shared<Actor>();
    actor->radius = Constants::BULLET_RADIUS;
    actor->position = position + direction * Constants::BULLET_POSITION_OFFSET;
    actor->rotation = angle;
    actor->weaponType = weaponType;

    actor->setTexture(bulletTexture);
    sf::Vector2u size = bulletTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * Constants::BULLET_SCALE_MULTIPLIER) / static_cast<float>(size.x);

    if (weaponType == WeaponType::RocketLauncher) {
        actor->setScale({ scale * 2, scale * 2 });
    }
    else {
        actor->setScale({ scale, scale });
    }

    sf::Color weaponColor;
    switch (weaponType) {
    case WeaponType::Rifle:
        weaponColor = sf::Color::Yellow;
        break;
    case WeaponType::Revolver:
        weaponColor = sf::Color::Red;
        break;
    case WeaponType::Shotgun:
        weaponColor = sf::Color::Green;
        break;
    case WeaponType::RocketLauncher:
        weaponColor = sf::Color::Magenta;
        break;
    default:
        weaponColor = sf::Color::White;
        break;
    }
    actor->getSprite()->setColor(weaponColor);

    auto controller = std::make_shared<BulletController>(weaponType);
    actor->addController(controller);

    return actor;
}

std::shared_ptr<WeaponPickup> GameplayScene::createWeaponPickup() {
    WeaponType weaponType = WeaponSystem::getRandomWeaponType();
    auto weaponPickup = std::make_shared<WeaponPickup>(weaponType);

    weaponPickup->position = {
        static_cast<float>(std::rand() % (Constants::WINDOW_WIDTH - Constants::WEAPON_SPAWN_MARGIN * 2) + Constants::WEAPON_SPAWN_MARGIN),
        static_cast<float>(std::rand() % (Constants::WEAPON_SPAWN_HEIGHT_MAX - Constants::WEAPON_SPAWN_HEIGHT_MIN) + Constants::WEAPON_SPAWN_HEIGHT_MIN)
    };

    sf::Texture* selectedTexture;
    switch (weaponType) {
    case WeaponType::Rifle:
        selectedTexture = &rifleTexture;
        break;
    case WeaponType::Revolver:
        selectedTexture = &revolverTexture;
        break;
    case WeaponType::Shotgun:
        selectedTexture = &shotgunTexture;
        break;
    case WeaponType::Flamethrower:
        selectedTexture = &flamethrowerTexture;
        break;
    case WeaponType::RocketLauncher:
        selectedTexture = &rocketLauncherTexture;
        break;
    default:
        selectedTexture = &bulletTexture;
        break;
    }

    weaponPickup->setTexture(*selectedTexture);
    sf::Vector2u size = selectedTexture->getSize();
    weaponPickup->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    weaponPickup->setScale({ Constants::WEAPON_SCALE, Constants::WEAPON_SCALE });

    auto controller = std::make_shared<WeaponPickupController>();
    weaponPickup->addController(controller);

    return weaponPickup;
}

void GameplayScene::setupBackground() {
    backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
    backgroundSprite->setScale({
        Constants::WINDOW_WIDTH / static_cast<float>(backgroundTexture.getSize().x),
        Constants::WINDOW_HEIGHT / static_cast<float>(backgroundTexture.getSize().y)
        });
}

void GameplayScene::createInitialAsteroids() {
    for (int i = 0; i < Constants::INITIAL_ASTEROID_COUNT; ++i) {
        auto asteroid = createAsteroid();
        asteroid->position.y = static_cast<float>(-std::rand() % Constants::MAX_ASTEROID_SPAWN_OFFSET);
        asteroids.push_back(asteroid);
        addActor(asteroid);
    }
}

void GameplayScene::spawnNewAsteroid() {
    auto asteroid = createAsteroid();
    asteroids.push_back(asteroid);
    addActor(asteroid);
}

void GameplayScene::spawnWeaponPickup() {
    auto weaponPickup = createWeaponPickup();
    weaponPickups.push_back(weaponPickup);
    addActor(weaponPickup);
}

void GameplayScene::cleanupInactiveActors() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::shared_ptr<Actor>& actor) { return !actor->active; }), bullets.end());

    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
        [](const std::shared_ptr<Actor>& actor) { return !actor->active; }), asteroids.end());

    weaponPickups.erase(std::remove_if(weaponPickups.begin(), weaponPickups.end(),
        [](const std::shared_ptr<WeaponPickup>& actor) { return !actor->active; }), weaponPickups.end());
}

void GameplayScene::loadTextures() {
    backgroundTexture.loadFromFile("assets/images/background.png");
    shipTexture.loadFromFile("assets/images/ship.png");
    asteroidTexture.loadFromFile("assets/images/asteroid.png");
    bulletTexture.loadFromFile("assets/images/bullet.png");

    rifleTexture.loadFromFile("assets/images/rifle.png");
    shotgunTexture.loadFromFile("assets/images/shotgun.png");
    revolverTexture.loadFromFile("assets/images/revolver.png");
    flamethrowerTexture.loadFromFile("assets/images/flamethrower.png");
    rocketLauncherTexture.loadFromFile("assets/images/rocket_launcher.png");
}
