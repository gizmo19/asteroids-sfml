#include "../../include/Controllers/ExplosionController.hpp"
#include "../../include/Utils/MessageBus.hpp"
#include "../../include/Utils/MessageData.hpp"
#include "../../include/Utils/Constants.hpp"
#include "../../include/Actors/Actor.hpp"
#include <cmath>
#include <random>

ExplosionController::ExplosionController() : asteroids(nullptr) {
    MessageBus::subscribe(MessageType::ExplosionTriggered, [this](const Message& msg) {
        ExplosionData data = std::any_cast<ExplosionData>(msg.payload);
        handleExplosion(data.position, data.radius);
        });
}

void ExplosionController::setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroidList) {
    asteroids = asteroidList;
}

void ExplosionController::update(float deltaTime) {
    for (int i = explosionEffects.size() - 1; i >= 0; i--) {
        auto& effect = explosionEffects[i];

        updateExplosionStage(effect, deltaTime);
        updateParticles(effect, deltaTime);

        if (!effect.damageDealt && effect.stage == ExplosionStage::EXPAND &&
            effect.stageTimer > Constants::EXPLOSION_STAGE_THRESHOLD) {
            dealDamageToAsteroids(effect);
            effect.damageDealt = true;
        }

        effect.totalDuration -= deltaTime;
        if (effect.totalDuration <= 0) {
            explosionEffects.erase(explosionEffects.begin() + i);
        }
    }
}

void ExplosionController::render(sf::RenderWindow& window) {
    for (const auto& effect : explosionEffects) {
        for (const auto& particle : effect.particles) {
            if (particle.life > 0) {
                sf::CircleShape p(particle.size);
                p.setPosition(sf::Vector2f(particle.position.x - particle.size, particle.position.y - particle.size));
                p.setFillColor(particle.color);
                window.draw(p);
            }
        }

        if (effect.stage == ExplosionStage::EXPAND || effect.stage == ExplosionStage::FADE) {
            sf::CircleShape ring(effect.currentRadius);
            ring.setPosition(sf::Vector2f(effect.position.x - effect.currentRadius, effect.position.y - effect.currentRadius));
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineThickness(4.0f);
            ring.setOutlineColor(effect.currentColor);
            window.draw(ring);

            sf::CircleShape innerGlow(effect.currentRadius * 0.7f);
            innerGlow.setPosition(sf::Vector2f(effect.position.x - effect.currentRadius * 0.7f, effect.position.y - effect.currentRadius * 0.7f));
            sf::Color glowColor = effect.currentColor;
            glowColor.a = glowColor.a / 3;
            innerGlow.setFillColor(glowColor);
            window.draw(innerGlow);
        }

        if (effect.stage == ExplosionStage::FLASH) {
            sf::CircleShape flash(effect.maxRadius * 0.8f);
            flash.setPosition(sf::Vector2f(effect.position.x - effect.maxRadius * 0.8f, effect.position.y - effect.maxRadius * 0.8f));
            flash.setFillColor(sf::Color(255, 255, 255, 200));
            window.draw(flash);
        }
    }
}

void ExplosionController::handleExplosion(sf::Vector2f position, float radius) {
    printf("EXPLOSION TRIGGERED! Position: (%.2f, %.2f), Radius: %.2f\n",
        position.x, position.y, radius);

    ExplosionEffect effect;
    effect.position = position;
    effect.maxRadius = radius;
    effect.currentRadius = 0;
    effect.stage = ExplosionStage::FLASH;
    effect.stageTimer = 0;
    effect.currentColor = sf::Color(255, 100, 0, 220);
    effect.totalDuration = Constants::EXPLOSION_TOTAL_DURATION;
    effect.damageDealt = false;

    createParticleExplosion(effect);
    explosionEffects.push_back(effect);
}

