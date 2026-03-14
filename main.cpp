#include <GL/glew.h>
#include <GL/glut.h>
#include "game.h"
#include <iostream>

Game* game;

void display() {
    // Luôn xóa buffer trước khi vẽ
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    game->Render();
    
    glutSwapBuffers();
}

void idle() {
    static float lastTime = 0.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    game->Update(deltaTime);
    glutPostRedisplay(); // Buộc vẽ lại liên tục
}

void reshape(int w, int h) {
    // Cập nhật khung hình khi người dùng thay đổi kích thước cửa sổ
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space Invader");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Lỗi khởi tạo GLEW!" << std::endl;
        return -1;
    }

    // Khởi tạo đối tượng game sau khi đã có Context OpenGL
    game = new Game();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    
    // Đăng ký nhận phím
    glutKeyboardFunc([](unsigned char k, int x, int y) { game->HandleKeyPress(k); });
    glutKeyboardUpFunc([](unsigned char k, int x, int y) { game->HandleKeyRelease(k); });

    glutMainLoop();
    return 0;
}