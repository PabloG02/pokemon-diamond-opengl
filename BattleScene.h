#pragma once

#include "Object.h"
#include "Scene.h"
#include "Direction.h"
#include "Pokemon.h"
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
    void drawHPBars(int windowWidth, int windowHeight, const Pokemon &playerPkm,
                    const Pokemon &rivalPkm);
    void changeSelectedOption(Direction direction);
    void triggerSelection();
    void runFightSequence();
    void endBattle();

  private:
    BattleScene() = default; // Private constructor for singleton
    Object battleBackground;
    Object playerPokemon;
    Object rivalPokemon;
    // TODO
    Pokemon playerPkm{"Staraptor", 60, 25, 10};
    Pokemon rivalPkm{"Kricketot", 50, 20, 8};

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
