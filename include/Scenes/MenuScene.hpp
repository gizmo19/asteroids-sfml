#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

class Core;

class MenuScene : public Scene {
public:
    MenuScene(Core* core);
    void initialize() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Core* core;
    sf::Font font;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> startText;
    sf::Texture backgroundTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;

    bool isStartPressed();
};
