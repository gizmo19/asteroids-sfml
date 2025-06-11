#include "../../include/Utils/WeaponSystem.hpp"
#include <random>

WeaponStats WeaponSystem::getWeaponStats(WeaponType type) {
    switch (type) {
    case WeaponType::Rifle:
        return { 0.001f, 1, 0.0f, 12.0f, 8.0f, 1.0f }; // fireRate, bulletCount, spreadAngle, bulletSpeed, duration, playerSpeed
    case WeaponType::Revolver:
        return { 1.0f, 1, 0.0f, 8.0f, 10.0f, 1.5f };
    case WeaponType::Shotgun:
        return { 0.8f, 5, 45.0f, 10.0f, 6.0f, 0.8f };
    default:
        return { 0.2f, 1, 0.0f, 10.0f, 0.0f, 1.0f };
    }
}

WeaponType WeaponSystem::getRandomWeaponType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 3);

    return static_cast<WeaponType>(dis(gen));
}

std::string WeaponSystem::getWeaponTexture(WeaponType type) {
    switch (type) {
    case WeaponType::Rifle:
        return "rifle.png";
    case WeaponType::Revolver:
        return "revolver.png";
    case WeaponType::Shotgun:
        return "shotgun.png";
    default:
        return "bullet.png";
    }
}