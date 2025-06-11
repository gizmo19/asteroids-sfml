#pragma once
#include "Controller.hpp"
#include "../Actors/WeaponPickup.hpp"

class BulletController : public Controller {
public:
    BulletController(WeaponType weaponType = WeaponType::Default);
    void update(float deltaTime) override;

    WeaponType getWeaponType() const { return weaponType; }
private:
    void updateMovement(float deltaTime);
    void checkBounds();

    WeaponType weaponType;
};
