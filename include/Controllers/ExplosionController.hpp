#pragma once
#include "Controller.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Actor;

enum class ExplosionStage {
    FLASH,
    EXPAND,
    FADE
};

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float life;
    float maxLife;
    float size;
};

struct ExplosionEffect {
    sf::Vector2f position;
    float maxRadius;
    float currentRadius;
    ExplosionStage stage;
    float stageTimer;
    sf::Color currentColor;
    std::vector<Particle> particles;
    float totalDuration;
    bool damageDealt;
};

class ExplosionController : public Controller {
public:
    ExplosionController();

    void update(float deltaTime) override;
    void setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroidList);
    void render(sf::RenderWindow& window);

private:
    std::vector<std::shared_ptr<Actor>>* asteroids;
    std::vector<ExplosionEffect> explosionEffects;

    void handleExplosion(sf::Vector2f position, float radius);
    void createParticleExplosion(ExplosionEffect& effect);
    void updateExplosionStage(ExplosionEffect& effect, float deltaTime);
    void updateParticles(ExplosionEffect& effect, float deltaTime);
    void dealDamageToAsteroids(const ExplosionEffect& effect);
};
