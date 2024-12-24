#pragma once

#include "Object.h"
#include "Scene.h"
#include "Direction.h"
#include <array>

class BattleScene : public Scene {
  public:
    static BattleScene &getInstance() {
        static BattleScene instance;
        return instance;
    }

    void initialize() override;
    void registerInputCallbacks() override;
    void render() override;
    void update(double deltaTime) override;

    void keyboardCallback(unsigned char key, int x, int y);
    void specialKeyboardCallback(unsigned char key, int x, int y);

    void drawUI();
    void changeSelectedOption(Direction direction);
    void triggerSelection();

  private:
    BattleScene() = default; // Private constructor for singleton
    Object battleBackground;
    Object playerPokemon;
    Object rivalPokemon;

    double alpha{20.0};
    double beta{24.0};
    double scale{0.065};

    // UI
    // State
    bool visible{true};
    int selectedEntry{0};

    // Menu entries and layout settings
    std::array<std::string, 4> battleOptions{"Fight", "Pokemon", "Bag", "Run"};
    const int buttonWidth{100};
    const int buttonHeight{25};
    const int marginRight{40};  // Margin from the right edge of the screen
    const int marginBottom{40}; // Margin from the top edge of the screen
    const int buttonSpacing{8}; // Space between buttons
};
