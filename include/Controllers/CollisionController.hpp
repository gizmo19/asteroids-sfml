#pragma once
#include "Controller.hpp"
#include "../Actors/Actor.hpp"
#include "../Actors/WeaponPickup.hpp"
#include <vector>
#include <memory>

class CollisionController : public Controller {
public:
    CollisionController();
    void update(float deltaTime) override;

    void setShip(std::shared_ptr<Actor> ship);
    void setBullets(std::vector<std::shared_ptr<Actor>>* bullets);
    void setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroids);
    void setWeaponPickups(std::vector<std::shared_ptr<WeaponPickup>>* weaponPickups);

private:
    std::shared_ptr<Actor> ship;
    std::vector<std::shared_ptr<Actor>>* bullets;
    std::vector<std::shared_ptr<Actor>>* asteroids;
    std::vector<std::shared_ptr<WeaponPickup>>* weaponPickups;

    bool checkCollision(std::shared_ptr<Actor> a, std::shared_ptr<Actor> b);
    void handleBulletAsteroidCollisions();
    void handleShipAsteroidCollisions();
    void handleShipWeaponPickupCollisions();
};
