#include "../../include/Controllers/BulletController.hpp"
#include "../../include/Actors/Actor.hpp"

BulletController::BulletController() {}

void BulletController::update(float deltaTime) {
    if (!attachedActor) return;

    updateMovement(deltaTime);
    checkBounds();
}

void BulletController::updateMovement(float deltaTime) {
    attachedActor->position += attachedActor->velocity;
}

void BulletController::checkBounds() {
    if (attachedActor->position.x < 0 || attachedActor->position.x > 1600 ||
        attachedActor->position.y < 0 || attachedActor->position.y > 1200) {
        attachedActor->active = false;
    }
}
