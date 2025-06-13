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
        return {
            Constants::WeaponStats::Rifle::FIRE_RATE,
            Constants::WeaponStats::Rifle::BULLET_COUNT,
            Constants::WeaponStats::Rifle::SPREAD,
            Constants::WeaponStats::Rifle::BULLET_SPEED,
            Constants::WeaponStats::Rifle::DURATION,
            Constants::WeaponStats::Rifle::SPEED_MULTIPLIER
        };
    case WeaponType::Revolver:
        return {
            Constants::WeaponStats::Revolver::FIRE_RATE,
            Constants::WeaponStats::Revolver::BULLET_COUNT,
            Constants::WeaponStats::Revolver::SPREAD,
            Constants::WeaponStats::Revolver::BULLET_SPEED,
            Constants::WeaponStats::Revolver::DURATION,
            Constants::WeaponStats::Revolver::SPEED_MULTIPLIER
        };
    case WeaponType::Shotgun:
        return {
            Constants::WeaponStats::Shotgun::FIRE_RATE,
            Constants::WeaponStats::Shotgun::BULLET_COUNT,
            Constants::WeaponStats::Shotgun::SPREAD,
            Constants::WeaponStats::Shotgun::BULLET_SPEED,
            Constants::WeaponStats::Shotgun::DURATION,
            Constants::WeaponStats::Shotgun::SPEED_MULTIPLIER
        };
    case WeaponType::RocketLauncher:
        return {
            Constants::WeaponStats::RocketLauncher::FIRE_RATE,
            Constants::WeaponStats::RocketLauncher::BULLET_COUNT,
            Constants::WeaponStats::RocketLauncher::SPREAD,
            Constants::WeaponStats::RocketLauncher::BULLET_SPEED,
            Constants::WeaponStats::RocketLauncher::DURATION,
            Constants::WeaponStats::RocketLauncher::SPEED_MULTIPLIER
        };
    case WeaponType::Flamethrower:
        return {
            Constants::WeaponStats::Flamethrower::FIRE_RATE,
            Constants::WeaponStats::Flamethrower::BULLET_COUNT,
            Constants::WeaponStats::Flamethrower::SPREAD,
            Constants::WeaponStats::Flamethrower::BULLET_SPEED,
            Constants::WeaponStats::Flamethrower::DURATION,
            Constants::WeaponStats::Flamethrower::SPEED_MULTIPLIER
        };
    default:
        return {
            Constants::WeaponStats::Default::FIRE_RATE,
            Constants::WeaponStats::Default::BULLET_COUNT,
            Constants::WeaponStats::Default::SPREAD,
            Constants::WeaponStats::Default::BULLET_SPEED,
            Constants::WeaponStats::Default::DURATION,
            Constants::WeaponStats::Default::SPEED_MULTIPLIER
        };
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
