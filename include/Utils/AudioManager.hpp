#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    static AudioManager& getInstance();

    void playMusic(const std::string& filename, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);

    void playSound(const std::string& filename);
    void setSoundVolume(float volume);

    bool isMusicPlaying() const;

private:
    AudioManager();
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    sf::Music music;
    float musicVolume;
    float soundVolume;

    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    sf::SoundBuffer dummyBuffer;
    sf::Sound sound;
};
