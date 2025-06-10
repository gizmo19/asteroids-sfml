#include "../../include/Controllers/CollisionController.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include <cmath>
#include <algorithm>

CollisionController::CollisionController() : bullets(nullptr), asteroids(nullptr) {}

void CollisionController::update(float deltaTime) {
    handleBulletAsteroidCollisions();
    handleShipAsteroidCollisions();
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

bool CollisionController::checkCollision(std::shared_ptr<Actor> a, std::shared_ptr<Actor> b) {
    if (!a->active || !b->active) return false;

    float dx = a->position.x - b->position.x;
    float dy = a->position.y - b->position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (a->radius + b->radius);
}

void CollisionController::handleBulletAsteroidCollisions() {
    if (!bullets || !asteroids) return;

    for (auto& bullet : *bullets) {
        if (!bullet->active) continue;

        for (auto& asteroid : *asteroids) {
            if (!asteroid->active) continue;

            if (checkCollision(bullet, asteroid)) {
                bullet->active = false;
                asteroid->active = false;

                Message msg;
                msg.type = MessageType::AsteroidDestroyed;
                msg.sender = this;
                msg.payload = 10;
                MessageBus::publish(msg);
                break;
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
