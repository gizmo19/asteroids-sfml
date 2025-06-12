#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Button;

class GameOverScene : public Scene {
public:
    GameOverScene(sf::RenderWindow* window, int score, float survivalTime);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    int score;
    float survivalTime;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text creditsText;
    sf::Text timerText;
    sf::RenderWindow* gameWindow;

    std::shared_ptr<Button> restartButton;
    std::shared_ptr<Button> exitButton;

    sf::Vector2f getMousePosition();
    void handleMouseInput();
};
