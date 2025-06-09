#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <cmath>

struct GameObject {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius;
    float angle = 0.f;
    float angularVelocity = 0.f;
    bool active = true;
};

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 1200;

int main() {
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Asteroids SFML");
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/pixelart_starfield.png")) {
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({
        static_cast<float>(WIDTH) / backgroundTexture.getSize().x,
        static_cast<float>(HEIGHT) / backgroundTexture.getSize().y
        });

    sf::Vector2f shipPos = { WIDTH / 2.f, HEIGHT - 40.f };
    sf::Vector2f shipVel = { 0.f, 0.f };
    float shipAngle = 0.f;
    constexpr float SHIP_ACCELERATION = 0.4f;
    constexpr float SHIP_FRICTION = 0.92f;
    constexpr float MAX_SHIP_SPEED = 12.f;

    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("assets/images/space_fighter.png")) {
        return 1;
    }

    sf::Sprite shipSprite(shipTexture);

    sf::Vector2u size = shipTexture.getSize();
    shipSprite.setOrigin(sf::Vector2f(
        static_cast<float>(size.x) / 2.f,
        static_cast<float>(size.y) / 2.f
    ));

    sf::Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("assets/images/asteroid.png")) {
        return 1;
    }

    std::vector<GameObject> asteroids;
    for (int i = 0; i < 12; ++i) {
        GameObject a;
        a.radius = 20.f + std::rand() % 25;
        a.pos = {
            static_cast<float>(std::rand() % (WIDTH - 40) + 20),
            static_cast<float>(-std::rand() % (HEIGHT * 2))
        };
        a.vel = {
            (static_cast<float>(std::rand() % 200) - 100.f) / 50.f,
            2.f + static_cast<float>(std::rand() % 15) / 3.f
        };
        a.angularVelocity = (static_cast<float>(std::rand() % 200) - 100.f) / 20.f;
        a.active = true;
        asteroids.push_back(a);
    }

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/bullet.png")) {
        return 1;
    }

    std::vector<GameObject> bullets;
    int score = 0;
    bool gameOver = false;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (!gameOver) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            float dx = static_cast<float>(mousePos.x) - shipPos.x;
            float dy = static_cast<float>(mousePos.y) - shipPos.y;
            shipAngle = std::atan2(dy, dx) * 180.f / 3.14159f + 90.f;

            sf::Vector2f acceleration = { 0.f, 0.f };

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
                acceleration.x -= SHIP_ACCELERATION;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
                acceleration.x += SHIP_ACCELERATION;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
                acceleration.y -= SHIP_ACCELERATION;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
                acceleration.y += SHIP_ACCELERATION;

            shipVel += acceleration;

            shipVel *= SHIP_FRICTION;

            float speed = std::sqrt(shipVel.x * shipVel.x + shipVel.y * shipVel.y);
            if (speed > MAX_SHIP_SPEED) {
                shipVel.x = (shipVel.x / speed) * MAX_SHIP_SPEED;
                shipVel.y = (shipVel.y / speed) * MAX_SHIP_SPEED;
            }

            shipPos += shipVel;

            if (shipPos.x < 20) {
                shipPos.x = 20;
                shipVel.x = std::abs(shipVel.x) * 0.3f;
            }
            if (shipPos.x > WIDTH - 20) {
                shipPos.x = WIDTH - 20;
                shipVel.x = -std::abs(shipVel.x) * 0.3f;
            }
            if (shipPos.y < 20) {
                shipPos.y = 20;
                shipVel.y = std::abs(shipVel.y) * 0.3f;
            }
            if (shipPos.y > HEIGHT - 20) {
                shipPos.y = HEIGHT - 20;
                shipVel.y = -std::abs(shipVel.y) * 0.3f;
            }

            static bool canShoot = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (canShoot) {
                    GameObject b;
                    b.radius = 6.f;

                    float angleRad = (shipAngle - 90.f) * 3.14159f / 180.f;
                    float bulletSpeed = 10.f;

                    b.pos = {
                        shipPos.x + std::cos(angleRad) * 28.f,
                        shipPos.y + std::sin(angleRad) * 28.f
                    };
                    b.vel = {
                        std::cos(angleRad) * bulletSpeed + shipVel.x * 0.5f,
                        std::sin(angleRad) * bulletSpeed + shipVel.y * 0.5f
                    };
                    b.angle = shipAngle;
                    b.active = true;
                    bullets.push_back(b);
                    canShoot = false;
                }
            }
            else {
                canShoot = true;
            }

            for (auto& b : bullets) {
                if (b.active) {
                    b.pos += b.vel;
                    if (b.pos.x < 0 || b.pos.x > WIDTH || b.pos.y < 0 || b.pos.y > HEIGHT) {
                        b.active = false;
                    }
                }
            }

            for (auto& a : asteroids) {
                if (!a.active) continue;
                a.pos += a.vel;
                if (a.pos.y - a.radius > HEIGHT) {
                    a.pos.y = -a.radius;
                    a.pos.x = static_cast<float>(std::rand() % (WIDTH - 40) + 20);
                }
                float dx = shipPos.x - a.pos.x, dy = shipPos.y - a.pos.y;
                if (std::sqrt(dx * dx + dy * dy) < a.radius + 16.f) {
                    gameOver = true;
                }
            }

            for (auto& b : bullets) {
                if (!b.active) continue;
                for (auto& a : asteroids) {
                    if (!a.active) continue;
                    float dx = b.pos.x - a.pos.x, dy = b.pos.y - a.pos.y;
                    if (std::sqrt(dx * dx + dy * dy) < a.radius + b.radius) {
                        b.active = false;
                        a.active = false;
                        score += 10;
                        a.pos.y = -a.radius;
                        a.pos.x = static_cast<float>(std::rand() % (WIDTH - 40) + 20);
                        a.active = true;
                        break;
                    }
                }
            }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                [](const GameObject& o) { return !o.active; }), bullets.end());

            if (std::rand() % 40 == 0 && asteroids.size() < 12) {
                GameObject a;
                a.radius = 24.f + std::rand() % 18;
                a.pos = { static_cast<float>(std::rand() % (WIDTH - 40) + 20), -a.radius };
                a.vel = { 0.f, 3.f + static_cast<float>(std::rand() % 10) / 3.f };
                a.active = true;
                asteroids.push_back(a);
            }
        }

        window.clear();

        window.draw(backgroundSprite);

        shipSprite.setPosition(shipPos);
        shipSprite.setRotation(sf::degrees(shipAngle));
        window.draw(shipSprite);

        for (const auto& a : asteroids) {
            if (!a.active) continue;
            sf::Sprite asteroidSprite(asteroidTexture);
            sf::Vector2u texSize = asteroidTexture.getSize();
            asteroidSprite.setOrigin(sf::Vector2f(
                static_cast<float>(texSize.x) / 2.f,
                static_cast<float>(texSize.y) / 2.f
            ));
            float scale = (a.radius * 2.f) / static_cast<float>(texSize.x);
            asteroidSprite.setScale({ scale, scale });
            asteroidSprite.setPosition(a.pos);
            window.draw(asteroidSprite);
        }

        for (const auto& b : bullets) {
            if (!b.active) continue;
            sf::Sprite bulletSprite(bulletTexture);
            sf::Vector2u texSize = bulletTexture.getSize();
            bulletSprite.setOrigin(sf::Vector2f(
                static_cast<float>(texSize.x) / 2.f,
                static_cast<float>(texSize.y) / 2.f
            ));
            float scale = (b.radius * 8.f) / static_cast<float>(texSize.x);
            bulletSprite.setScale({ scale, scale });
            bulletSprite.setPosition(b.pos);
            bulletSprite.setRotation(sf::degrees(b.angle));
            window.draw(bulletSprite);
        }

        if (gameOver) {
            sf::RectangleShape line(sf::Vector2f(WIDTH, 8));
            line.setPosition({ 0.f, static_cast<float>(HEIGHT - 8) });
            line.setFillColor(sf::Color::Red);
            window.draw(line);
        }

        window.display();
        sf::sleep(sf::milliseconds(18));
    }
}
