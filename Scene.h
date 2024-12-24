#pragma once
#include "AudioEngine.h"

class Scene {
  public:
    // Virtual destructor
    virtual ~Scene() = default;

    virtual void initialize() = 0;
    virtual void render() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void registerInputCallbacks() = 0;

  protected:
    AudioEngine &audioEngine = AudioEngine::getInstance();
    bool isInitialized{false};
};
