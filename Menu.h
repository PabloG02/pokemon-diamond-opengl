#pragma once

#include "Direction.h"
#include <array>
#include <string>

class Menu {
  public:
    void render();
    void toggleVisibility();
    bool isVisible() const;
    void move(Direction direction);
    void triggerSelection();

  private:
    // State
    bool visible{false};
    int selectedEntry{0};

    // Menu entries and layout settings
    std::array<std::string, 7> menuEntries{"Pokedex", "Pokemon", "Bag", "Name",
                                           "Save",    "Options", "Exit"};
    const int buttonWidth{100};
    const int buttonHeight{25};
    const int marginRight{40};  // Margin from the right edge of the screen
    const int marginTop{40};    // Margin from the top edge of the screen
    const int buttonSpacing{0}; // Space between buttons
};
