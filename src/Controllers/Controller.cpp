#include "../../include/Controllers/Controller.hpp"

Controller::Controller() : attachedActor(nullptr), attachedScene(nullptr) {}

Controller::~Controller() {}

void Controller::attachToActor(Actor* actor) {
    attachedActor = actor;
}

void Controller::attachToScene(Scene* scene) {
    attachedScene = scene;
}
