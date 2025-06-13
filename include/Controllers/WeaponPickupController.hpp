#pragma once
#include "Controller.hpp"
#include <SFML/System/Clock.hpp>

class WeaponPickupController : public Controller {
public:
    WeaponPickupController();
    void update(float deltaTime) override;

private:
    sf::Clock bobClock;
    sf::Clock lifetimeClock;
    float originalY;
    bool isOriginalYSet;

    void updateBobbing();
    void checkLifetime();
};
