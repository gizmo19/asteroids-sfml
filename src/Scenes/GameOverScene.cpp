#include "../../include/Scenes/GameOverScene.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Core/Core.hpp"
GameOverScene::GameOverScene(Core* core, sf::RenderWindow* window, int score) :
    core(core),
    score(score),
    gameWindow(window),
    gameOverText(sf::Text(font)),
    scoreText(sf::Text(font)),
    creditsText(sf::Text(font)),
    exitButtonText(sf::Text(font)),
    playAgainButtonText(sf::Text(font))
{
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

    //button zakoncz grê
    exitButtonText = sf::Text(font, "Zakoncz gre", 48);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setOutlineColor(sf::Color::Black);
    exitButtonText.setOutlineThickness(3);

    sf::FloatRect exitBounds = exitButtonText.getLocalBounds();
    exitButtonText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - exitBounds.size.x) / 2 - 200,
        Constants::WINDOW_HEIGHT / 2 + 200
    ));

    playAgainButtonText = sf::Text(font, "Zagraj ponownie", 48);
    playAgainButtonText.setFillColor(sf::Color::Green);
    playAgainButtonText.setOutlineColor(sf::Color::Black);
    playAgainButtonText.setOutlineThickness(4);
    sf::FloatRect againBounds = playAgainButtonText.getLocalBounds();
    playAgainButtonText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - againBounds.size.x) / 2 + 200,
        Constants::WINDOW_HEIGHT / 2 + 200
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



void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(exitButtonText);
    window.draw(creditsText);
    window.draw(playAgainButtonText);

}
void GameOverScene::update(float deltaTime) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = gameWindow->mapPixelToCoords(sf::Mouse::getPosition(*gameWindow));

        sf::FloatRect exitBounds = exitButtonText.getGlobalBounds();
        sf::FloatRect playAgainBounds = playAgainButtonText.getGlobalBounds();


        if (exitBounds.contains(mousePos)) {
            gameWindow->close();
        }

        else if (playAgainBounds.contains(mousePos)) {
            core->switchToGameplayScene();
        }
    }
}