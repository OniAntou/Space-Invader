#include "game.h"
#include <GL/glut.h>
#include <iostream>

Game::Game() : state(MENU) {
    // Initialize game data
    std::cout << "Game initialized" << std::endl;
}

Game::~Game() {
    // Cleanup
}

void Game::Update(float deltaTime) {
    switch (state) {
        case MENU:
            // Update menu logic
            break;
        case PLAYING:
            // Update game logic: move player, enemies, check collisions, etc.
            break;
        case GAME_OVER:
            // Update game over logic
            break;
    }
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    switch (state) {
        case MENU: {
            // Render menu
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(-0.5f, 0.0f);
            const char* menuText = "Press SPACE to start";
            for (const char* c = menuText; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
            break;
        }
        case PLAYING: {
            // Render game: player, enemies, bullets, etc.
            // For now, just a placeholder
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-0.1f, -0.1f);
            glVertex2f(0.1f, -0.1f);
            glVertex2f(0.1f, 0.1f);
            glVertex2f(-0.1f, 0.1f);
            glEnd();
            break;
        }
        case GAME_OVER: {
            // Render game over screen
            glColor3f(1.0f, 0.0f, 0.0f);
            glRasterPos2f(-0.5f, 0.0f);
            const char* gameOverText = "Game Over! Press R to restart";
            for (const char* c = gameOverText; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
            break;
        }
    }

    glutSwapBuffers();
}

void Game::HandleKeyPress(unsigned char key) {
    switch (key) {
        case ' ':
            if (state == MENU) {
                state = PLAYING;
            }
            break;
        case 'r':
        case 'R':
            if (state == GAME_OVER) {
                state = MENU;
            }
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

void Game::HandleKeyRelease(unsigned char key) {
    // Handle key releases if needed
}
