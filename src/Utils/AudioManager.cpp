#include "../../include/Utils/AudioManager.hpp"
#include "../../include/Utils/Constants.hpp"

AudioManager::AudioManager()
    : musicVolume(Constants::Audio::DEFAULT_MUSIC_VOLUME),
    soundVolume(Constants::Audio::DEFAULT_LASER_VOLUME),
    sound(dummyBuffer) {
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::playMusic(const std::string& filename, bool loop) {
    if (music.openFromFile(filename)) {
        music.setLooping(loop);
        music.setVolume(musicVolume);
        music.play();
    }
}

void AudioManager::stopMusic() {
    music.stop();
}

void AudioManager::pauseMusic() {
    music.pause();
}

void AudioManager::resumeMusic() {
    music.play();
}

void AudioManager::setMusicVolume(float volume) {
    musicVolume = volume;
    music.setVolume(volume);
}

void AudioManager::playSound(const std::string& filename) {
    if (soundBuffers.find(filename) == soundBuffers.end()) {
        if (!soundBuffers[filename].loadFromFile(filename)) {
            return;
        }
    }

    sound.setBuffer(soundBuffers[filename]);
    sound.setVolume(soundVolume);
    sound.play();
}

void AudioManager::setSoundVolume(float volume) {
    soundVolume = volume;
}

bool AudioManager::isMusicPlaying() const {
    return music.getStatus() == sf::Music::Status::Playing;
}
