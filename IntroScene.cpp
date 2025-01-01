#include "IntroScene.h"
#include "WorldScene.h"
#include "freeglut.h"
#include "TextureLoader.h"

void IntroScene::initialize() {
    // Dialga
    dialga.loadFromFile("./assets/art/models/title-screen-dialga/Dialga.obj");
    dialga.setGroupWithScrollingTexture("iar_skin", 0.0, -0.2);
    // Pokemon logo
    pokemonLogoTexture = TextureLoader::loadTexture("./assets/pokemon-diamond-logo-big.png");
    // Audio
    audioEngine.initialize();
    audioEngine.playMusic("./assets/audio/music/title-screen.mp3");
    registerInputCallbacks();
}

void IntroScene::registerInputCallbacks() {
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        IntroScene::getInstance().keyboardCallback(key, x, y);
    });
    glutSpecialFunc(nullptr);
    glutMouseFunc(nullptr);
    glutMotionFunc(nullptr);
    glutMouseWheelFunc(nullptr);
}

void IntroScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Render Dialga
    glPushMatrix();
    glRotated(-40, 0.0, 1.0, 0.0);
    glTranslated(0.4, -1.0, 0.0);
    glScaled(0.01, 0.01, 0.01);
    dialga.render();
    glPopMatrix();

    // Render Pokemon logo
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pokemonLogoTexture);
    glColor3d(1.0, 1.0, 1.0);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    // Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTranslated(-1.15, 0.5, 0);

    double aspectRatio = 800.0 / 418.0;
    double width = 1.25;
    double height = width / aspectRatio;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-width / 2, -height / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(width / 2, -height / 2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(width / 2, height / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-width / 2, height / 2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    // Render text
    glPushMatrix();
    glColor3ub(255, 0, 0);
    glRasterPos2d(-1.25, -0.5);
    const unsigned char *text = reinterpret_cast<const unsigned char *>("Press the C Button");
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, text);
    glPopMatrix();

    glutSwapBuffers();
}

void IntroScene::update(double deltaTime) {
    dialga.update(deltaTime);
}

extern Scene *scene;
void IntroScene::keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
    case 13: // Enter key
    case 'c':
    case 'C':
        scene = &WorldScene::getInstance();
        scene->initialize();
        break;
    }
}