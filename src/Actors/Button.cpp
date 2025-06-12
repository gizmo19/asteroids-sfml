#include "../../include/Actors/Button.hpp"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, sf::Font& font)
    : buttonSize(size), isHovered(false),
    normalColor(sf::Color(70, 70, 70, 200)),
    hoverColor(sf::Color(100, 100, 100, 220)),
    buttonText(font) {  // Inicjalizuj sf::Text z fontem

    this->position = position;

    buttonShape.setSize(buttonSize);
    buttonShape.setPosition(position);
    buttonShape.setFillColor(normalColor);
    buttonShape.setOutlineColor(sf::Color::White);
    buttonShape.setOutlineThickness(3);

    buttonText.setString(text);
    buttonText.setCharacterSize(36);
    buttonText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(sf::Vector2f(
        position.x + (buttonSize.x - textBounds.size.x) / 2,
        position.y + (buttonSize.y - textBounds.size.y) / 2
    ));
}

void Button::render(sf::RenderWindow& window) {
    if (isHovered) {
        buttonShape.setFillColor(hoverColor);
        buttonShape.setOutlineThickness(4);
    }
    else {
        buttonShape.setFillColor(normalColor);
        buttonShape.setOutlineThickness(3);
    }

    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

bool Button::isClicked(const sf::Vector2f& mousePos) {
    sf::FloatRect bounds = buttonShape.getGlobalBounds();
    if (bounds.contains(mousePos)) {
        if (onClick) {
            onClick();
        }
        return true;
    }
    return false;
}

void Button::setHovered(bool hovered) {
    isHovered = hovered;
}
