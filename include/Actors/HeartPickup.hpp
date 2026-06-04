#pragma once
#include "Actor.hpp"
#include <SFML/System/Clock.hpp>

class HeartPickup : public Actor {
public:
    HeartPickup();
    void render(sf::RenderWindow& window) override;

private:
    sf::Clock glowClock;
};
