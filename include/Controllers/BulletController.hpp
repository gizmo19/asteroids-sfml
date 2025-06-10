#pragma once
#include "Controller.hpp"

class BulletController : public Controller {
public:
    BulletController();
    void update(float deltaTime) override;

private:
    void updateMovement(float deltaTime);
    void checkBounds();
};
