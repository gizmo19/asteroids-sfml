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

GameplayScene::GameplayScene(Core* core, sf::RenderWindow* window) : core(core), score(0), gameOver(false), gameWindow(window),
    text(sf::Text(font)), pauseTitleText(sf::Text(font)), pauseResumeText(sf::Text(font)), pauseMenuText(sf::Text(font)), pauseQuitText(sf::Text(font)) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameplayScene::initialize() {

    if (!font.openFromFile("assets/fonts/jersey.ttf")) {
        return;
    }
    sf::Text text(font);

    loadTextures();
    setupBackground();

    lives = 3;
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
    collisionController->setHeartPickups(&heartPickups);
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

    pauseTitleText = sf::Text(font, "PAUSED", 100);
    pauseTitleText.setFillColor(sf::Color::White);
    pauseTitleText.setOutlineColor(sf::Color::Black);
    pauseTitleText.setOutlineThickness(4);

    pauseResumeText = sf::Text(font, "Resume", 60);
    pauseResumeText.setOutlineColor(sf::Color::Black);
    pauseResumeText.setOutlineThickness(3);

    pauseMenuText = sf::Text(font, "Main Menu", 60);
    pauseMenuText.setOutlineColor(sf::Color::Black);
    pauseMenuText.setOutlineThickness(3);

    pauseQuitText = sf::Text(font, "Quit Game", 60);
    pauseQuitText.setOutlineColor(sf::Color::Black);
    pauseQuitText.setOutlineThickness(3);

    MessageBus::subscribe(MessageType::LifePickedUp, [this](const Message& msg) {
        if (lives < 3) lives++;
    });

    MessageBus::subscribe(MessageType::ShipDestroyed, [this](const Message& msg) {
        lives--;
        if (lives <= 0) {
            if (!gameOver) {
                gameOver = true;
                AudioManager::getInstance().playSound(Constants::Audio::DEATH_SOUND_PATH);
                AudioManager::getInstance().stopMusic();
                float survivalTime = gameTimer.getElapsedTime().asSeconds();
                core->switchToGameOverScene(score, survivalTime);
            }
        } else {
            float w = static_cast<float>(gameWindow->getSize().x);
            float h = static_cast<float>(gameWindow->getSize().y);
            ship->position = { w / 2.0f, h - 40.0f };
            ship->velocity = { 0.0f, 0.0f };
            isFlashing = true;
            flashClock.restart();
        }
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

    bool escNowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    if (escNowPressed && !escWasPressed) {
        paused = !paused;
    }
    escWasPressed = escNowPressed;

    if (paused) {
        handlePauseInput();
        return;
    }

    Scene::update(deltaTime);

    if (weaponSpawnClock.getElapsedTime().asSeconds() > Constants::WEAPON_SPAWN_INTERVAL) {
        spawnWeaponPickup();
        weaponSpawnClock.restart();
    }

    if (heartSpawnClock.getElapsedTime().asSeconds() > Constants::HEART_SPAWN_INTERVAL) {
        spawnHeartPickup();
        heartSpawnClock.restart();
    }

    Scene::cleanupInactiveActors();
    cleanupInactiveActors();

    int diffLevel = std::min(score / 500, 10);
    int targetAsteroids = Constants::MIN_ASTEROID_COUNT + diffLevel * 2;
    while (static_cast<int>(asteroids.size()) < targetAsteroids) {
        spawnNewAsteroid();
    }

    explosionController->update(deltaTime);
}

void GameplayScene::render(sf::RenderWindow& window) {
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    }

    if (isFlashing && ship && ship->getSprite()) {
        float elapsed = flashClock.getElapsedTime().asSeconds();
        if (elapsed >= 2.0f) {
            isFlashing = false;
            ship->getSprite()->setColor(sf::Color::White);
        } else {
            bool visible = (static_cast<int>(elapsed / 0.1f) % 2) == 0;
            ship->getSprite()->setColor(visible ? sf::Color::White : sf::Color(255, 255, 255, 0));
        }
    }

    Scene::render(window);

    explosionController->render(window);

    if (gameOver) {
        float winW = static_cast<float>(window.getSize().x);
        float winH = static_cast<float>(window.getSize().y);
        sf::RectangleShape line(sf::Vector2f(winW, Constants::GAME_OVER_LINE_HEIGHT));
        line.setPosition({ 0.0f, winH - Constants::GAME_OVER_LINE_HEIGHT });
        line.setFillColor(sf::Color::Red);
        window.draw(line);
    }

    text.setCharacterSize(48);
    text.setOutlineThickness(3);
    text.setPosition({ 32.f, 16.f });
    text.setFillColor(sf::Color::Yellow);
    text.setString("Score: " + std::to_string(score));
    window.draw(text);

    if (shipController) {
        float winH = static_cast<float>(window.getSize().y);

        const float BAR_W = 220.0f;
        const float BAR_H = 16.0f;
        const float MARGIN_X = 16.0f;
        const float AMMO_H = 20.0f;
        const float BORDER = 2.0f;
        const float AMMO_GAP = 2.0f;
        float blockBottom = winH - 20.0f;
        float ammoY = blockBottom - AMMO_H;
        float barY = ammoY - BORDER * 2.0f - AMMO_GAP - BAR_H;
        float barX = MARGIN_X;

        float ratio;
        if (shipController->hasSpecialWeapon())
            ratio = std::max(0.0f, std::min(1.0f, shipController->getWeaponTimeLeft() / shipController->getWeaponTotalTime()));
        else if (shipController->getIsReloading())
            ratio = shipController->getReloadProgress();
        else
            ratio = 1.0f;

        sf::RectangleShape border(sf::Vector2f(BAR_W + BORDER * 2.0f, BAR_H + BORDER * 2.0f));
        border.setPosition({ barX - BORDER, barY - BORDER });
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color::White);
        border.setOutlineThickness(BORDER);
        window.draw(border);

        sf::RectangleShape bar(sf::Vector2f(BAR_W * ratio, BAR_H));
        bar.setPosition({ barX, barY });
        bar.setFillColor(sf::Color(255, 140, 0));
        window.draw(bar);

        {
            int ammo = shipController->getCurrentAmmo();
            int ammoMax = shipController->getMaxAmmo();
            if (ammoMax > 0) {
                float cellW = (BAR_W - (ammoMax - 1) * AMMO_GAP) / static_cast<float>(ammoMax);

                sf::RectangleShape ammoBorder(sf::Vector2f(BAR_W + BORDER * 2.0f, AMMO_H + BORDER * 2.0f));
                ammoBorder.setPosition({ barX - BORDER, ammoY - BORDER });
                ammoBorder.setFillColor(sf::Color::Transparent);
                ammoBorder.setOutlineColor(sf::Color::White);
                ammoBorder.setOutlineThickness(BORDER);
                window.draw(ammoBorder);

                for (int i = 0; i < ammoMax; ++i) {
                    sf::RectangleShape cell(sf::Vector2f(cellW, AMMO_H));
                    cell.setPosition({ barX + i * (cellW + AMMO_GAP), ammoY });
                    cell.setFillColor(i < ammo ? sf::Color(255, 140, 0) : sf::Color(60, 60, 60));
                    window.draw(cell);
                }
            }
        }

        const int MAX_LIVES = 3;
        const float HEART_SIZE = (BAR_W - 2.0f * (MAX_LIVES - 1)) / MAX_LIVES;
        float heartsY = barY - HEART_SIZE - 6.0f;
        float heartStep = (BAR_W - HEART_SIZE) / (MAX_LIVES - 1);
        sf::Vector2u heartTexSize = heartTexture.getSize();
        float heartScale = HEART_SIZE / static_cast<float>(heartTexSize.x);
        for (int i = 0; i < MAX_LIVES; ++i) {
            if (i >= lives) continue;
            sf::Sprite heart(heartTexture);
            heart.setScale({ heartScale, heartScale });
            heart.setPosition({ barX + i * heartStep, heartsY });
            window.draw(heart);
        }
    }

    if (paused) {
        renderPauseMenu(window);
    }
}

