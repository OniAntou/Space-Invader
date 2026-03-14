#include <GL/glew.h> // Bắt buộc phải đặt dòng này TRƯỚC glut.h
#include <GL/glut.h>
#include "game.h"
#include <iostream>

Game* game;

void display() {
    game->Render();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    game->HandleKeyPress(key);
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    game->HandleKeyRelease(key);
}

void idle() {
    static float lastTime = 0.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    game->Update(deltaTime);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    // Projection is handled by shaders, no need for gluOrtho2D
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space Invader");

    // BẮT BUỘC PHẢI KHỞI TẠO GLEW Ở ĐÂY
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW Init Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Bật blending cho texture có alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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