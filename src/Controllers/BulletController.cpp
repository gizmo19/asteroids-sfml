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
    if (attachedActor->position.x < -50.0f || attachedActor->position.x > Constants::WINDOW_WIDTH + 50.0f ||
        attachedActor->position.y < -50.0f || attachedActor->position.y > Constants::WINDOW_HEIGHT + 50.0f) {
        attachedActor->active = false;
    }
}
