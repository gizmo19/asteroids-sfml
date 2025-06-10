#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Actor;
class Controller;

class Scene {
public:
    Scene();
    virtual ~Scene();

    virtual void initialize() = 0;
    virtual void update(float deltaTime);
    virtual void render(sf::RenderWindow& window);
    void cleanupInactiveActors();

    void addActor(std::shared_ptr<Actor> actor);
    void addController(std::shared_ptr<Controller> controller);
    
    void setWindow(sf::RenderWindow* window) { this->window = window; }

protected:
    std::vector<std::shared_ptr<Actor>> actors;
    std::vector<std::shared_ptr<Controller>> controllers;
    sf::RenderWindow* window = nullptr;
};
