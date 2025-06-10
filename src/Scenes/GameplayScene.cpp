#include "../../include/Scenes/GameplayScene.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Actors/WeaponPickup.hpp"
#include "../../include/Controllers/ShipController.hpp"
#include "../../include/Controllers/AsteroidController.hpp"
#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Controllers/WeaponPickupController.hpp"
#include "../../include/Controllers/CollisionController.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include "../../include/Utils/WeaponSystem.hpp"
#include "../../include/Utils/MessageData.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

GameplayScene::GameplayScene(sf::RenderWindow* window) : score(0), gameOver(false), gameWindow(window) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameplayScene::initialize() {
    loadTextures();
    setupBackground();

    ship = createShip();
    addActor(ship);

    createInitialAsteroids();

    auto collisionController = std::make_shared<CollisionController>();
    collisionController->setShip(ship);
    collisionController->setBullets(&bullets);
    collisionController->setAsteroids(&asteroids);
    collisionController->setWeaponPickups(&weaponPickups);
    addController(collisionController);

    MessageBus::subscribe(MessageType::BulletFired, [this](const Message& msg) {
        BulletData data = std::any_cast<BulletData>(msg.payload);

        float angleRad = (data.angle - 90.0f) * 3.14159f / 180.0f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        auto bullet = createBullet(data.position, direction, data.angle, data.weaponType);
        bullet->velocity = direction * data.speed + ship->velocity * 0.5f;
        bullets.push_back(bullet);
        addActor(bullet);
        });

    MessageBus::subscribe(MessageType::AsteroidDestroyed, [this](const Message& msg) {
        int points = std::any_cast<int>(msg.payload);
        score += points;
        spawnNewAsteroid();
        });

    MessageBus::subscribe(MessageType::GameOver, [this](const Message& msg) {
        gameOver = true;
        });
}

void GameplayScene::update(float deltaTime) {
    if (gameOver) return;

    Scene::update(deltaTime);

    if (weaponSpawnClock.getElapsedTime().asSeconds() > WEAPON_SPAWN_INTERVAL) {
        spawnWeaponPickup();
        weaponSpawnClock.restart();
    }

    Scene::cleanupInactiveActors();
    cleanupInactiveActors();

    while (asteroids.size() < 12) {
        spawnNewAsteroid();
    }
}

void GameplayScene::render(sf::RenderWindow& window) {
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    }
    Scene::render(window);

    if (gameOver) {
        sf::RectangleShape line(sf::Vector2f(1600, 8));
        line.setPosition({ 0.0f, 1192.0f });
        line.setFillColor(sf::Color::Red);
        window.draw(line);
    }
}

std::shared_ptr<Actor> GameplayScene::createShip() {
    auto actor = std::make_shared<Actor>();
    actor->position = { 800.0f, 1160.0f };
    actor->radius = 16.0f;

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
    actor->radius = 20.0f + std::rand() % 25;
    actor->position = {
        static_cast<float>(std::rand() % (1600 - 40) + 20),
        -actor->radius
    };
    actor->velocity = {
        (static_cast<float>(std::rand() % 200) - 100.0f) / 50.0f,
        2.0f + static_cast<float>(std::rand() % 15) / 3.0f
    };

    actor->setTexture(asteroidTexture);
    sf::Vector2u size = asteroidTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * 2.0f) / static_cast<float>(size.x);
    actor->setScale({ scale, scale });

    auto controller = std::make_shared<AsteroidController>();
    actor->addController(controller);

    return actor;
}

std::shared_ptr<Actor> GameplayScene::createBullet(sf::Vector2f position, sf::Vector2f direction, float angle, WeaponType weaponType) {
    auto actor = std::make_shared<Actor>();
    actor->radius = 6.0f;
    actor->position = position + direction * 28.0f;
    actor->rotation = angle;

    actor->setTexture(bulletTexture);
    sf::Vector2u size = bulletTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * 8.0f) / static_cast<float>(size.x);
    actor->setScale({ scale, scale });

    sf::Color weaponColor;
    switch (weaponType) {
    case WeaponType::Minigun:
        weaponColor = sf::Color::Yellow;
        break;
    case WeaponType::Bazooka:
        weaponColor = sf::Color::Red;
        break;
    case WeaponType::Shotgun:
        weaponColor = sf::Color::Green;
        break;
    default:
        weaponColor = sf::Color::White;
        break;
    }
    actor->getSprite()->setColor(weaponColor);

    auto controller = std::make_shared<BulletController>();
    actor->addController(controller);

    return actor;
}

std::shared_ptr<WeaponPickup> GameplayScene::createWeaponPickup() {
    WeaponType weaponType = WeaponSystem::getRandomWeaponType();
    auto weaponPickup = std::make_shared<WeaponPickup>(weaponType);

    weaponPickup->position = {
        static_cast<float>(std::rand() % (1600 - 100) + 50),
        static_cast<float>(std::rand() % (1000 - 200) + 100)
    };

    sf::Texture* selectedTexture;
    switch (weaponType) {
    case WeaponType::Minigun:
        selectedTexture = &rifleTexture;
        break;
    case WeaponType::Bazooka:
        selectedTexture = &rocketTexture;
        break;
    case WeaponType::Shotgun:
        selectedTexture = &shotgunTexture;
        break;
    default:
        selectedTexture = &bulletTexture;
        break;
    }

    weaponPickup->setTexture(*selectedTexture);
    sf::Vector2u size = selectedTexture->getSize();
    weaponPickup->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    weaponPickup->setScale({ 2.0f, 2.0f });

    auto controller = std::make_shared<WeaponPickupController>();
    weaponPickup->addController(controller);

    return weaponPickup;
}

void GameplayScene::loadTextures() {
    backgroundTexture.loadFromFile("assets/images/background.png");
    shipTexture.loadFromFile("assets/images/ship.png");
    asteroidTexture.loadFromFile("assets/images/asteroid.png");
    bulletTexture.loadFromFile("assets/images/bullet.png");
    rifleTexture.loadFromFile("assets/images/rifle.png");
    rocketTexture.loadFromFile("assets/images/rocket.png");
    shotgunTexture.loadFromFile("assets/images/shotgun.png");
}

void GameplayScene::setupBackground() {
    backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
    backgroundSprite->setScale({
        1600.0f / backgroundTexture.getSize().x,
        1200.0f / backgroundTexture.getSize().y
        });
}

void GameplayScene::createInitialAsteroids() {
    for (int i = 0; i < 12; ++i) {
        auto asteroid = createAsteroid();
        asteroid->position.y = static_cast<float>(-std::rand() % 2400);
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