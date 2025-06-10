#pragma once
#include <SFML/System/Vector2.hpp>
#include "../Actors/WeaponPickup.hpp"

struct BulletData {
    sf::Vector2f position;
    float angle;
    float speed;
    WeaponType weaponType;
};
