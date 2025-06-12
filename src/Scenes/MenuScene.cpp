#include "../../include/Scenes/MenuScene.hpp"
#include "../../include/Core/Core.hpp"
#include "../../include/Scenes/GameplayScene.hpp"
#include <SFML/Window/Keyboard.hpp>

MenuScene::MenuScene(Core* core) : core(core) {}

void MenuScene::initialize() {
    if (!font.openFromFile("assets/fonts/jersey.ttf")) {
        return;
    }

    backgroundTexture.loadFromFile("assets/images/background.png");
    backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
    backgroundSprite->setScale(sf::Vector2f{
        1600.0f / static_cast<float>(backgroundTexture.getSize().x),
        1200.0f / static_cast<float>(backgroundTexture.getSize().y)
        });

    titleText = std::make_unique<sf::Text>(font);
    titleText->setString("ASTEROIDS");
    titleText->setCharacterSize(120);
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setOutlineColor(sf::Color::Black);
    titleText->setOutlineThickness(3);
    sf::FloatRect titleBounds = titleText->getLocalBounds();
    titleText->setPosition(sf::Vector2f{ 800.0f - titleBounds.size.x / 2.0f, 300.0f });

    startText = std::make_unique<sf::Text>(font);
    startText->setString("Press SPACE to Start");
    startText->setCharacterSize(48);
    startText->setFillColor(sf::Color::White);
    startText->setOutlineColor(sf::Color::Black);
    startText->setOutlineThickness(2);
    sf::FloatRect startBounds = startText->getLocalBounds();
    startText->setPosition(sf::Vector2f{ 800.0f - startBounds.size.x / 2.0f, 600.0f });
}

void MenuScene::update(float deltaTime) {
    Scene::update(deltaTime);

    if (isStartPressed()) {
        auto gameplayScene = std::make_shared<GameplayScene>(core, &core->getWindow());
        core->addScene(gameplayScene);
        core->setActiveScene(1);
        core->startGameTimer();
    }
}

void MenuScene::render(sf::RenderWindow& window) {
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    }

    if (titleText) {
        window.draw(*titleText);
    }

    if (startText) {
        window.draw(*startText);
    }

    Scene::render(window);
}

bool MenuScene::isStartPressed() {
    static bool wasPressed = false;
    bool isPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    if (isPressed && !wasPressed) {
        wasPressed = true;
        return true;
    }

    if (!isPressed) {
        wasPressed = false;
    }

    return false;
}
