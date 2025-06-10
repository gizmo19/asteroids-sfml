#pragma once

class Actor;
class Scene;

class Controller {
public:
    Controller();
    virtual ~Controller();

    virtual void update(float deltaTime) = 0;

    void attachToActor(Actor* actor);
    void attachToScene(Scene* scene);

protected:
    Actor* attachedActor;
    Scene* attachedScene;
};
