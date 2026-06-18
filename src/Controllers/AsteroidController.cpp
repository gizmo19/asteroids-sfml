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
    float w = window ? static_cast<float>(window->getSize().x) : 1600.0f;
    float h = window ? static_cast<float>(window->getSize().y) : 1200.0f;
    if (attachedActor->position.y - attachedActor->radius > h) {
        attachedActor->position.y = -attachedActor->radius;
        attachedActor->position.x = static_cast<float>(std::rand() % (static_cast<int>(w) - 40) + 20);
    }
}
