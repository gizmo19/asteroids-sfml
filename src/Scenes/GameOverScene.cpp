#include "../../include/Scenes/GameOverScene.hpp"


GameOverScene::GameOverScene(sf::RenderWindow* window, int score)
    : score(score), gameWindow(window), gameOverText(sf::Text(font)), scoreText(sf::Text(font)), creditsText(sf::Text(font)) {
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

    scoreText = sf::Text(font, "Wynik: " + std::to_string(score), 60);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(4);
    scoreText.setPosition({ width / 2.f - 140.f , height / 2.f + 80.f });
    
    //autorzy
    creditsText = sf::Text(font, "by Dawid Nachlik & Mateusz Hajduczek", 32);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setOutlineColor(sf::Color::Black);
    creditsText.setOutlineThickness(2);
    creditsText.setPosition({ width / 2.f - 250.f, height - 100.f });
}

void GameOverScene::update(float deltaTime) {}

void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(gameOverText);
    window.draw(creditsText);
}