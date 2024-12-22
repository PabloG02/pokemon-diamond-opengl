#include "WorldScene.h"
#include "MouseHandler.h"
#include "freeglut.h"
#include "glig.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

void WorldScene::initialize() {
    //map.loadTerrain("./assets/Twinleaf Town - Terrain.txt");
    //map.loadMapObjects("./assets/Twinleaf Town - Objects.txt");
    map.loadTerrain("./assets/Route 201 - Terrain.txt");
    map.loadMapObjects("./assets/Route 201 - Objects.txt");
    player.loadFromFile("./assets/art/models/red/Red.obj");
    player.setCollisionMap(map.getCollisionMap());
    audioEngine.playMusic("./assets/audio/music/route-201.mp3");
    registerInputCallbacks();
}

void WorldScene::registerInputCallbacks() {
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        WorldScene::getInstance().keyboardCallback(key, x, y);
    });
    glutSpecialFunc([](int key, int x, int y) {
        WorldScene::getInstance().specialKeyboardCallbackMovement(key, x, y);
    });
    MouseHandler::initialize(alpha, beta, scale);
    glutMouseFunc(MouseHandler::onClick);
    glutMotionFunc(MouseHandler::onMotionClicked);
    glutMouseWheelFunc(MouseHandler::onMouseWheelScroll);
}

void WorldScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotated(beta, 1.0, 0.0, 0.0);
    glRotated(-alpha, 0.0, 1.0, 0.0);
    glScaled(scale, scale, scale);

    glTranslated(-player.getX(), -0.5 - player.getY(), -player.getZ());

    map.render();
    renderPlayer();

    menu.render();

    glutSwapBuffers();
}

void WorldScene::update(double deltaTime) {
    player.update(deltaTime);
}

void WorldScene::renderPlayer() {
    glPushMatrix();
    player.render();
    glPopMatrix();
}

void WorldScene::keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
    case 'm':
        if (menu.isVisible()) {
            glutSpecialFunc([](int key, int x, int y) {
                WorldScene::getInstance().specialKeyboardCallbackMovement(key, x, y);
            });
        } else {
            glutSpecialFunc([](int key, int x, int y) {
                WorldScene::getInstance().specialKeyboardCallbackMenu(key, x, y);
            });
        }
        menu.toggleVisibility();
        break;
    case 13: // Enter key
        if (menu.isVisible()) {
            menu.triggerSelection();
        } else {
            player.interact();
        }
        break;
    }
}

/**
 * @brief Callback function for handling special keyboard (like arrow keys) input.
 *
 * @param key The key code of the pressed special key, defined in GLUT (e.g., `GLUT_KEY_UP`).
 * @param x The x-coordinate of the mouse when the key was pressed (in window coordinates).
 * @param y The y-coordinate of the mouse when the key was pressed (in window coordinates).
 *
 * @note This does not return a value or produce output. Instead, it modifies the global state of
 * the application and triggers a redraw of the current frame using `glutPostRedisplay()`.
 *
 * @see glutSpecialFunc for registering this callback function.
 */
void WorldScene::specialKeyboardCallbackMovement(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // Up arrow key pressed
        player.queueMovement(Direction::UP);
        break;

    case GLUT_KEY_DOWN: // Down arrow key pressed
        player.queueMovement(Direction::DOWN);
        break;

    case GLUT_KEY_RIGHT: // Right arrow key pressed
        player.queueMovement(Direction::RIGHT);
        break;

    case GLUT_KEY_LEFT: // Left arrow key pressed
        player.queueMovement(Direction::LEFT);
        break;
    }
}

void WorldScene::specialKeyboardCallbackMenu(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // Up arrow key pressed
        menu.move(Direction::UP);
        break;

    case GLUT_KEY_DOWN: // Down arrow key pressed
        menu.move(Direction::DOWN);
        break;
    }
}
