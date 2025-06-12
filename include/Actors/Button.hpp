#pragma once
#include "Actor.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class Button : public Actor {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Font& font);

    void render(sf::RenderWindow& window) override;
    void setOnClick(std::function<void()> callback);
    bool isClicked(const sf::Vector2f& mousePos);
    void setHovered(bool hovered);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Vector2f buttonSize;
    std::function<void()> onClick;
    bool isHovered;

    sf::Color normalColor;
    sf::Color hoverColor;
};
