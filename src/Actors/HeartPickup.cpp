#include "../../include/Actors/HeartPickup.hpp"
#include "../../include/Utils/Constants.hpp"
#include <cmath>

HeartPickup::HeartPickup() {
    radius = Constants::WEAPON_PICKUP_RADIUS;
}

void HeartPickup::render(sf::RenderWindow& window) {
    float time = glowClock.getElapsedTime().asSeconds();
    float pulse = (std::sin(time * Constants::GLOW_PULSE_SPEED * 1.5f) + 1.0f) * 0.5f;
    float alpha = Constants::GLOW_MIN_ALPHA + (Constants::GLOW_MAX_ALPHA - Constants::GLOW_MIN_ALPHA) * pulse;

    sf::CircleShape outerGlow(Constants::GLOW_OUTER_RADIUS);
    outerGlow.setOrigin({ Constants::GLOW_OUTER_RADIUS, Constants::GLOW_OUTER_RADIUS });
    outerGlow.setPosition(position);
    sf::Color outer(220, 20, 60, static_cast<std::uint8_t>(alpha * 0.3f * 255.0f));
    outerGlow.setFillColor(outer);
    window.draw(outerGlow);

    sf::CircleShape innerGlow(Constants::GLOW_INNER_RADIUS);
    innerGlow.setOrigin({ Constants::GLOW_INNER_RADIUS, Constants::GLOW_INNER_RADIUS });
    innerGlow.setPosition(position);
    sf::Color inner(220, 20, 60, static_cast<std::uint8_t>(alpha * 0.6f * 255.0f));
    innerGlow.setFillColor(inner);
    window.draw(innerGlow);

    Actor::render(window);
}
