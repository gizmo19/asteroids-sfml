#include "../../include/Core/Core.hpp"
#include "../../include/Scenes/Scene.hpp"

Core::Core() : window(sf::VideoMode({ 1600, 1200 }), "Asteroids SFML"), deltaTime(0.0f) {
    window.setFramerateLimit(60);
}

Core::~Core() {}

void Core::run() {
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        handleEvents();
        update();
        render();
    }
}

void Core::addScene(std::shared_ptr<Scene> scene) {
    scenes.push_back(scene);
    scene->setWindow(&window);
    if (!activeScene) {
        activeScene = scene;
        scene->initialize();
    }
}

void Core::setActiveScene(size_t index) {
    if (index < scenes.size()) {
        activeScene = scenes[index];
        activeScene->setWindow(&window);
        activeScene->initialize();
    }
}

sf::RenderWindow& Core::getWindow() {
    return window;
}

float Core::getDeltaTime() const {
    return deltaTime;
}

void Core::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Core::update() {
    if (activeScene) {
        activeScene->update(deltaTime);
    }
}

void Core::render() {
    window.clear();

    if (activeScene) {
        activeScene->render(window);
    }

    window.display();
}
