#pragma once
#include "Controller.hpp"
#include "../Actors/WeaponPickup.hpp"
#include <SFML/Graphics.hpp>

class BulletController : public Controller {
public:
    BulletController(WeaponType weaponType = WeaponType::Default);
    void update(float deltaTime) override;
    void setWindow(sf::RenderWindow* win) { window = win; }

    WeaponType getWeaponType() const { return weaponType; }
private:
    void updateMovement(float deltaTime);
    void checkBounds();

    WeaponType weaponType;
    sf::RenderWindow* window = nullptr;
};
