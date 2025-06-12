#include "../../include/Scenes/GameOverScene.hpp"
#include "../../include/Utils/Constants.hpp"

GameOverScene::GameOverScene(sf::RenderWindow* window, int score, float survivalTime)
    : score(score), survivalTime(survivalTime), gameWindow(window),
    gameOverText(sf::Text(font)), scoreText(sf::Text(font)), timerText(sf::Text(font)), creditsText(sf::Text(font)), exitText(sf::Text(font)), playAgainText(sf::Text(font)){
}

void GameOverScene::initialize() {
    font = sf::Font("assets/fonts/jersey.ttf");
    gameOverText = sf::Text(font, "GAME OVER", 150);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(6);

    float width = gameWindow->getSize().x;
    float height = gameWindow->getSize().y;

    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - gameOverBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 - 100
    ));

    scoreText = sf::Text(font, "Score: " + std::to_string(score), 60);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(4);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - scoreBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 + 100
    ));

    //timer

    timerText = sf::Text(font, "Time played: " + std::to_string(static_cast<int>(survivalTime)) + "s", 40);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(4);
    timerText.setPosition({ width / 2.f - 140.f , height / 2.f + 140.f });
    sf::FloatRect timerBounds = timerText.getLocalBounds();
    timerText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - timerBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 + 200
    ));

    //exit
    
    exitText = sf::Text(font, "Zakoncz gre", 48);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(3);

    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - exitBounds.size.x) / 2 - 200,
        Constants::WINDOW_HEIGHT / 2 + 300
    ));

    //play again

    playAgainText = sf::Text(font, "Zagraj ponownie", 48);
    playAgainText.setFillColor(sf::Color::Green);
    playAgainText.setOutlineColor(sf::Color::Black);
    playAgainText.setOutlineThickness(4);
    sf::FloatRect againBounds = playAgainText.getLocalBounds();
    playAgainText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - againBounds.size.x) / 2 + 200,
        Constants::WINDOW_HEIGHT / 2 + 300
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

void GameOverScene::update(float deltaTime) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      
        sf::Vector2f mousePos = gameWindow->mapPixelToCoords(sf::Mouse::getPosition(*gameWindow));
        sf::FloatRect exitBounds = exitText.getGlobalBounds();
        //sf::FloatRect playAgainBounds = playAgainButtonText.getGlobalBounds();


        if (exitBounds.contains(mousePos)) {
            gameWindow->close();
        }

       /* else if (playAgainBounds.contains(mousePos)) {
            core->switchToGameplayScene();
        }*/
    }
}

void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(timerText);
    window.draw(gameOverText);
    window.draw(exitText);
    window.draw(playAgainText);
    window.draw(creditsText);

}