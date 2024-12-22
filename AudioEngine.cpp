#include "AudioEngine.h"
#include <iostream>

void AudioEngine::initialize() {
    // Initialize the audio engine
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine" << std::endl;
    }
}

void AudioEngine::playSound(const std::string &soundFile) {
    // Load and play a sound effect
    ma_result result = ma_engine_play_sound(&engine, soundFile.c_str(), nullptr);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << soundFile << " (Error: " << result << ")"
                  << std::endl;
    } else {
        std::cout << "Playing sound: " << soundFile << std::endl;
    }
}

void AudioEngine::playMusic(const std::string &musicFile, bool loop) {
    // Uninitialize previous music sound if it's already initialized
    ma_sound_stop(&musicSound);
    ma_sound_uninit(&musicSound);

    // Load and play music
    ma_result result = ma_sound_init_from_file(&engine, musicFile.c_str(), MA_SOUND_FLAG_STREAM,
                                               nullptr, nullptr, &musicSound);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load music: " << musicFile << " (Error: " << result << ")"
                  << std::endl;
        return;
    }

    ma_sound_set_looping(&musicSound, loop ? MA_TRUE : MA_FALSE);

    result = ma_sound_start(&musicSound);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to start music: " << musicFile << " (Error: " << result << ")"
                  << std::endl;
    } else {
        std::cout << "Playing music: " << musicFile << (loop ? " (looping)" : "") << std::endl;
    }
}

void AudioEngine::stopAllSounds() {
    ma_sound_stop(&musicSound);
    ma_sound_uninit(&musicSound);

    ma_result result = ma_engine_stop(&engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to stop sounds!" << std::endl;
    } else {
        std::cout << "Stopping all sounds." << std::endl;
    }
}
