#include "../../include/Controllers/AudioController.hpp"
#include "../../include/Utils/AudioManager.hpp"

AudioController::AudioController(const std::string& musicFile) : musicFilename(musicFile) {}

AudioController::~AudioController() {}

void AudioController::update(float deltaTime) {
    if (!initialized && !musicFilename.empty()) {
        AudioManager::getInstance().playMusic(musicFilename, true);
        initialized = true;
    }
}
