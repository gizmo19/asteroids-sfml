#include "../../include/Core/Core.hpp"
#include "../../include/Scenes/Scene.hpp"
#include "../../include/Scenes/GameOverScene.hpp"

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

float Core::getGameTime() const {
    return gameStarted ? gameTimer.getElapsedTime().asSeconds() : 0.0f;
}

void Core::startGameTimer() {
    if (!gameStarted) {
        gameTimer.restart();
        gameStarted = true;
        printf("\n=== GAME STARTED ===\n");
        printf("Timer started at: %.1f\n", gameTimer.getElapsedTime().asSeconds());
    }
}

void Core::stopGameTimer() {
    if (gameStarted) {
        float finalTime = gameTimer.getElapsedTime().asSeconds();
        gameStarted = false;
        printf("\n=== GAME OVER ===\n");
        printf("Final time: %.1f seconds (%.1f minutes)\n", finalTime, finalTime / 60.0f);
        printf("================\n");
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
void Core::switchToGameOverScene(int score, float survivalTime) {
    auto gameOverScene = std::make_shared<GameOverScene>(&window, score, survivalTime);
    activeScene = gameOverScene;
    activeScene->setWindow(&window);
    activeScene->initialize();
}

void Core::switchToMenu() {
    stopGameTimer();
    setActiveScene(0);
}
