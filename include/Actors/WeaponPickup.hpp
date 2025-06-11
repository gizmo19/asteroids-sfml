#pragma once
#include "Actor.hpp"
#include <SFML/System/Clock.hpp>

enum class WeaponType {
    Default,
    Rifle,
    Revolver,
    Shotgun,
    RocketLauncher,
    Flamethrower
};

enum class WeaponRarity {
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
};

struct RarityData {
    WeaponRarity rarity;
    sf::Color glowColor;
    float pulseIntensity;
    float spawnRate;
};

class WeaponPickup : public Actor {
public:
    WeaponPickup(WeaponType type);
    WeaponType getWeaponType() const { return weaponType; }
    WeaponRarity getRarity() const { return rarityData.rarity; }

    void render(sf::RenderWindow& window) override;

private:
    WeaponType weaponType;
    RarityData rarityData;
    sf::Clock glowClock;

    void initializeRarityData();
    void renderGlow(sf::RenderWindow& window);
    float calculateGlowAlpha() const;
    sf::CircleShape createGlowShape(float radius, float alpha) const;
};
