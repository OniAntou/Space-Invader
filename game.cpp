#include "game.h"
#include <GL/glew.h>
#include <iostream>
#include <algorithm>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <GL/freeglut.h> // Để dùng glutBitmapCharacter

Game::Game() : state(MENU), spaceshipRenderer(nullptr), currentLevel(1) {
    Init();
}

Game::~Game() {
    delete spaceshipRenderer;
    ResourceManager::Clear();
}

void Game::Init() {
    // 1. Load Shaders
    ResourceManager::LoadShader("model.vs", "model.fs", "model");

    // 2. Setup Renderer
    spaceshipRenderer = new ModelRenderer(*ResourceManager::GetShader("model"));

    // 3. Khởi tạo thông số người chơi
    playerSize = glm::vec2(0.2f, 0.2f);
    playerPosition = glm::vec2(0.0f, -0.8f);
    
    // 5. Khởi tạo các ngôi sao
    srand((unsigned int)time(NULL));
    for (int y = 0; y < 100; ++y) {
        stars.push_back({
            glm::vec2((rand() % 200 - 100) / 100.0f, (rand() % 200 - 100) / 100.0f), // X, Y từ -1.0 đến 1.0
            (rand() % 3) + 1.0f, // Kích thước: 1-3
            (rand() % 50 + 10) / 1000.0f, // Tốc độ trôi
            (rand() % 100) / 100.0f // Độ sáng: 0.0 - 1.0
        });
    }

    InitLevel();
}

void Game::InitLevel() {
    enemies.clear();
    bullets.clear();

    // Làm quái vật nhỏ lại
    glm::vec2 enemySize(0.08f, 0.08f);
    
    // Tốc độ kẻ địch tỉ lệ thuận với Level
    glm::vec2 enemyVelocity(0.0f, -0.2f - (currentLevel * 0.05f)); 

    // Level 1: 3 con, Level 2: 5 con, Level 3: 7 con
    int numEnemies = 3 + (currentLevel - 1) * 2;

    for (int i = 0; i < numEnemies; ++i) {
        // Random X từ -0.9 đến 0.9
        float randX = ((float)(rand() % 180) / 100.0f) - 0.9f;
        // Random Y từ 0.5 đến 1.5 (một số con xuất hiện cao hơn màn hình để rơi xuống từ từ)
        float randY = ((float)(rand() % 100) / 100.0f) + 0.5f;

        enemies.push_back(Enemy(glm::vec2(randX, randY), enemySize, enemyVelocity));
    }
}

void Game::Reset() {
    state = PLAYING;
    playerPosition = glm::vec2(0.0f, -0.8f);
    currentLevel = 1;
    InitLevel();
}

void Game::Update(float deltaTime) {
    // Cập nhật ngôi sao nền (cho tất cả trạng thái để màn hình MENU cũng đẹp)
    for (auto &star : stars) {
        star.Position.y -= star.Speed * deltaTime;
        if (star.Position.y < -1.0f) {
            star.Position.y = 1.0f; // Vòng lại lên trên
            star.Position.x = ((float)(rand() % 200) / 100.0f) - 1.0f;
        }
    }

    if (state != PLAYING) return;

    // Di chuyển Player
    float playerSpeed = 1.0f;
    if (keys['a'] || keys['A']) playerPosition.x -= playerSpeed * deltaTime;
    if (keys['d'] || keys['D']) playerPosition.x += playerSpeed * deltaTime;
    playerPosition.x = std::clamp(playerPosition.x, -0.9f, 0.9f);

    // Cập nhật kẻ địch
    for (auto &enemy : enemies) {
        if (!enemy.Destroyed) {
            enemy.Position.y += enemy.Velocity.y * deltaTime;
            if (enemy.Position.y < -1.0f) state = GAME_OVER;
        }
    }

    // Cập nhật đạn và Va chạm
    for (auto &bullet : bullets) {
        if (!bullet.Active) continue;
        bullet.Position += bullet.Velocity * deltaTime;

        for (auto &enemy : enemies) {
            if (!enemy.Destroyed && 
                bullet.Position.x >= enemy.Position.x - enemy.Size.x/2 &&
                bullet.Position.x <= enemy.Position.x + enemy.Size.x/2 &&
                bullet.Position.y >= enemy.Position.y - enemy.Size.y/2 &&
                bullet.Position.y <= enemy.Position.y + enemy.Size.y/2) {
                
                enemy.Destroyed = true;
                bullet.Active = false;
            }
        }
    }

    // Xóa kẻ địch đã bị tiêu diệt
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return e.Destroyed; }), enemies.end());

    // Kiểm tra qua màn (Level up)
    if (enemies.empty()) {
        currentLevel++;
        if (currentLevel > MAX_LEVEL) {
            state = GAME_WON;
        } else {
            InitLevel();
        }
    }

    // Dọn dẹp đạn đã bay mất hoặc va chạm
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.Active || b.Position.y > 1.1f; }), bullets.end());
}

