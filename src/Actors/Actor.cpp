#include "../../include/Actors/Actor.hpp"
#include "../../include/Controllers/Controller.hpp"

Actor::Actor() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), rotation(0.0f),
radius(0.0f), active(true) {
}

Actor::~Actor() {}

void Actor::update(float deltaTime) {
    for (auto& controller : controllers) {
        controller->update(deltaTime);
    }
}

void Actor::render(sf::RenderWindow& window) {
    if (sprite) {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
        window.draw(*sprite);
    }
}

void Actor::addController(std::shared_ptr<Controller> controller) {
    controllers.push_back(controller);
    controller->attachToActor(this);
}

void Actor::setTexture(const sf::Texture& texture) {
    sprite = std::make_unique<sf::Sprite>(texture);
}

void Actor::setOrigin(const sf::Vector2f& origin) {
    if (sprite) {
        sprite->setOrigin(origin);
    }
}

void Actor::setScale(const sf::Vector2f& scale) {
    if (sprite) {
        sprite->setScale(scale);
    }
}

sf::Sprite* Actor::getSprite() const {
    return sprite.get();
}
