#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Scene;

class Core {
public:
    Core();
    ~Core();

    void run();
    void addScene(std::shared_ptr<Scene> scene);
    void setActiveScene(size_t index);
    sf::RenderWindow& getWindow();
    float getDeltaTime() const;

private:
    sf::RenderWindow window;
    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> activeScene;
    sf::Clock clock;
    float deltaTime;

    void handleEvents();
    void update();
    void render();
};
