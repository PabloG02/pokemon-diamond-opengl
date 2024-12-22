#pragma once

#include "miniaudio.h"
#include <memory>
#include <string>
#include <unordered_map>

class AudioEngine {
public:
    static AudioEngine &getInstance() {
        static AudioEngine instance;
        return instance;
    }

    void initialize();
    void playSound(const std::string &soundFile);
    void playMusic(const std::string &musicFile, bool loop = true);
    void stopAllSounds();

private:
    ma_engine engine;
    ma_sound musicSound;
};
