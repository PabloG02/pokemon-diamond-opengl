#pragma once

#include "Object.h"
#include "Scene.h"

class IntroScene : public Scene {
  public:
    static IntroScene &getInstance() {
        static IntroScene instance;
        return instance;
    }

    void initialize() override;
    void registerInputCallbacks() override;
    void render() override;
    void update(double deltaTime) override;

    void keyboardCallback(unsigned char key, int x, int y);

  private:
    IntroScene() = default; // Private constructor for singleton
    Object dialga;
    GLuint pokemonLogoTexture;
};