void ExplosionController::createParticleExplosion(ExplosionEffect& effect) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDis(0, 2 * Constants::PI);
    std::uniform_real_distribution<float> speedDis(
        Constants::EXPLOSION_PARTICLE_MIN_SPEED, 
        Constants::EXPLOSION_PARTICLE_MAX_SPEED
    );
    std::uniform_real_distribution<float> lifeDis(
        Constants::EXPLOSION_PARTICLE_MIN_LIFE, 
        Constants::EXPLOSION_PARTICLE_MAX_LIFE
    );
    std::uniform_real_distribution<float> sizeDis(
        Constants::EXPLOSION_PARTICLE_MIN_SIZE, 
        Constants::EXPLOSION_PARTICLE_MAX_SIZE
    );

    for (int i = 0; i < Constants::EXPLOSION_PARTICLE_COUNT; i++) {
        Particle p;
        p.position = effect.position;

        float angle = angleDis(gen);
        float speed = speedDis(gen);
        p.velocity.x = cos(angle) * speed;
        p.velocity.y = sin(angle) * speed;

        p.maxLife = lifeDis(gen);
        p.life = p.maxLife;
        p.size = sizeDis(gen);

        int red = 255 - (i * 5);
        int green = 150 - (i * 3);
        p.color = sf::Color(std::max(100, red), std::max(50, green), 0, 255);

        effect.particles.push_back(p);
    }
}

void ExplosionController::updateExplosionStage(ExplosionEffect& effect, float deltaTime) {
    effect.stageTimer += deltaTime;

    switch (effect.stage) {
    case ExplosionStage::FLASH:
        if (effect.stageTimer > 0.15f) {
            effect.stage = ExplosionStage::EXPAND;
            effect.stageTimer = 0;
        }
        break;

    case ExplosionStage::EXPAND:
    {
        if (effect.stageTimer > 0.8f) {
            effect.stage = ExplosionStage::FADE;
            effect.stageTimer = 0;
        }
        float expandProgress = effect.stageTimer / 0.8f;
        effect.currentRadius = effect.maxRadius * expandProgress;
        effect.currentColor = sf::Color(255, 100, 0, 180);
    }
    break;

    case ExplosionStage::FADE:
    {
        float fadeProgress = effect.stageTimer / 1.0f;
        int alpha = static_cast<int>(180 * (1.0f - fadeProgress));
        effect.currentColor.a = std::max(0, alpha);
    }
    break;
    }
}

void ExplosionController::updateParticles(ExplosionEffect& effect, float deltaTime) {
    for (auto& particle : effect.particles) {
        if (particle.life <= 0) continue;

        particle.position.x += particle.velocity.x * deltaTime;
        particle.position.y += particle.velocity.y * deltaTime;

        particle.life -= deltaTime;

        float lifeRatio = particle.life / particle.maxLife;
        int alpha = static_cast<int>(255 * std::max(0.0f, lifeRatio));
        particle.color.a = alpha;

        particle.velocity.x *= 0.96f;
        particle.velocity.y *= 0.96f;

        if (lifeRatio < 0.3f) {
            particle.size *= 0.98f;
        }
    }
}

void ExplosionController::dealDamageToAsteroids(const ExplosionEffect& effect) {
    if (!asteroids || asteroids->empty()) return;

    printf("Checking %zu asteroids for explosion damage\n", asteroids->size());

    auto asteroidsCopy = *asteroids;

    int destroyedCount = 0;

    for (auto& asteroid : asteroidsCopy) {
        if (!asteroid || !asteroid->active) continue;

        float distance = std::sqrt(
            std::pow(asteroid->position.x - effect.position.x, 2) +
            std::pow(asteroid->position.y - effect.position.y, 2)
        );

        if (distance <= effect.maxRadius + asteroid->radius) {
            printf("Destroying asteroid at distance: %.2f\n", distance);

            asteroid->active = false;
            destroyedCount++;

            Message msg;
            msg.type = MessageType::AsteroidDestroyed;
            msg.sender = this;
            msg.payload = Constants::ASTEROID_SCORE_POINTS;
            MessageBus::publish(msg);
        }
    }

    printf("Explosion destroyed %d asteroids\n", destroyedCount);
}

