#pragma once
#include "../../include/Controllers/Controller.hpp"
#include <string>

class AudioController : public Controller {
public:
    AudioController(const std::string& musicFile);
    ~AudioController();

    void update(float deltaTime) override;

private:
    std::string musicFilename;
    bool initialized = false;
};
