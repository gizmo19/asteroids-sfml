#include "../../include/Scenes/GameplayScene.hpp"
#include "../../include/Core/Core.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Controllers/ShipController.hpp"
#include "../../include/Controllers/AsteroidController.hpp"
#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Controllers/CollisionController.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>


GameplayScene::GameplayScene(Core* core, sf::RenderWindow* window): core(core), score(0), gameOver(false), gameWindow(window), text(sf::Text(font)) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameplayScene::initialize() {
    if (!font.openFromFile("assets/fonts/jersey.ttf")) {
        return;
    }
    sf::Text text(font);

    loadTextures();
    setupBackground();

    ship = createShip();
    addActor(ship);

    createInitialAsteroids();

    auto collisionController = std::make_shared<CollisionController>();
    collisionController->setShip(ship);
    collisionController->setBullets(&bullets);
    collisionController->setAsteroids(&asteroids);
    addController(collisionController);

    MessageBus::subscribe(MessageType::BulletFired, [this](const Message& msg) {
        sf::Vector2f shipPos = std::any_cast<sf::Vector2f>(msg.payload);

        sf::Vector2i mousePos = sf::Mouse::getPosition(*gameWindow);
        float dx = static_cast<float>(mousePos.x) - shipPos.x;
        float dy = static_cast<float>(mousePos.y) - shipPos.y;
        float angle = std::atan2(dy, dx) * 180.0f / 3.14159f + 90.0f;

        float angleRad = (angle - 90.0f) * 3.14159f / 180.0f;
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        auto bullet = createBullet(shipPos, direction, angle);
        bullets.push_back(bullet);
        addActor(bullet);
    });

    MessageBus::subscribe(MessageType::AsteroidDestroyed, [this](const Message& msg) {
        int points = std::any_cast<int>(msg.payload);
        score += points;
        spawnNewAsteroid();
        });

    MessageBus::subscribe(MessageType::GameOver, [this](const Message& msg) {
        core->switchToGameOverScene();
      });
}

void GameplayScene::update(float deltaTime) {
    if (gameOver) return;

    Scene::update(deltaTime);

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


    text.setCharacterSize(48); 
    text.setOutlineThickness(3);
    text.setPosition({ 32.f, 16.f });
    text.setFillColor(sf::Color::Yellow);
    text.setString("Score: " + std::to_string(score));
    window.draw(text);
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

std::shared_ptr<Actor> GameplayScene::createBullet(sf::Vector2f position, sf::Vector2f direction, float angle) {
    auto actor = std::make_shared<Actor>();
    actor->radius = 6.0f;
    actor->position = position + direction * 28.0f;
    actor->velocity = direction * 10.0f + ship->velocity * 0.5f;
    actor->rotation = angle;

    actor->setTexture(bulletTexture);
    sf::Vector2u size = bulletTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * 8.0f) / static_cast<float>(size.x);
    actor->setScale({ scale, scale });

    auto controller = std::make_shared<BulletController>();
    actor->addController(controller);

    return actor;
}

void GameplayScene::loadTextures() {
    backgroundTexture.loadFromFile("assets/images/background.png");
    shipTexture.loadFromFile("assets/images/ship.png");
    asteroidTexture.loadFromFile("assets/images/asteroid.png");
    bulletTexture.loadFromFile("assets/images/bullet.png");
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

void GameplayScene::cleanupInactiveActors() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const std::shared_ptr<Actor>& actor) { return !actor->active; }), bullets.end());

    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
        [](const std::shared_ptr<Actor>& actor) {
            return !actor->active;
        }), asteroids.end());
}
