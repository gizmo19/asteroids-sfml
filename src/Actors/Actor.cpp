#include "../../include/Actors/Actor.hpp"
#include "../../include/Controllers/Controller.hpp"
#include "../../include/Actors/WeaponPickup.hpp"

Actor::Actor() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), rotation(0.0f),
radius(0.0f), active(true), weaponType(WeaponType::Default) {
}

Actor::~Actor() {}

void Actor::update(float deltaTime) {
    for (auto& controller : controllers) {
        if (controller) {
            controller->update(deltaTime);
        }
    }
}

void Actor::render(sf::RenderWindow& window) {
    if (sprite.has_value() && active) {
        sprite->setPosition(position);
        sprite->setRotation(sf::degrees(rotation));
        window.draw(*sprite);
    }
}

void Actor::addController(std::shared_ptr<Controller> controller) {
    if (controller) {
        controllers.push_back(controller);
        controller->attachToActor(this);
    }
}

void Actor::setTexture(const sf::Texture& texture) {
    sprite = sf::Sprite(texture);
}

void Actor::setOrigin(const sf::Vector2f& origin) {
    if (sprite.has_value()) {
        sprite->setOrigin(origin);
    }
}

void Actor::setScale(const sf::Vector2f& scale) {
    if (sprite.has_value()) {
        sprite->setScale(scale);
    }
}

sf::Sprite* Actor::getSprite() const {
    if (sprite.has_value()) {
        return const_cast<sf::Sprite*>(&sprite.value());
    }
    return nullptr;
}