std::shared_ptr<Actor> GameplayScene::createShip() {
    auto actor = std::make_shared<Actor>();
    float w = static_cast<float>(gameWindow->getSize().x);
    float h = static_cast<float>(gameWindow->getSize().y);
    actor->position = { w / 2.0f, h - 40.0f };
    actor->radius = Constants::SHIP_RADIUS;

    actor->setTexture(shipTexture);
    sf::Vector2u size = shipTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

    auto controller = std::make_shared<ShipController>();
    controller->setWindow(gameWindow);
    actor->addController(controller);
    shipController = controller;

    return actor;
}

std::shared_ptr<Actor> GameplayScene::createAsteroid() {
    auto actor = std::make_shared<Actor>();
    actor->radius = Constants::MIN_ASTEROID_RADIUS + std::rand() % Constants::ASTEROID_RADIUS_RANGE;
    int winW = static_cast<int>(gameWindow->getSize().x);
    actor->position = {
        static_cast<float>(std::rand() % (winW - Constants::ASTEROID_SPAWN_MARGIN) + Constants::ASTEROID_SPAWN_MIN_X),
        -actor->radius
    };
    float diffMult = 1.0f + std::min(score / 500, 10) * 0.15f;
    actor->velocity = {
        (static_cast<float>(std::rand() % Constants::ASTEROID_VELOCITY_RANGE) - 100.0f) / Constants::ASTEROID_VELOCITY_DIVISOR * diffMult,
        (Constants::BASE_ASTEROID_SPEED + static_cast<float>(std::rand() % Constants::ASTEROID_SPEED_RANGE) / Constants::ASTEROID_SPEED_DIVISOR) * diffMult
    };

    actor->setTexture(asteroidTexture);
    sf::Vector2u size = asteroidTexture.getSize();
    actor->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    float scale = (actor->radius * Constants::ASTEROID_SCALE_MULTIPLIER) / static_cast<float>(size.x);
    actor->setScale({ scale, scale });

    auto controller = std::make_shared<AsteroidController>();
    controller->setWindow(gameWindow);
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
    controller->setWindow(gameWindow);
    actor->addController(controller);

    return actor;
}

