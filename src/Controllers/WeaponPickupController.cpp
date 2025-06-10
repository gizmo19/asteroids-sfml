#include "../../include/Controllers/WeaponPickupController.hpp"
#include "../../include/Actors/Actor.hpp"
#include <cmath>

WeaponPickupController::WeaponPickupController() : isOriginalYSet(false), originalY(0.0f) {}

void WeaponPickupController::update(float deltaTime) {
    if (!attachedActor) return;

    updateBobbing();
    checkLifetime();
}

void WeaponPickupController::updateBobbing() {
    if (!isOriginalYSet) {
        originalY = attachedActor->position.y;
        isOriginalYSet = true;
    }

    float bobOffset = std::sin(bobClock.getElapsedTime().asSeconds() * BOB_SPEED) * BOB_AMPLITUDE;
    attachedActor->position.y = originalY + bobOffset;
}

void WeaponPickupController::checkLifetime() {
    if (lifetimeClock.getElapsedTime().asSeconds() > MAX_LIFETIME) {
        attachedActor->active = false;
    }
}
