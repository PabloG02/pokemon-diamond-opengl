#include "BattleScene.h"
#include "MouseHandler.h"
#include "freeglut.h"
#include "WorldScene.h"

void BattleScene::initialize() {
    if (!isInitialized) {
        // Battle background
        battleBackground.loadFromFile(
            "./assets/art/models/battle-scene/Map_Base_01_First_Steppe.obj");
        // Pokemon models
        playerPokemon.loadFromFile("./assets/art/models/staraptor/staraptor.obj");
        rivalPokemon.loadFromFile("./assets/art/models/kricketot/kricketot.obj");
        isInitialized = true;
    }
    // Audio
    audioEngine.playMusic("./assets/audio/music/battle-wild-pokemon.mp3");
    registerInputCallbacks();
}

void BattleScene::registerInputCallbacks() {
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        BattleScene::getInstance().keyboardCallback(key, x, y);
    });
    glutSpecialFunc([](int key, int x, int y) {
        BattleScene::getInstance().specialKeyboardCallback(key, x, y);
    });
    MouseHandler::initialize(alpha, beta, scale);
    glutMouseFunc(MouseHandler::onClick);
    glutMotionFunc(MouseHandler::onMotionClicked);
    glutMouseWheelFunc(MouseHandler::onMouseWheelScroll);
}

void BattleScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Battle background
    glPushMatrix();
    glRotated(beta, 1.0, 0.0, 0.0);
    glRotated(-alpha, 0.0, 1.0, 0.0);
    glScaled(scale, scale, scale);

    battleBackground.render();
    glPopMatrix();

    // Player Pokemon
    glPushMatrix();
    glRotated(beta, 1.0, 0.0, 0.0);
    glRotated(-alpha, 0.0, 1.0, 0.0);
    glRotated(180, 0.0, 1.0, 0.0);
    glScaled(scale * 4, scale * 4, scale * 4);
    glTranslated(0.0, 0.0, -3.0);
    playerPokemon.render();
    glPopMatrix();

    // Rival Pokemon
    glPushMatrix();
    glRotated(beta, 1.0, 0.0, 0.0);
    glRotated(-alpha, 0.0, 1.0, 0.0);
    glScaled(scale * 4, scale * 4, scale * 4);
    glTranslated(0.0, 0.0, -3.0);
    rivalPokemon.render();
    glPopMatrix();

    drawUI();

    glutSwapBuffers();
}

void BattleScene::update(double deltaTime) {
    // Update logic
}

void BattleScene::drawUI() {
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

    // Draw each menu entry
    for (int i = 0; i < battleOptions.size(); i++) {
        int buttonX = windowWidth - marginRight - buttonWidth;
        int buttonY = windowHeight - marginBottom - i * (buttonHeight + buttonSpacing);

        // Draw white background rectangle for button
        glColor3f(1.0, 1.0, 1.0); // White color for the background
        glBegin(GL_QUADS);
        glVertex2i(buttonX, buttonY);                              // Top-left
        glVertex2i(buttonX + buttonWidth, buttonY);                // Top-right
        glVertex2i(buttonX + buttonWidth, buttonY - buttonHeight); // Bottom-right
        glVertex2i(buttonX, buttonY - buttonHeight);               // Bottom-left
        glEnd();

        // Set text color
        if (i == selectedEntry) {
            glColor3ub(255, 0, 0);
        } else {
            glColor3f(0.0, 0.0, 0.0);
        }
        // Draw text
        glRasterPos2i(buttonX + 10, buttonY - 18);
        const unsigned char *text =
            reinterpret_cast<const unsigned char *>(battleOptions[i].c_str());
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

void BattleScene::changeSelectedOption(Direction direction) {
    if (!visible)
        return;

    if (direction == Direction::UP) {
        selectedEntry = (selectedEntry - 1 + battleOptions.size()) % battleOptions.size();
    } else if (direction == Direction::DOWN) {
        selectedEntry = (selectedEntry + 1) % battleOptions.size();
    }
}

extern Scene *scene;
void BattleScene::triggerSelection() {
    if (!visible)
        return;

    switch (selectedEntry) {
    case 0:
        // Fight
        break;
    case 1:
        // Pokemon
        break;
    case 2:
        // Bag
        break;
    case 3:
        // Run
        scene = &WorldScene::getInstance();
        scene->initialize();
        break;
    }
}

void BattleScene::keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
    case 13: // Enter key
    case 'c':
    case 'C':
        triggerSelection();
        break;
    }
}

void BattleScene::specialKeyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // Up arrow key pressed
        changeSelectedOption(Direction::UP);
        break;

    case GLUT_KEY_DOWN: // Down arrow key pressed
        changeSelectedOption(Direction::DOWN);
        break;
    }
}
