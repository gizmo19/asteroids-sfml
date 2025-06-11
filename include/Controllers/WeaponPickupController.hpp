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

    static constexpr float MAX_LIFETIME = 15.0f;
    static constexpr float BOB_AMPLITUDE = 10.0f;
    static constexpr float BOB_SPEED = 3.0f;
};
