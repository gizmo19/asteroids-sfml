#pragma once
#include "Controller.hpp"
#include <vector>
#include <memory>

class Actor;

class CollisionController : public Controller {
public:
    CollisionController();
    void update(float deltaTime) override;

    void setShip(std::shared_ptr<Actor> ship);
    void setBullets(std::vector<std::shared_ptr<Actor>>* bullets);
    void setAsteroids(std::vector<std::shared_ptr<Actor>>* asteroids);

private:
    std::shared_ptr<Actor> ship;
    std::vector<std::shared_ptr<Actor>>* bullets;
    std::vector<std::shared_ptr<Actor>>* asteroids;

    bool checkCollision(std::shared_ptr<Actor> a, std::shared_ptr<Actor> b);
    void handleBulletAsteroidCollisions();
    void handleShipAsteroidCollisions();
};
