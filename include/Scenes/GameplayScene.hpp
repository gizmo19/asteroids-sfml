#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class GameplayScene : public Scene {
public:
    GameplayScene();
    GameplayScene(sf::RenderWindow* window);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    std::shared_ptr<Actor> createShip();
    std::shared_ptr<Actor> createAsteroid();
    std::shared_ptr<Actor> createBullet(sf::Vector2f position, sf::Vector2f direction, float angle);

    std::vector<std::shared_ptr<Actor>>& getBullets() { return bullets; }
    std::vector<std::shared_ptr<Actor>>& getAsteroids() { return asteroids; }
    std::shared_ptr<Actor> getShip() { return ship; }

private:
    sf::Texture backgroundTexture;
    sf::Texture shipTexture;
    sf::Texture asteroidTexture;
    sf::Texture bulletTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;

    sf::RenderWindow* gameWindow;

    std::shared_ptr<Actor> ship;
    std::vector<std::shared_ptr<Actor>> bullets;
    std::vector<std::shared_ptr<Actor>> asteroids;

    int score;
    bool gameOver;

    void loadTextures();
    void setupBackground();
    void createInitialAsteroids();
    void spawnNewAsteroid();
    void cleanupInactiveActors();
};
