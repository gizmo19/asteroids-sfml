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
    static const float EXPLOSION_RADIUS = 400.0f;
    static const int ASTEROID_SCORE_POINTS = 10;

    // Ship Movement
    constexpr float SHIP_ACCELERATION = 0.4f;
    constexpr float SHIP_FRICTION = 0.92f;
    constexpr float SHIP_MAX_SPEED = 12.0f;

    // Boundary Checks
    constexpr float BOUNDARY_OFFSET = 50.0f;

    // Explosion Effects
    constexpr float EXPLOSION_TOTAL_DURATION = 2.0f;
    constexpr float EXPLOSION_STAGE_THRESHOLD = 0.1f;
    constexpr int EXPLOSION_PARTICLE_COUNT = 25;
    constexpr float EXPLOSION_PARTICLE_MIN_SPEED = 50.0f;
    constexpr float EXPLOSION_PARTICLE_MAX_SPEED = 200.0f;
    constexpr float EXPLOSION_PARTICLE_MIN_LIFE = 0.8f;
    constexpr float EXPLOSION_PARTICLE_MAX_LIFE = 1.5f;
    constexpr float EXPLOSION_PARTICLE_MIN_SIZE = 1.5f;
    constexpr float EXPLOSION_PARTICLE_MAX_SIZE = 4.0f;

    namespace Audio {
        constexpr const char* SOUNDTRACK_PATH = "assets/music/soundtrack.mp3";
		constexpr const char* LASER_SOUND_PATH = "assets/music/shoot.mp3";
		constexpr const char* DESTRUCTION_SOUND_PATH = "assets/music/destruction.mp3";
		constexpr const char* EXPLOSION_SOUND_PATH = "assets/music/explosion.mp3";
        constexpr const char* DEATH_SOUND_PATH = "assets/music/death.mp3";
        constexpr float DEFAULT_MUSIC_VOLUME = 1.0f;
		constexpr float DEFAULT_LASER_VOLUME = 1.0f;
		constexpr float DEFAULT_DESTRUCTION_VOLUME = 1.0f;
		constexpr float DEFAULT_EXPLOSION_VOLUME = 1.0f;
		constexpr float DEFAULT_DEATH_VOLUME = 1.0f;
    }

    // Weapon Stats
    namespace WeaponStats {
        namespace Rifle {
            constexpr float FIRE_RATE = 0.001f;
            constexpr int BULLET_COUNT = 1;
            constexpr float SPREAD = 0.0f;
            constexpr float BULLET_SPEED = 12.0f;
            constexpr float DURATION = 8.0f;
            constexpr float SPEED_MULTIPLIER = 1.0f;
        }

        namespace Revolver {
            constexpr float FIRE_RATE = 1.0f;
            constexpr int BULLET_COUNT = 1;
            constexpr float SPREAD = 0.0f;
            constexpr float BULLET_SPEED = 8.0f;
            constexpr float DURATION = 10.0f;
            constexpr float SPEED_MULTIPLIER = 1.5f;
        }

        namespace Shotgun {
            constexpr float FIRE_RATE = 0.8f;
            constexpr int BULLET_COUNT = 5;
            constexpr float SPREAD = 45.0f;
            constexpr float BULLET_SPEED = 10.0f;
            constexpr float DURATION = 6.0f;
            constexpr float SPEED_MULTIPLIER = 0.8f;
        }

        namespace RocketLauncher {
            constexpr float FIRE_RATE = 2.0f;
            constexpr int BULLET_COUNT = 1;
            constexpr float SPREAD = 0.0f;
            constexpr float BULLET_SPEED = 8.0f;
            constexpr float DURATION = 10.0f;
            constexpr float SPEED_MULTIPLIER = 0.7f;
        }

        namespace Flamethrower {
            constexpr float FIRE_RATE = 0.1f;
            constexpr int BULLET_COUNT = 3;
            constexpr float SPREAD = 25.0f;
            constexpr float BULLET_SPEED = 6.0f;
            constexpr float DURATION = 8.0f;
            constexpr float SPEED_MULTIPLIER = 0.9f;
        }

        namespace Default {
            constexpr float FIRE_RATE = 0.2f;
            constexpr int BULLET_COUNT = 1;
            constexpr float SPREAD = 0.0f;
            constexpr float BULLET_SPEED = 10.0f;
            constexpr float DURATION = 0.0f;
            constexpr float SPEED_MULTIPLIER = 1.0f;
        }
    }
}
