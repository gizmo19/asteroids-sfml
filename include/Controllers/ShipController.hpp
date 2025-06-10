#pragma once
#include "Controller.hpp"
#include "../Utils/WeaponSystem.hpp"
#include "../Utils/MessageData.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class ShipController : public Controller {
public:
    ShipController();
    void update(float deltaTime) override;
    void setWindow(sf::RenderWindow* win) { window = win; }

    bool hasSpecialWeapon() const;
    float getWeaponTimeLeft() const;
    float getWeaponTotalTime() const;
    WeaponType getCurrentWeaponType() const;

private:
    sf::RenderWindow* window;
    float acceleration;
    float friction;
    float maxSpeed;
    bool canShoot;
    sf::Clock shootClock;

    WeaponType currentWeapon;
    sf::Clock weaponClock;
    float weaponDuration;

    void handleInput();
    void handleMouseRotation();
    void updateMovement(float deltaTime);
    void handleShooting();
    void constrainToBounds();
    void updateWeapon(float deltaTime);
    void fireWeapon();
    void setWeapon(WeaponType type);
};
