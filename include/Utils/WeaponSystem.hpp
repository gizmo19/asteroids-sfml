#pragma once
#include "../Actors/WeaponPickup.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>

struct WeaponStats {
    float fireRate;
    int bulletCount;
    float spread;
    float bulletSpeed;
    float duration;
    float speedMultiplier;
};

struct WeaponRarityItem {
    WeaponType type;
    float weight;
};

class WeaponSystem {
public:
    static WeaponStats getWeaponStats(WeaponType type);
    static WeaponType getRandomWeaponType();
    static std::string getWeaponTexture(WeaponType type);

private:
    static float getTotalWeight();
    static const std::vector<WeaponRarityItem> rarityTable;
};
