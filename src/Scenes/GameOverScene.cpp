#include "../../include/Scenes/GameOverScene.hpp"


GameOverScene::GameOverScene(sf::RenderWindow* window)
    : gameWindow(window), gameOverText(sf::Text(font)) {
}

void GameOverScene::initialize() {
    font = sf::Font("assets/fonts/jersey.ttf");
    gameOverText = sf::Text(font, "GAME OVER", 150);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(6);

    float width = gameWindow->getSize().x;
    float height = gameWindow->getSize().y;
    gameOverText.setPosition({ width / 2.f - 330.f, height / 2.f - 200.f });
}

void GameOverScene::update(float deltaTime) {}

void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(gameOverText);
}