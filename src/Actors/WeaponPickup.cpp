#include "../../include/Actors/WeaponPickup.hpp"
#include "../../include/Utils/Constants.hpp"
#include <cmath>

WeaponPickup::WeaponPickup(WeaponType type) : weaponType(type) {
    radius = Constants::WEAPON_PICKUP_RADIUS;
    initializeRarityData();
}

void WeaponPickup::initializeRarityData() {
    switch (weaponType) {
    case WeaponType::Rifle:
        rarityData = { WeaponRarity::Common, sf::Color::White, 1.0f, Constants::RIFLE_SPAWN_RATE };
        break;
    case WeaponType::Revolver:
        rarityData = { WeaponRarity::Uncommon, sf::Color::Green, 1.2f, Constants::REVOLVER_SPAWN_RATE };
        break;
    case WeaponType::Shotgun:
        rarityData = { WeaponRarity::Rare, sf::Color::Blue, 1.5f, Constants::SHOTGUN_SPAWN_RATE };
        break;
    case WeaponType::RocketLauncher:
        rarityData = { WeaponRarity::Epic, sf::Color(255, 165, 0), 1.8f, Constants::ROCKET_SPAWN_RATE };
        break;
    case WeaponType::Flamethrower:
        rarityData = { WeaponRarity::Legendary, sf::Color(255, 215, 0), 2.0f, Constants::FLAMETHROWER_SPAWN_RATE };
        break;
    default:
        rarityData = { WeaponRarity::Common, sf::Color::White, 1.0f, 100.0f };
        break;
    }
}

void WeaponPickup::render(sf::RenderWindow& window) {
    renderGlow(window);
    Actor::render(window);
}

void WeaponPickup::renderGlow(sf::RenderWindow& window) {
    float alpha = calculateGlowAlpha();

    sf::CircleShape outerGlow = createGlowShape(Constants::GLOW_OUTER_RADIUS, alpha * 0.3f);
    sf::CircleShape innerGlow = createGlowShape(Constants::GLOW_INNER_RADIUS, alpha * 0.6f);

    window.draw(outerGlow);
    window.draw(innerGlow);
}

float WeaponPickup::calculateGlowAlpha() const {
    float time = glowClock.getElapsedTime().asSeconds();
    float pulseSpeed = Constants::GLOW_PULSE_SPEED * rarityData.pulseIntensity;
    float pulse = (std::sin(time * pulseSpeed) + 1.0f) * 0.5f;

    return Constants::GLOW_MIN_ALPHA + (Constants::GLOW_MAX_ALPHA - Constants::GLOW_MIN_ALPHA) * pulse;
}

sf::CircleShape WeaponPickup::createGlowShape(float radius, float alpha) const {
    sf::CircleShape glow(radius);
    glow.setOrigin({ radius, radius });
    glow.setPosition(position);

    sf::Color glowColor = rarityData.glowColor;
    glowColor.a = static_cast<std::uint8_t>(alpha * 255.0f);
    glow.setFillColor(glowColor);

    return glow;
}
