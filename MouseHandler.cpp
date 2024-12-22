#include "MouseHandler.h"
#include "freeglut.h"
#include <cmath>
#include <iostream>

bool MouseHandler::leftButtonPressed = false;
std::pair<int, int> MouseHandler::lastPosition;
double *MouseHandler::alpha = nullptr;
double *MouseHandler::beta = nullptr;
double *MouseHandler::scale = nullptr;

void MouseHandler::initialize(double &alphaRef, double &betaRef, double &scaleRef) {
    alpha = &alphaRef;
    beta = &betaRef;
    scale = &scaleRef;
}

/**
 * @brief Callback function for handling mouse click events.
 *
 * @param button The mouse button that was pressed (e.g., GLUT_LEFT_BUTTON).
 * @param state The state of the mouse button (e.g., GLUT_DOWN or GLUT_UP).
 * @param x The x-coordinate of the mouse cursor at the time of the click.
 * @param y The y-coordinate of the mouse cursor at the time of the click.
 */
void MouseHandler::onClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        leftButtonPressed = (state == GLUT_DOWN);
        lastPosition = {x, y};
    }
}

/**
 * @brief Callback function for handling mouse motion events when a button is pressed.
 *
 * @param x The current x-coordinate of the mouse cursor.
 * @param y The current y-coordinate of the mouse cursor.
 */
void MouseHandler::onMotionClicked(int x, int y) {
    if (leftButtonPressed) {
        constexpr double ROTATION_INCREMENT = 0.3;
        auto [lastX, lastY] = lastPosition;

        // Top left corner is (0, 0)
        int deltaX = x - lastX;
        int deltaY = lastY - y;

        if (deltaX > 0)
            *alpha -= ROTATION_INCREMENT;
        else if (deltaX < 0)
            *alpha += ROTATION_INCREMENT;

        if (deltaY > 0)
            *beta -= ROTATION_INCREMENT;
        else if (deltaY < 0)
            *beta += ROTATION_INCREMENT;

        // Normalize alpha to the range [0, 360)
        // std::fmod returns the floating-point remainder of the division operation
        *alpha = std::fmod(*alpha + 360.0, 360.0);
        *beta = std::fmod(*beta + 360.0, 360.0);

        lastPosition = {x, y};
    }
}

void MouseHandler::onMouseWheelScroll(int wheel, int direction, int x, int y) {
    if (direction == 1) {
        // Zoom in
        *scale += 0.01;
    } else if (direction == -1) {
        // Zoom out
        *scale -= 0.01;
    }
}
