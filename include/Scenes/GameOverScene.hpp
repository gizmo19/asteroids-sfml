#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include "../../include/Core/Core.hpp"

class Core;
class GameOverScene : public Scene {
public:
    GameOverScene(Core* core, sf::RenderWindow* window, int score);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    int score;
    Core* core;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text creditsText;
    sf::Text exitButtonText;
    sf::Text playAgainButtonText;

    sf::RenderWindow* gameWindow;

};