#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class Core;

class GameOverScene : public Scene {
public:
    GameOverScene(sf::RenderWindow* window, Core* core, int score, float survivalTime);
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
    Core* core;
    sf::Clock delayClock;
    sf::Text promptText;
};