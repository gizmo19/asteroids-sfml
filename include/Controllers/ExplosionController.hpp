#pragma once
#include "Controller.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Actor;

class ExplosionController : public Controller {
public:
    ExplosionController();

    void update(float deltaTime) override;
    void setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroidList);
    void render(sf::RenderWindow& window);

private:
    std::vector<std::shared_ptr<Actor>>* asteroids;

    std::vector<sf::CircleShape> explosionEffects;
    std::vector<float> explosionTimers;

    void handleExplosion(sf::Vector2f position, float radius);
};
