#pragma once
#include "Controller.hpp"
#include <SFML/Graphics.hpp>

class ShipController : public Controller {
public:
    ShipController();
    void update(float deltaTime) override;
    void setWindow(sf::RenderWindow* win) { window = win; }

private:
    sf::RenderWindow* window;
    float acceleration;
    float friction;
    float maxSpeed;
    bool canShoot;
    sf::Clock shootClock;

    void handleInput();
    void handleMouseRotation();
    void updateMovement(float deltaTime);
    void handleShooting();
    void constrainToBounds();
    void updateRotation();
};

