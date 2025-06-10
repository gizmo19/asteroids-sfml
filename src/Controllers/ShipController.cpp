#include "../../include/Controllers/ShipController.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include <cmath>

ShipController::ShipController() : acceleration(0.4f), friction(0.92f),
maxSpeed(12.0f), canShoot(true), window(nullptr), currentWeapon(WeaponType::Default), weaponDuration(0.0f) {

    MessageBus::subscribe(MessageType::WeaponPickedUp, [this](const Message& msg) {
        WeaponType weaponType = std::any_cast<WeaponType>(msg.payload);
        setWeapon(weaponType);
        });
}

void ShipController::update(float deltaTime) {
    if (!attachedActor) return;

    handleInput();
    handleMouseRotation();
    updateMovement(deltaTime);
    constrainToBounds();
    updateWeapon(deltaTime);
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
            fireWeapon();
        }
    }
    else {
        canShoot = true;
    }
}

void ShipController::updateWeapon(float deltaTime) {
    if (currentWeapon != WeaponType::Default && weaponDuration > 0.0f) {
        weaponDuration -= deltaTime;
        if (weaponDuration <= 0.0f) {
            currentWeapon = WeaponType::Default;
            Message msg;
            msg.type = MessageType::WeaponExpired;
            msg.sender = this;
            MessageBus::publish(msg);
        }
    }
}

void ShipController::fireWeapon() {
    WeaponStats stats = WeaponSystem::getWeaponStats(currentWeapon);

    if (shootClock.getElapsedTime().asSeconds() < stats.fireRate) {
        return;
    }

    shootClock.restart();
    canShoot = false;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    float dx = static_cast<float>(mousePos.x) - attachedActor->position.x;
    float dy = static_cast<float>(mousePos.y) - attachedActor->position.y;
    float baseAngle = std::atan2(dy, dx) * 180.0f / 3.14159f + 90.0f;

    for (int i = 0; i < stats.bulletCount; ++i) {
        float angle = baseAngle;
        if (stats.bulletCount > 1) {
            float spreadStep = stats.spread / (stats.bulletCount - 1);
            angle += -stats.spread / 2.0f + i * spreadStep;
        }

        Message msg;
        msg.type = MessageType::BulletFired;
        msg.sender = this;

        BulletData data;
        data.position = attachedActor->position;
        data.angle = angle;
        data.speed = stats.bulletSpeed;
        data.weaponType = currentWeapon;

        msg.payload = data;
        MessageBus::publish(msg);
    }
}

void ShipController::setWeapon(WeaponType type) {
    currentWeapon = type;
    WeaponStats stats = WeaponSystem::getWeaponStats(type);
    weaponDuration = stats.duration;
    weaponClock.restart();
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

bool ShipController::hasSpecialWeapon() const {
    return currentWeapon != WeaponType::Default && weaponDuration > 0.0f;
}

float ShipController::getWeaponTimeLeft() const {
    return weaponDuration;
}

float ShipController::getWeaponTotalTime() const {
    return WeaponSystem::getWeaponStats(currentWeapon).duration;
}

WeaponType ShipController::getCurrentWeaponType() const {
    return currentWeapon;
}
