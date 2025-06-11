#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class GameOverScene : public Scene {
public:
    GameOverScene(sf::RenderWindow* window, int score);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    int score;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text creditsText;
    sf::RenderWindow* gameWindow;
};