#include "../../include/Scenes/GameOverScene.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Actors/Button.hpp"
#include "../../include/Utils/MessageBus.hpp"

GameOverScene::GameOverScene(sf::RenderWindow* window, int score, float survivalTime)
    : score(score), survivalTime(survivalTime), gameWindow(window),
    gameOverText(sf::Text(font)), scoreText(sf::Text(font)), timerText(sf::Text(font)), creditsText(sf::Text(font)) {
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

    timerText = sf::Text(font, "Time played: " + std::to_string(static_cast<int>(survivalTime)) + "s", 40);
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(4);
    sf::FloatRect timerBounds = timerText.getLocalBounds();
    timerText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - timerBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT / 2 + 200
    ));

    creditsText = sf::Text(font, "Made by: Dawid Nachlik & Mateusz Hajduczek", 30);
    creditsText.setFillColor(sf::Color::Cyan);
    creditsText.setOutlineColor(sf::Color::Black);
    creditsText.setOutlineThickness(2);
    sf::FloatRect creditsBounds = creditsText.getLocalBounds();
    creditsText.setPosition(sf::Vector2f(
        (Constants::WINDOW_WIDTH - creditsBounds.size.x) / 2,
        Constants::WINDOW_HEIGHT - 50
    ));

    restartButton = std::make_shared<Button>(
        sf::Vector2f(Constants::WINDOW_WIDTH / 2 - 120, Constants::WINDOW_HEIGHT / 2 + 300),
        sf::Vector2f(240, 60),
        "ZAGRAJ PONOWNIE",
        font
    );

    restartButton->setOnClick([]() {
        MessageBus::publish({ MessageType::RestartGame, nullptr, std::any{} });
        });

    exitButton = std::make_shared<Button>(
        sf::Vector2f(Constants::WINDOW_WIDTH / 2 - 70, Constants::WINDOW_HEIGHT / 2 + 380),
        sf::Vector2f(140, 60),
        "WYJDZ",
        font
    );

    exitButton->setOnClick([]() {
        MessageBus::publish({ MessageType::ExitGame, nullptr, std::any{} });
        });
}

void GameOverScene::update(float deltaTime) {
    Scene::update(deltaTime);
    handleMouseInput();
}

void GameOverScene::render(sf::RenderWindow& window) {
    Scene::render(window);

    window.draw(gameOverText);
    window.draw(scoreText);
    window.draw(timerText);
    window.draw(creditsText);

    if (restartButton) {
        restartButton->render(window);
    }
    if (exitButton) {
        exitButton->render(window);
    }
}

sf::Vector2f GameOverScene::getMousePosition() {
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(*gameWindow);
    return sf::Vector2f(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));
}

void GameOverScene::handleMouseInput() {
    sf::Vector2f mousePos = getMousePosition();

    if (restartButton) {
        sf::FloatRect restartBounds(restartButton->position, sf::Vector2f(240, 60));
        restartButton->setHovered(restartBounds.contains(mousePos));
    }

    if (exitButton) {
        sf::FloatRect exitBounds(exitButton->position, sf::Vector2f(140, 60));
        exitButton->setHovered(exitBounds.contains(mousePos));
    }

    static bool mouseWasPressed = false;
    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    if (mousePressed && !mouseWasPressed) {
        if (restartButton) {
            restartButton->isClicked(mousePos);
        }
        if (exitButton) {
            exitButton->isClicked(mousePos);
        }
    }

    mouseWasPressed = mousePressed;
}
