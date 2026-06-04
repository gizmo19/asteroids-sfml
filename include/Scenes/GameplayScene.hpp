#pragma once
#include "Scene.hpp"
#include "../Actors/Actor.hpp"
#include "../Actors/WeaponPickup.hpp"
#include "../Controllers/ExplosionController.hpp"
#include "../Controllers/ShipController.hpp"
#include "../Actors/HeartPickup.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <memory>

class Core;
class GameplayScene : public Scene {
public:
    GameplayScene();
    GameplayScene(Core* core, sf::RenderWindow* window);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    std::shared_ptr<Actor> ship;
    std::shared_ptr<ShipController> shipController;
    std::vector<std::shared_ptr<Actor>> bullets;
    std::vector<std::shared_ptr<Actor>> asteroids;
    std::vector<std::shared_ptr<WeaponPickup>> weaponPickups;
    std::vector<std::shared_ptr<HeartPickup>> heartPickups;
    /*sf::Font font;
    sf::Text scoreText;*/
    Core* core;
    sf::Font font;
    sf::Text text;

    sf::Texture backgroundTexture;
    sf::Texture shipTexture;
    sf::Texture asteroidTexture;
    sf::Texture bulletTexture;

    sf::Texture rifleTexture;
    sf::Texture shotgunTexture;
	sf::Texture revolverTexture;
	sf::Texture flamethrowerTexture;
    sf::Texture rocketLauncherTexture;

    std::unique_ptr<sf::Sprite> backgroundSprite;

    sf::Clock weaponSpawnClock;
    sf::Clock heartSpawnClock;
    static constexpr float WEAPON_SPAWN_INTERVAL = 5.0f;

    int score;
    int lives = 3;
    bool gameOver;
    bool paused = false;
    bool escWasPressed = false;
    bool isFlashing = false;
    sf::Clock flashClock;
    sf::RenderWindow* gameWindow;

    sf::Texture heartTexture;

    sf::Text pauseTitleText;
    sf::Text pauseResumeText;
    sf::Text pauseMenuText;
    sf::Text pauseQuitText;

    void loadTextures();
    void setupBackground();
    std::shared_ptr<Actor> createShip();
    std::shared_ptr<Actor> createAsteroid();
    std::shared_ptr<Actor> createBullet(sf::Vector2f position, sf::Vector2f direction, float angle, WeaponType weaponType = WeaponType::Default);
    std::shared_ptr<WeaponPickup> createWeaponPickup();

    void createInitialAsteroids();
    void spawnNewAsteroid();
    void spawnWeaponPickup();
    void spawnHeartPickup();
    void cleanupInactiveActors();
    void handlePauseInput();
    void renderPauseMenu(sf::RenderWindow& window);

    std::shared_ptr<ExplosionController> explosionController;

    sf::Clock gameTimer;
};