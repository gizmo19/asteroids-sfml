#pragma once
#include "Controller.hpp"
#include <SFML/Graphics.hpp>

class AsteroidController : public Controller {
public:
    AsteroidController();
    void update(float deltaTime) override;
    void setWindow(sf::RenderWindow* win) { window = win; }

private:
    float angularVelocity;
    sf::RenderWindow* window = nullptr;

    void updateMovement(float deltaTime);
    void wrapPosition();
};
