#include "../../include/Controllers/CollisionController.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Utils/MessageData.hpp"
#include <cmath>
#include <algorithm>

CollisionController::CollisionController() : bullets(nullptr), asteroids(nullptr), weaponPickups(nullptr) {}

void CollisionController::update(float deltaTime) {
    handleBulletAsteroidCollisions();
    handleShipAsteroidCollisions();
    handleShipWeaponPickupCollisions();
}

void CollisionController::setShip(std::shared_ptr<Actor> ship) {
    this->ship = ship;
}

void CollisionController::setBullets(std::vector<std::shared_ptr<Actor>>* bullets) {
    this->bullets = bullets;
}

void CollisionController::setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroids) {
    this->asteroids = asteroids;
}

void CollisionController::setWeaponPickups(std::vector<std::shared_ptr<WeaponPickup>>* weaponPickups) {
    this->weaponPickups = weaponPickups;
}

bool CollisionController::checkCollision(std::shared_ptr<Actor> a, std::shared_ptr<Actor> b) {
    if (!a->active || !b->active) return false;

    float dx = a->position.x - b->position.x;
    float dy = a->position.y - b->position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (a->radius + b->radius);
}

void CollisionController::handleBulletAsteroidCollisions() {
    if (!bullets || !asteroids) return;

    auto bulletsCopy = *bullets;
    auto asteroidsCopy = *asteroids;

    for (auto& bullet : bulletsCopy) {
        if (!bullet || !bullet->active) continue;

        for (auto& asteroid : asteroidsCopy) {
            if (!asteroid || !asteroid->active) continue;

            if (checkCollision(bullet, asteroid)) {
                WeaponType bulletWeaponType = bullet->weaponType;

                bullet->active = false;

                if (bulletWeaponType == WeaponType::RocketLauncher) {
                    Message explosionMsg;
                    explosionMsg.type = MessageType::ExplosionTriggered;
                    explosionMsg.sender = this;

                    ExplosionData explosionData;
                    explosionData.position = asteroid->position;
                    explosionData.radius = 100.0f;
                    explosionData.weaponType = WeaponType::RocketLauncher;

                    explosionMsg.payload = explosionData;
                    MessageBus::publish(explosionMsg);
                }

                asteroid->active = false;

                Message destroyMsg;
                destroyMsg.type = MessageType::AsteroidDestroyed;
                destroyMsg.sender = this;
                destroyMsg.payload = 100;
                MessageBus::publish(destroyMsg);
            }
        }
    }
}

void CollisionController::handleShipAsteroidCollisions() {
    if (!ship || !asteroids) return;

    for (auto& asteroid : *asteroids) {
        if (!asteroid->active) continue;

        if (checkCollision(ship, asteroid)) {
            Message msg;
            msg.type = MessageType::GameOver;
            msg.sender = this;
            MessageBus::publish(msg);
            break;
        }
    }
}

void CollisionController::handleShipWeaponPickupCollisions() {
    if (!ship || !weaponPickups) return;

    for (auto& weaponPickup : *weaponPickups) {
        if (!weaponPickup->active) continue;

        if (checkCollision(ship, weaponPickup)) {
            weaponPickup->active = false;

            Message msg;
            msg.type = MessageType::WeaponPickedUp;
            msg.sender = this;
            msg.payload = weaponPickup->getWeaponType();
            MessageBus::publish(msg);
            break;
        }
    }
}
