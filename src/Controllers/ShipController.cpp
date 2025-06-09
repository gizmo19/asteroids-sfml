#include "../../include/Controllers/ShipController.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include <cmath>

ShipController::ShipController() : acceleration(0.4f), friction(0.92f),
maxSpeed(12.0f), canShoot(true), window(nullptr) {
}

void ShipController::update(float deltaTime) {
    if (!attachedActor) return;

    handleInput();
    handleMouseRotation();
    updateMovement(deltaTime);
    constrainToBounds();
    handleShooting();
}

void ShipController::handleInput() {
    sf::Vector2f accel(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
        accel.x -= acceleration;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
        accel.x += acceleration;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
        accel.y -= acceleration;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
        accel.y += acceleration;

    attachedActor->velocity += accel;
}

void ShipController::handleMouseRotation() {
    if (!attachedActor || !window) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Vector2f shipPos = attachedActor->position;
    sf::Vector2f direction = sf::Vector2f(mousePos.x, mousePos.y) - shipPos;

    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f + 90.0f;
    attachedActor->rotation = angle;
}



void ShipController::updateMovement(float deltaTime) {
    attachedActor->velocity *= friction;

    float speed = std::sqrt(attachedActor->velocity.x * attachedActor->velocity.x +
        attachedActor->velocity.y * attachedActor->velocity.y);
    if (speed > maxSpeed) {
        attachedActor->velocity.x = (attachedActor->velocity.x / speed) * maxSpeed;
        attachedActor->velocity.y = (attachedActor->velocity.y / speed) * maxSpeed;
    }

    attachedActor->position += attachedActor->velocity;
}

void ShipController::handleShooting() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (canShoot) {
            Message msg;
            msg.type = MessageType::BulletFired;
            msg.sender = this;
            msg.payload = attachedActor->position;
            MessageBus::publish(msg);
            canShoot = false;
        }
    }
    else {
        canShoot = true;
    }
}

void ShipController::constrainToBounds() {
    const float BOUNDS_MARGIN = 20.0f;
    const float BOUNCE_FACTOR = 0.3f;

    if (attachedActor->position.x < BOUNDS_MARGIN) {
        attachedActor->position.x = BOUNDS_MARGIN;
        attachedActor->velocity.x = std::abs(attachedActor->velocity.x) * BOUNCE_FACTOR;
    }
    if (attachedActor->position.x > 1600 - BOUNDS_MARGIN) {
        attachedActor->position.x = 1600 - BOUNDS_MARGIN;
        attachedActor->velocity.x = -std::abs(attachedActor->velocity.x) * BOUNCE_FACTOR;
    }
    if (attachedActor->position.y < BOUNDS_MARGIN) {
        attachedActor->position.y = BOUNDS_MARGIN;
        attachedActor->velocity.y = std::abs(attachedActor->velocity.y) * BOUNCE_FACTOR;
    }
    if (attachedActor->position.y > 1200 - BOUNDS_MARGIN) {
        attachedActor->position.y = 1200 - BOUNDS_MARGIN;
        attachedActor->velocity.y = -std::abs(attachedActor->velocity.y) * BOUNCE_FACTOR;
    }
}
