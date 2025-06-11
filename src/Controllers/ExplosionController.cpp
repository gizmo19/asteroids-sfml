#include "../../include/Controllers/ExplosionController.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include "../../include/Utils/MessageData.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Actors/Actor.hpp"
#include <cmath>

ExplosionController::ExplosionController() : asteroids(nullptr) {
    MessageBus::subscribe(MessageType::ExplosionTriggered, [this](const Message& msg) {
        ExplosionData data = std::any_cast<ExplosionData>(msg.payload);
        handleExplosion(data.position, data.radius);
        });
}

void ExplosionController::setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroidList) {
    asteroids = asteroidList;
}

void ExplosionController::update(float deltaTime) {}

void ExplosionController::handleExplosion(sf::Vector2f position, float radius) {
    if (!asteroids) return;

    for (auto& asteroid : *asteroids) {
        if (!asteroid->active) continue;

        float distance = std::sqrt(
            std::pow(asteroid->position.x - position.x, 2) +
            std::pow(asteroid->position.y - position.y, 2)
        );

        if (distance <= radius + asteroid->radius) {
            asteroid->active = false;

            Message msg;
            msg.type = MessageType::AsteroidDestroyed;
            msg.sender = this;
            msg.payload = 10;
            MessageBus::publish(msg);
        }
    }
}
