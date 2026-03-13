#include <GL/glut.h>
#include "game.h"
#include <iostream>

Game* game;

void display() {
    game->Render();
}

void keyboard(unsigned char key, int x, int y) {
    game->HandleKeyPress(key);
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    game->HandleKeyRelease(key);
}

void idle() {
    // Update game logic
    static float lastTime = 0.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    game->Update(deltaTime);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space Invader");

    game = new Game();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();

    delete game;
    return 0;
}    