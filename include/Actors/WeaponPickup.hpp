#pragma once
#include "Actor.hpp"

enum class WeaponType {
    Default,
    Minigun,
    Bazooka,
    Shotgun
};

class WeaponPickup : public Actor {
public:
    WeaponPickup(WeaponType type);
    WeaponType getWeaponType() const { return weaponType; }

private:
    WeaponType weaponType;
};
