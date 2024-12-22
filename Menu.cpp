#include "Menu.h"
#include "freeglut.h"
#include <numbers>

//void drawRoundedRect(float x, float y, float width, float height, float radius) {
//    int num_segments = 20; // Controls the smoothness of the rounded corner
//
//    // Coordinates for the inner rectangle (without rounded corners)
//    float x0 = x + radius;
//    float y0 = y + radius;
//    float x1 = x + width - radius;
//    float y1 = y + height - radius;
//
//    // Draw the four corner arcs with GL_TRIANGLE_FAN
//    glBegin(GL_TRIANGLE_FAN);
//    for (int i = 0; i < num_segments; ++i) {
//        float theta = i * std::numbers::pi / 2 / num_segments; // Quarter circle
//
//        // Bottom-left corner
//        if (i == 0)
//            glVertex2f(x0, y0); // Center of the fan
//        glVertex2f(x0 - cos(theta) * radius, y0 - sin(theta) * radius);
//
//        // Bottom-right corner
//        if (i == 0)
//            glVertex2f(x1, y0);
//        glVertex2f(x1 + cos(theta) * radius, y0 - sin(theta) * radius);
//
//        // Top-right corner
//        if (i == 0)
//            glVertex2f(x1, y1);
//        glVertex2f(x1 + cos(theta) * radius, y1 + sin(theta) * radius);
//
//        // Top-left corner
//        if (i == 0)
//            glVertex2f(x0, y1);
//        glVertex2f(x0 - cos(theta) * radius, y1 + sin(theta) * radius);
//    }
//    glEnd();
//
//    // Draw the four edges as rectangles
//    glBegin(GL_QUADS);
//
//    // Bottom edge
//    glVertex2f(x0, y);
//    glVertex2f(x1, y);
//    glVertex2f(x1, y0);
//    glVertex2f(x0, y0);
//
//    // Top edge
//    glVertex2f(x0, y + height);
//    glVertex2f(x1, y + height);
//    glVertex2f(x1, y1);
//    glVertex2f(x0, y1);
//
//    // Left edge
//    glVertex2f(x, y0);
//    glVertex2f(x + radius, y0);
//    glVertex2f(x + radius, y1);
//    glVertex2f(x, y1);
//
//    // Right edge
//    glVertex2f(x + width - radius, y0);
//    glVertex2f(x + width, y0);
//    glVertex2f(x + width, y1);
//    glVertex2f(x + width - radius, y1);
//
//    glEnd();
//
//    // Draw the center rectangle (without rounded corners)
//    glBegin(GL_QUADS);
//    glVertex2f(x0, y0);
//    glVertex2f(x1, y0);
//    glVertex2f(x1, y1);
//    glVertex2f(x0, y1);
//    glEnd();
//}

void Menu::render() {
    if (!visible)
        return;

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    glDisable(GL_DEPTH_TEST);
    // Switch to orthographic projection for 2D overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Coordinates and dimensions for the menu rectangles
    // Calculate menu position and button positions
    int menuX = windowWidth - marginRight - buttonWidth; // X position for both buttons
    int menuY = windowHeight - marginTop;                // Y position for the top of the menu

    // Draw white background rectangle for menu
    glColor3f(1.0, 1.0, 1.0); // White color for the background
    glBegin(GL_QUADS);
    glVertex2i(menuX, menuY);               // Top-left
    glVertex2i(menuX + buttonWidth, menuY); // Top-right
    glVertex2i(menuX + buttonWidth,
               menuY - menuEntries.size() * (buttonHeight + buttonSpacing));        // Bottom-right
    glVertex2i(menuX, menuY - menuEntries.size() * (buttonHeight + buttonSpacing)); // Bottom-left
    glEnd();

    // Draw each menu entry
    for (int i = 0; i < menuEntries.size(); i++) {
        int currentY = menuY - i * (buttonHeight + buttonSpacing);
        // Set text color
        if (i == selectedEntry) {
            glColor3ub(233, 127, 40);
        } else {
            glColor3f(0.0, 0.0, 0.0);
        }
        // Draw text
        glRasterPos2i(menuX + 10, currentY - 18);
        const unsigned char *text = reinterpret_cast<const unsigned char *>(menuEntries[i].c_str());
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, text);
    }

    // Restore previous projection and modelview matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Re-enable depth testing after 2D overlay
    glEnable(GL_DEPTH_TEST);
}

void Menu::toggleVisibility() {
    visible = !visible;
}

bool Menu::isVisible() const {
    return visible;
}

void Menu::move(Direction direction) {
    if (!visible)
        return;

    if (direction == Direction::UP) {
        selectedEntry = (selectedEntry - 1 + menuEntries.size()) % menuEntries.size();
    } else if (direction == Direction::DOWN) {
        selectedEntry = (selectedEntry + 1) % menuEntries.size();
    }
}

void Menu::triggerSelection() {
    if (!visible)
        return;

    switch (selectedEntry) {
    case 0:
        // Pokedex
        break;
    case 1:
        // Pokemon
        break;
    case 2:
        // Bag
        break;
    case 3:
        // Name
        break;
    case 4:
        // Save
        break;
    case 5:
        // Options
        break;
    case 6:
        // Exit
        glutLeaveMainLoop();
        break;
    }
}