#include "../../include/Scenes/Scene.hpp"
#include "../../include/Actors/Actor.hpp"
#include "../../include/Controllers/Controller.hpp"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::update(float deltaTime) {
    for (auto& controller : controllers) {
        controller->update(deltaTime);
    }

    auto actorsCopy = actors;
    for (auto& actor : actorsCopy) {
        if (actor && actor->active) {
            actor->update(deltaTime);
        }
    }
}

void Scene::render(sf::RenderWindow& window) {
    auto actorsCopy = actors;
    for (auto& actor : actorsCopy) {
        if (actor && actor->active) {
            actor->render(window);
        }
    }
}

void Scene::cleanupInactiveActors() {
    actors.erase(std::remove_if(actors.begin(), actors.end(),
        [](const std::shared_ptr<Actor>& actor) {
            return !actor || !actor->active;
        }), actors.end());
}

void Scene::addActor(std::shared_ptr<Actor> actor) {
    actors.push_back(actor);
}

void Scene::addController(std::shared_ptr<Controller> controller) {
    controllers.push_back(controller);
    controller->attachToScene(this);
}
