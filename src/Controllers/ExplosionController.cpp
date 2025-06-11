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

void ExplosionController::update(float deltaTime) {
    for (int i = explosionTimers.size() - 1; i >= 0; i--) {
        explosionTimers[i] -= deltaTime;
        if (explosionTimers[i] <= 0) {
            explosionEffects.erase(explosionEffects.begin() + i);
            explosionTimers.erase(explosionTimers.begin() + i);
        }
    }
}

void ExplosionController::render(sf::RenderWindow& window) {
    for (auto& explosion : explosionEffects) {
        window.draw(explosion);
    }
}

void ExplosionController::handleExplosion(sf::Vector2f position, float radius) {
    if (!asteroids) return;

    printf("EXPLOSION TRIGGERED! Position: (%.2f, %.2f), Radius: %.2f\n",
        position.x, position.y, radius);

    sf::CircleShape explosion(radius);
    explosion.setPosition(sf::Vector2f(position.x - radius, position.y - radius));
    explosion.setFillColor(sf::Color(255, 255, 0, 80));
    explosion.setOutlineThickness(3.0f);
    explosion.setOutlineColor(sf::Color::Red);

    explosionEffects.push_back(explosion);
    explosionTimers.push_back(1.0f);

    int destroyedCount = 0;
    for (auto& asteroid : *asteroids) {
        if (!asteroid->active) continue;

        float distance = std::sqrt(
            std::pow(asteroid->position.x - position.x, 2) +
            std::pow(asteroid->position.y - position.y, 2)
        );

        if (distance <= radius + asteroid->radius) {
            asteroid->active = false;
            destroyedCount++;

            Message msg;
            msg.type = MessageType::AsteroidDestroyed;
            msg.sender = this;
            msg.payload = 10;
            MessageBus::publish(msg);
        }
    }

    printf("Explosion destroyed %d asteroids\n", destroyedCount);
}
