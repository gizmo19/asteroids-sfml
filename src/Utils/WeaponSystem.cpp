#include "../../include/Utils/WeaponSystem.hpp"
#include "../../include/Utils/Constants.hpp"
#include <random>

const std::vector<WeaponRarityItem> WeaponSystem::rarityTable = {
    { WeaponType::Rifle, Constants::RIFLE_SPAWN_RATE },
    { WeaponType::Revolver, Constants::REVOLVER_SPAWN_RATE },
    { WeaponType::Shotgun, Constants::SHOTGUN_SPAWN_RATE },
    { WeaponType::RocketLauncher, Constants::ROCKET_SPAWN_RATE },
    { WeaponType::Flamethrower, Constants::FLAMETHROWER_SPAWN_RATE }
};

WeaponStats WeaponSystem::getWeaponStats(WeaponType type) {
    switch (type) {
    case WeaponType::Rifle:
        return { 0.001f, 1, 0.0f, 12.0f, 8.0f, 1.0f };
    case WeaponType::Revolver:
        return { 1.0f, 1, 0.0f, 8.0f, 10.0f, 1.5f };
    case WeaponType::Shotgun:
        return { 0.8f, 5, 45.0f, 10.0f, 6.0f, 0.8f };
    case WeaponType::RocketLauncher:
        return { 2.0f, 1, 0.0f, 8.0f, 10.0f, 0.7f };
    case WeaponType::Flamethrower:
        return { 0.1f, 3, 25.0f, 6.0f, 8.0f, 0.9f };
    default:
        return { 0.2f, 1, 0.0f, 10.0f, 0.0f, 1.0f };
    }
}

WeaponType WeaponSystem::getRandomWeaponType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, getTotalWeight());

    float randomValue = dis(gen);
    float currentWeight = 0.0f;

    for (const auto& item : rarityTable) {
        currentWeight += item.weight;
        if (randomValue <= currentWeight) {
            return item.type;
        }
    }

    return WeaponType::Rifle;
}

float WeaponSystem::getTotalWeight() {
    static float totalWeight = 0.0f;
    if (totalWeight == 0.0f) {
        for (const auto& item : rarityTable) {
            totalWeight += item.weight;
        }
    }
    return totalWeight;
}

std::string WeaponSystem::getWeaponTexture(WeaponType type) {
    switch (type) {
    case WeaponType::Rifle:
        return "rifle.png";
    case WeaponType::Revolver:
        return "revolver.png";
    case WeaponType::Shotgun:
        return "shotgun.png";
    case WeaponType::RocketLauncher:
        return "rocket_launcher.png";
    case WeaponType::Flamethrower:
        return "flamethrower.png";
    default:
        return "bullet.png";
    }
}
