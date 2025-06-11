#include "../../include/Scenes/GameOverScene.hpp"
#include "../../include/Utils/Constants.hpp"
GameOverScene::GameOverScene(sf::RenderWindow* window, int score)
    : score(score), gameWindow(window), gameOverText(sf::Text(font)), scoreText(sf::Text(font)), creditsText(sf::Text(font)) {
}




void GameOverScene::initialize() {
 
    //Game Over
    font = sf::Font("assets/fonts/jersey.ttf");
    gameOverText = sf::Text(font, "GAME OVER", 150);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(6);


    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - gameOverBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 - 100
    ));

     //wynik
    scoreText = sf::Text(font, "Wynik: " + std::to_string(score), 60);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(4);


    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - scoreBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 + 100
    ));

    //autorzy
    creditsText = sf::Text(font, "by Dawid Nachlik & Mateusz Hajduczek", 32);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setOutlineColor(sf::Color::Black);
    creditsText.setOutlineThickness(2);

    sf::FloatRect creditsBounds = creditsText.getLocalBounds();
    creditsText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - creditsBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT - 100
    ));


  
}

void GameOverScene::update(float deltaTime) {}

void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(creditsText);
}