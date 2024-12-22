#pragma once

#include <utility>

class MouseHandler {
  public:
    static void initialize(double &alphaRef, double &betaRef, double &scaleRef);
    static void onClick(int button, int state, int x, int y);
    static void onMotionClicked(int x, int y);
    static void onMouseWheelScroll(int button, int dir, int x, int y);

  private:
    static double *alpha; // Pointer to main alpha
    static double *beta;  // Pointer to main beta
    static double *scale; // Pointer to main scale

    // Keeps track of the left mouse button's state and the last known position of the mouse cursor.
    static bool leftButtonPressed;
    static std::pair<int, int> lastPosition;
};