#pragma once

namespace Constants {
    // Window
    constexpr unsigned int WINDOW_WIDTH = 1600;
    constexpr unsigned int WINDOW_HEIGHT = 1200;

    // Game Object Radii
    constexpr float SHIP_RADIUS = 16.0f;
    constexpr float BULLET_RADIUS = 6.0f;
    constexpr float WEAPON_PICKUP_RADIUS = 25.0f;
    constexpr float MIN_ASTEROID_RADIUS = 20.0f;
    constexpr int ASTEROID_RADIUS_RANGE = 25;

    // Asteroid Generation
    constexpr int INITIAL_ASTEROID_COUNT = 12;
    constexpr int MIN_ASTEROID_COUNT = 12;
    constexpr int MAX_ASTEROID_SPAWN_OFFSET = 2400;
    constexpr float BASE_ASTEROID_SPEED = 2.0f;
    constexpr int ASTEROID_SPEED_RANGE = 15;
    constexpr float ASTEROID_SPEED_DIVISOR = 3.0f;
    constexpr int ASTEROID_VELOCITY_RANGE = 200;
    constexpr float ASTEROID_VELOCITY_DIVISOR = 50.0f;
    constexpr int ASTEROID_SPAWN_MARGIN = 40;
    constexpr int ASTEROID_SPAWN_MIN_X = 20;

    // Ship
    constexpr float SHIP_START_X = 800.0f;
    constexpr float SHIP_START_Y = 1160.0f;

    // Bullet
    constexpr float BULLET_VELOCITY_INFLUENCE = 0.5f;
    constexpr float BULLET_ANGLE_OFFSET = 90.0f;
    constexpr float BULLET_POSITION_OFFSET = 28.0f;
    constexpr float BULLET_SCALE_MULTIPLIER = 8.0f;

    // Weapon Pickup
    constexpr float WEAPON_SPAWN_INTERVAL = 10.0f;
    constexpr float WEAPON_SCALE = 2.0f;
    constexpr int WEAPON_SPAWN_MARGIN = 50;
    constexpr int WEAPON_SPAWN_HEIGHT_MIN = 100;
    constexpr int WEAPON_SPAWN_HEIGHT_MAX = 1000;

    // Weapon Pickup Animation
    constexpr float BOB_SPEED = 3.0f;
    constexpr float BOB_AMPLITUDE = 10.0f;
    constexpr float MAX_LIFETIME = 30.0f;

    // Scaling
    constexpr float ASTEROID_SCALE_MULTIPLIER = 2.0f;

    // Math
    constexpr float PI = 3.14159f;
    constexpr float DEGREES_TO_RADIANS = PI / 180.0f;

    // UI
    constexpr float GAME_OVER_LINE_HEIGHT = 8.0f;
    constexpr float GAME_OVER_LINE_Y = 1192.0f;

    // Weapon Rarity System
    constexpr float RIFLE_SPAWN_RATE = 45.0f;        // Common
    constexpr float REVOLVER_SPAWN_RATE = 30.0f;     // Uncommon
    constexpr float SHOTGUN_SPAWN_RATE = 15.0f;      // Rare
    constexpr float ROCKET_SPAWN_RATE = 7.0f;        // Epic
    constexpr float FLAMETHROWER_SPAWN_RATE = 3.0f;  // Legendary

    // Glow Effects
    constexpr float GLOW_PULSE_SPEED = 2.0f;
    constexpr float GLOW_MIN_ALPHA = 0.3f;
    constexpr float GLOW_MAX_ALPHA = 0.8f;
    constexpr float GLOW_OUTER_RADIUS = 40.0f;
    constexpr float GLOW_INNER_RADIUS = 25.0f;

    // Explosion constants
    static const float EXPLOSION_RADIUS = 500.0f;
    static const int ASTEROID_SCORE_POINTS = 10;

}
