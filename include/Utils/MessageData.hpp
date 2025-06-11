#pragma once
#include <SFML/Graphics.hpp>
#include <any>

enum class WeaponType;

struct BulletData {
    sf::Vector2f position;
    float angle;
    float speed;
    WeaponType weaponType;
};

struct ExplosionData {
    sf::Vector2f position;
    float radius;
    WeaponType weaponType;
};
