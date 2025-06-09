#pragma once
#include "Controller.hpp"

class AsteroidController : public Controller {
public:
    AsteroidController();
    void update(float deltaTime) override;

private:
    float angularVelocity;

    void updateMovement(float deltaTime);
    void wrapPosition();
};
