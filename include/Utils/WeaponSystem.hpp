#pragma once
#include "../Actors/WeaponPickup.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

struct WeaponStats {
    float fireRate;
    int bulletCount;
    float spread;
    float bulletSpeed;
    float duration;
};

class WeaponSystem {
public:
    static WeaponStats getWeaponStats(WeaponType type);
    static WeaponType getRandomWeaponType();
    static std::string getWeaponTexture(WeaponType type);
};
