#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class GameOverScene : public Scene {
public:
    GameOverScene(sf::RenderWindow* window);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text gameOverText;
    sf::RenderWindow* gameWindow;
};
