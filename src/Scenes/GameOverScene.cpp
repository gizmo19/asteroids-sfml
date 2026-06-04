#include "../../include/Scenes/GameOverScene.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Core/Core.hpp"

GameOverScene::GameOverScene(sf::RenderWindow* window, Core* core, int score, float survivalTime)
    : score(score), survivalTime(survivalTime), gameWindow(window), core(core),
    gameOverText(sf::Text(font)), scoreText(sf::Text(font)), timerText(sf::Text(font)), creditsText(sf::Text(font)), promptText(sf::Text(font)) {
}

void GameOverScene::initialize() {
    font = sf::Font("assets/fonts/jersey.ttf");
    gameOverText = sf::Text(font, "GAME OVER", 150);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(6);

    float width = static_cast<float>(gameWindow->getSize().x);
    float height = static_cast<float>(gameWindow->getSize().y);

    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(sf::Vector2f(
        (width - gameOverBounds.size.x) / 2.0f,
        height / 2.0f - height * 0.083f
    ));

    scoreText = sf::Text(font, "Score: " + std::to_string(score), 60);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(4);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(sf::Vector2f(
        (width - scoreBounds.size.x) / 2.0f,
        height / 2.0f + height * 0.083f
    ));

    timerText = sf::Text(font, "Time played: " + std::to_string(static_cast<int>(survivalTime)) + "s", 40);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(4);
    sf::FloatRect timerBounds = timerText.getLocalBounds();
    timerText.setPosition(sf::Vector2f(
        (width - timerBounds.size.x) / 2.0f,
        height / 2.0f + height * 0.167f
    ));

    creditsText = sf::Text(font, "by Dawid Nachlik & Mateusz Hajduczek", 32);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setOutlineColor(sf::Color::Black);
    creditsText.setOutlineThickness(2);
    sf::FloatRect creditsBounds = creditsText.getLocalBounds();
    creditsText.setPosition(sf::Vector2f(
        (width - creditsBounds.size.x) / 2.0f,
        height - 100.0f
    ));

    promptText = sf::Text(font, "Press any key to return to menu", 36);
    promptText.setFillColor(sf::Color(200, 200, 200));
    promptText.setOutlineColor(sf::Color::Black);
    promptText.setOutlineThickness(2);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setPosition(sf::Vector2f(
        (width - promptBounds.size.x) / 2.0f,
        height * 0.82f
    ));

    delayClock.restart();
}

void GameOverScene::update(float deltaTime) {
    if (delayClock.getElapsedTime().asSeconds() < 0.5f) return;

    bool anyKey = false;
    for (int i = 0; i < static_cast<int>(sf::Keyboard::KeyCount); ++i) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
            anyKey = true;
            break;
        }
    }
    if (!anyKey) {
        anyKey = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ||
                 sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    }

    if (anyKey) {
        core->switchToMenu();
    }
}

void GameOverScene::render(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(timerText);
    window.draw(gameOverText);

    window.draw(creditsText);
    window.draw(promptText);
}