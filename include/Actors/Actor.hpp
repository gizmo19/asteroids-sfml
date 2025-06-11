#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>

class Controller;
enum class WeaponType;

class Actor {
public:
    Actor();
    virtual ~Actor();

    virtual void update(float deltaTime);
    virtual void render(sf::RenderWindow& window);

    void addController(std::shared_ptr<Controller> controller);
    void setTexture(const sf::Texture& texture);

    void setOrigin(const sf::Vector2f& origin);
    void setScale(const sf::Vector2f& scale);
    sf::Sprite* getSprite() const;

    sf::Vector2f position;
    sf::Vector2f velocity;
    float rotation;
    float radius;
    bool active;
    WeaponType weaponType;

    const std::vector<std::shared_ptr<Controller>>& getControllers() const { return controllers; }

protected:
    std::vector<std::shared_ptr<Controller>> controllers;
    std::optional<sf::Sprite> sprite;
};
