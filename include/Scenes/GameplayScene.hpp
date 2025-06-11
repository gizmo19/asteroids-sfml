#pragma once
#include "Scene.hpp"
#include "../Actors/Actor.hpp"
#include "../Actors/WeaponPickup.hpp"
#include "../Controllers/ExplosionController.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <memory>

class GameplayScene : public Scene {
public:
    GameplayScene(sf::RenderWindow* window);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    std::shared_ptr<Actor> ship;
    std::vector<std::shared_ptr<Actor>> bullets;
    std::vector<std::shared_ptr<Actor>> asteroids;
    std::vector<std::shared_ptr<WeaponPickup>> weaponPickups;
    /*sf::Font font;
    sf::Text scoreText;*/
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
    static constexpr float WEAPON_SPAWN_INTERVAL = 5.0f;

    int score;
    bool gameOver;
    sf::RenderWindow* gameWindow;

    void loadTextures();
    void setupBackground();
    std::shared_ptr<Actor> createShip();
    std::shared_ptr<Actor> createAsteroid();
    std::shared_ptr<Actor> createBullet(sf::Vector2f position, sf::Vector2f direction, float angle, WeaponType weaponType = WeaponType::Default);
    std::shared_ptr<WeaponPickup> createWeaponPickup();

    void createInitialAsteroids();
    void spawnNewAsteroid();
    void spawnWeaponPickup();
    void cleanupInactiveActors();

    std::shared_ptr<ExplosionController> explosionController;

    sf::Clock gameTimer;
};