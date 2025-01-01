#include "Scene.h"
#include "IntroScene.h"
#include "WorldScene.h"
#include "BattleScene.h"
#include "freeglut.h"
#include "glig.h"
/* Texture loading library */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
/* Audio library */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


constexpr int REFRESH_RATE{144};
double lastFrameTime{0.0}, deltaTime{0.0};

constexpr int WINDOW_WIDTH{900};
constexpr int WINDOW_HEIGHT{900};
const char WINDOW_TITLE[]{"SGI Project"};

Scene *scene{&IntroScene::getInstance()};

void display() {
    scene->render();
}

void reshape(int width, int height) {
    // Set up the viewport to match the new window size.
    // The viewport defines the area of the window where OpenGL will render.
    // (0, 0) means the bottom-left corner of the window.
    glViewport(0, 0, width, height);

    // Switch to the projection matrix mode.
    // The projection matrix defines how we project 3D coordinates onto a 2D screen.
    // The projection matrix encodes how much of the scene is captured in a render by defining the
    // extents of the camera's view
    glMatrixMode(GL_PROJECTION);

    // Reset the projection matrix to the identity matrix.
    // This clears any previous settings, ensuring a fresh start.
    glLoadIdentity();

    // Set up an orthographic projection matrix.
    // This means objects are rendered without perspective distortion.
    // The parameters define the visible area in the 3D space
    const GLfloat aspect = static_cast<GLfloat>(height) / static_cast<GLfloat>(width);
    glOrtho(-2.0, 2.0, -2.0 * aspect, 2.0 * aspect, -8.0, 8.0);
}

void createWindow(int argc, char **argv) {
    // Configure GLUT library settings
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    // The GLUT_DEPTH flag enables the depth buffer (also known as the z-buffer), which is used to
    // keep track of the depth of each pixel in the scene. Enabling it allows OpenGL to determine
    // which objects are in front of others.

    // Set window size and create the window
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);
    // Register callback functions for display and reshape
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // Set default display values
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3ub(255, 255, 255);
}

void timer(int) {
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    scene->update(deltaTime);

    glutPostRedisplay(); // Request to redraw the window
    glutTimerFunc(1000 / REFRESH_RATE, timer, 0);
}

// argc: argument count, argv: argument vector
int main(int argc, char **argv) {
    createWindow(argc, argv);
    scene->initialize();
    // Request to redraw the window at a fixed rate
    glutTimerFunc(1000 / REFRESH_RATE, timer, 0);
    glutMainLoop();

    return 0;
}
