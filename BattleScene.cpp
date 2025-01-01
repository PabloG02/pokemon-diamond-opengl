#include "BattleScene.h"
#include "MouseHandler.h"
#include "freeglut.h"
#include "WorldScene.h"
#include <iostream>

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
    // TODO
    playerPkm = Pokemon{"Staraptor", 60, 25, 10};
    rivalPkm = Pokemon{"Kricketot", 50, 20, 8};

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
    // Rotate camera
    alpha += deltaTime * 1.5;
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

    drawHPBars(windowWidth, windowHeight, playerPkm, rivalPkm);

    // Restore previous projection and modelview matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Re-enable depth testing after 2D overlay
    glEnable(GL_DEPTH_TEST);
}

// TODO
void BattleScene::drawHPBars(int windowWidth, int windowHeight, const Pokemon &playerPkm,
                             const Pokemon &rivalPkm) {
    glColor3ub(255, 255, 255);
    glRasterPos2i(20, windowHeight - 40);
    std::string playerHPText = playerPkm.name + " HP: " + std::to_string(playerPkm.hp);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                     reinterpret_cast<const unsigned char *>(playerHPText.c_str()));

    glRasterPos2i(20, windowHeight - 70);
    std::string rivalHPText = rivalPkm.name + " HP: " + std::to_string(rivalPkm.hp);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                     reinterpret_cast<const unsigned char *>(rivalHPText.c_str()));
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

void BattleScene::triggerSelection() {
    if (!visible)
        return;

    switch (selectedEntry) {
    case 0:
        // Fight
        runFightSequence();
        break;
    case 1:
        // Pokemon
        break;
    case 2:
        // Bag
        break;
    case 3:
        // Run
        endBattle();
        break;
    }
}

// TODO
void BattleScene::runFightSequence() {
    // Player attacks first
    {
        // A simple damage formula: base + random variance
        int damage = std::max(playerPkm.attack - rivalPkm.defense, 0);
        damage += rand() % 5; // small random bonus
        rivalPkm.takeDamage(damage);

        std::cout << playerPkm.name << " deals " << damage << " damage to " << rivalPkm.name
                  << "! (HP left: " << rivalPkm.hp << ")\n";

        // Check if rival fainted
        if (rivalPkm.isFainted()) {
            std::cout << rivalPkm.name << " fainted!\n";
            // Transition out of battle, or mark battle as over
            endBattle();
            return;
        }
    }

    // Rival attacks next
    {
        int damage = std::max(rivalPkm.attack - playerPkm.defense, 0);
        damage += rand() % 5;
        playerPkm.takeDamage(damage);

        std::cout << rivalPkm.name << " deals " << damage << " damage to " << playerPkm.name
                  << "! (HP left: " << playerPkm.hp << ")\n";

        // Check if player fainted
        if (playerPkm.isFainted()) {
            std::cout << playerPkm.name << " fainted!\n";
            // Transition out of battle, or mark battle as over
            endBattle();
            return;
        }
    }
}

extern Scene *scene;
void BattleScene::endBattle() {
    scene = &WorldScene::getInstance();
    scene->initialize();
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