void Game::Render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Nền đen thui vũ trụ
    glClear(GL_COLOR_BUFFER_BIT);

    // Vẽ các ngôi sao trước tiên để làm nền
    glUseProgram(0); // Tắt shader để dùng fixed pipeline vẽ GL_POINTS
    glBegin(GL_POINTS);
    for (const auto &star : stars) {
        glColor3f(star.Brightness, star.Brightness, star.Brightness); // Đốm xám trắng
        // glPointSize(star.Size) chỉ hoạt động tốt ở ngoài glBegin/glEnd, 
        // ở mức cơ bản các điểm bằng nhau cũng ổn, hoặc ta bỏ qua size trong code đơn giản này.
        glVertex2f(star.Position.x, star.Position.y);
    }
    glEnd();

    // Helper in chữ 
    auto RenderText = [](float x, float y, const char* text) {
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    };

    auto RenderTextCentered = [](float y, const char* text) {
        int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text);
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        if (windowWidth == 0) windowWidth = 800; // prevent div by zero
        float x = -((float)textWidth / windowWidth);
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    };

    if (state == MENU) {
        glColor3f(1.0f, 1.0f, 1.0f); // Trắng
        RenderTextCentered(0.2f, "SPACE INVADER");
        RenderTextCentered(0.0f, "Press SPACE to start");
        glColor3f(1.0f, 0.0f, 0.0f); // Đỏ
        RenderTextCentered(-0.2f, "Quit: Esc");
    } 
    else if (state == GAME_OVER) {
        glColor3f(1.0f, 0.0f, 0.0f);
        RenderTextCentered(0.1f, "GAME OVER");
        glColor3f(1.0f, 1.0f, 1.0f);
        RenderTextCentered(-0.1f, "Press R to Restart");
    } 
    else if (state == GAME_WON) {
        glColor3f(0.0f, 1.0f, 0.0f);
        RenderTextCentered(0.1f, "YOU WIN!");
        glColor3f(1.0f, 1.0f, 1.0f);
        RenderTextCentered(-0.1f, "Press R to Play Again");
    }
    else if (state == PLAYING) {
        for (auto &b : bullets) spaceshipRenderer->DrawModel(ModelType::BULLET, b.Position, glm::vec2(0.05f, 0.1f), 0.0f, glm::vec3(1.0f, 1.0f, 0.0f)); // Đạn màu vàng
        for (auto &e : enemies) if (!e.Destroyed) spaceshipRenderer->DrawModel(ModelType::ENEMY, e.Position, e.Size, 0.0f, glm::vec3(1.0f, 0.2f, 0.2f)); // Quái vật màu đỏ
        spaceshipRenderer->DrawModel(ModelType::SPACESHIP, playerPosition, playerSize, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Máy bay màu xanh lá
        
        // Vẽ dòng chữ Level hiện tại ở góc
        glUseProgram(0);
        glColor3f(1.0f, 1.0f, 1.0f);
        char levelText[32];
        snprintf(levelText, sizeof(levelText), "Level: %d", currentLevel);
        RenderText(-0.95f, 0.9f, levelText);
    } 
}

void Game::HandleKeyPress(unsigned char key) {
    if (key < 256) keys[key] = true;
    if (key == ' ' && state == MENU) state = PLAYING;
    else if (key == ' ' && state == PLAYING) {
        bullets.emplace_back(playerPosition + glm::vec2(0.0f, 0.1f), glm::vec2(0.0f, 1.5f));
    }
    if ((key == 'r' || key == 'R') && (state == GAME_OVER || state == GAME_WON)) {
        Reset();
    }
    if (key == 27) exit(0);
}

void Game::HandleKeyRelease(unsigned char key) {
    if (key < 256) keys[key] = false;
}