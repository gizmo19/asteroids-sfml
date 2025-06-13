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
    if (attachedActor->position.x < -Constants::BOUNDARY_OFFSET ||
        attachedActor->position.x > Constants::WINDOW_WIDTH + Constants::BOUNDARY_OFFSET ||
        attachedActor->position.y < -Constants::BOUNDARY_OFFSET ||
        attachedActor->position.y > Constants::WINDOW_HEIGHT + Constants::BOUNDARY_OFFSET) {
        attachedActor->active = false;
    }
}
