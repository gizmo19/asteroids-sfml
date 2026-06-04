#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Utils/Constants.hpp"

BulletController::BulletController(WeaponType weaponType) : weaponType(weaponType) {
}

void BulletController::update(float deltaTime) {
    if (!attachedActor) return;

    updateMovement(deltaTime);
    checkBounds();
}

void BulletController::updateMovement(float deltaTime) {
    attachedActor->position += attachedActor->velocity;
}

void BulletController::checkBounds() {
    float w = window ? static_cast<float>(window->getSize().x) : static_cast<float>(Constants::WINDOW_WIDTH);
    float h = window ? static_cast<float>(window->getSize().y) : static_cast<float>(Constants::WINDOW_HEIGHT);
    if (attachedActor->position.x < -Constants::BOUNDARY_OFFSET ||
        attachedActor->position.x > w + Constants::BOUNDARY_OFFSET ||
        attachedActor->position.y < -Constants::BOUNDARY_OFFSET ||
        attachedActor->position.y > h + Constants::BOUNDARY_OFFSET) {
        attachedActor->active = false;
    }
}
