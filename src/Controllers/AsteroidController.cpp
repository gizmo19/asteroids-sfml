#include "../../include/Controllers/AsteroidController.hpp"
#include "../../include/Actors/Actor.hpp"
#include <cstdlib>

AsteroidController::AsteroidController() : angularVelocity(0.0f) {
    angularVelocity = (static_cast<float>(std::rand() % 200) - 100.0f) / 20.0f;
}

void AsteroidController::update(float deltaTime) {
    if (!attachedActor) return;

    updateMovement(deltaTime);
    wrapPosition();
}

void AsteroidController::updateMovement(float deltaTime) {
    attachedActor->position += attachedActor->velocity;
    attachedActor->rotation += angularVelocity;
}

void AsteroidController::wrapPosition() {
    if (attachedActor->position.y - attachedActor->radius > 1200) {
        attachedActor->position.y = -attachedActor->radius;
        attachedActor->position.x = static_cast<float>(std::rand() % (1600 - 40) + 20);
    }
}