std::shared_ptr<WeaponPickup> GameplayScene::createWeaponPickup() {
    WeaponType weaponType = WeaponSystem::getRandomWeaponType();
    auto weaponPickup = std::make_shared<WeaponPickup>(weaponType);

    int winW = static_cast<int>(gameWindow->getSize().x);
    int winH = static_cast<int>(gameWindow->getSize().y);
    int spawnHeightMin = winH / 12;
    int spawnHeightMax = winH * 5 / 6;
    weaponPickup->position = {
        static_cast<float>(std::rand() % (winW - Constants::WEAPON_SPAWN_MARGIN * 2) + Constants::WEAPON_SPAWN_MARGIN),
        static_cast<float>(std::rand() % (spawnHeightMax - spawnHeightMin) + spawnHeightMin)
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
    float w = static_cast<float>(gameWindow->getSize().x);
    float h = static_cast<float>(gameWindow->getSize().y);
    backgroundSprite->setScale({
        w / static_cast<float>(backgroundTexture.getSize().x),
        h / static_cast<float>(backgroundTexture.getSize().y)
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

    heartPickups.erase(std::remove_if(heartPickups.begin(), heartPickups.end(),
        [](const std::shared_ptr<HeartPickup>& actor) { return !actor->active; }), heartPickups.end());
}

void GameplayScene::handlePauseInput() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*gameWindow);
    bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    auto isHovered = [&](const sf::Text& text) {
        return text.getGlobalBounds().contains(sf::Vector2f(mousePos));
    };

    static bool wasClicked = false;
    bool justClicked = clicked && !wasClicked;
    wasClicked = clicked;

    if (justClicked) {
        if (isHovered(pauseResumeText)) {
            paused = false;
            escWasPressed = true;
        } else if (isHovered(pauseMenuText)) {
            paused = false;
            core->switchToMenu();
        } else if (isHovered(pauseQuitText)) {
            gameWindow->close();
        }
    }
}

void GameplayScene::renderPauseMenu(sf::RenderWindow& window) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    sf::RectangleShape overlay(sf::Vector2f(winW, winH));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    auto centerText = [&](sf::Text& t, float y) {
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition({ (winW - b.size.x) / 2.0f, y });
    };

    auto applyHover = [&](sf::Text& t, sf::Color normal, sf::Color hover) {
        t.setFillColor(t.getGlobalBounds().contains(sf::Vector2f(mousePos)) ? hover : normal);
    };

    centerText(pauseTitleText, winH * 0.25f);
    window.draw(pauseTitleText);

    float optionY = winH * 0.45f;
    float optionStep = winH * 0.1f;

    centerText(pauseResumeText, optionY);
    applyHover(pauseResumeText, sf::Color::White, sf::Color::Yellow);
    window.draw(pauseResumeText);

    centerText(pauseMenuText, optionY + optionStep);
    applyHover(pauseMenuText, sf::Color::White, sf::Color::Yellow);
    window.draw(pauseMenuText);

    centerText(pauseQuitText, optionY + optionStep * 2.0f);
    applyHover(pauseQuitText, sf::Color::White, sf::Color::Yellow);
    window.draw(pauseQuitText);
}

void GameplayScene::spawnHeartPickup() {
    auto heart = std::make_shared<HeartPickup>();

    int winW = static_cast<int>(gameWindow->getSize().x);
    int winH = static_cast<int>(gameWindow->getSize().y);
    heart->position = {
        static_cast<float>(std::rand() % (winW - Constants::WEAPON_SPAWN_MARGIN * 2) + Constants::WEAPON_SPAWN_MARGIN),
        static_cast<float>(std::rand() % (winH * 5 / 6 - winH / 12) + winH / 12)
    };

    heart->setTexture(heartTexture);
    sf::Vector2u size = heartTexture.getSize();
    heart->setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
    heart->setScale({ Constants::WEAPON_SCALE, Constants::WEAPON_SCALE });

    auto controller = std::make_shared<WeaponPickupController>();
    heart->addController(controller);

    heartPickups.push_back(heart);
    addActor(heart);
}

void GameplayScene::loadTextures() {
    backgroundTexture.loadFromFile("assets/images/background.png");
    shipTexture.loadFromFile("assets/images/ship.png");
    asteroidTexture.loadFromFile("assets/images/asteroid.png");
    bulletTexture.loadFromFile("assets/images/bullet.png");
    heartTexture.loadFromFile("assets/images/heart.png");

    rifleTexture.loadFromFile("assets/images/rifle.png");
    shotgunTexture.loadFromFile("assets/images/shotgun.png");
    revolverTexture.loadFromFile("assets/images/revolver.png");
    flamethrowerTexture.loadFromFile("assets/images/flamethrower.png");
    rocketLauncherTexture.loadFromFile("assets/images/rocket_launcher.png");
}
