#include "game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

Game::Game() : state(MENU), renderer(nullptr), spaceshipRenderer(nullptr) {
    Init();
}

Game::~Game() {
    delete renderer;
    delete spaceshipRenderer;
    ResourceManager::Clear();
}

void Game::Init() {
    // 1. Load Shaders & Textures
    ResourceManager::LoadShader("sprite.vs", "sprite.fs", "sprite");
    ResourceManager::LoadShader("model.vs", "model.fs", "model");
    ResourceManager::LoadTexture("resource/enemy.png", true, "enemy");
    ResourceManager::LoadTexture("resource/ship.png", true, "ship");
    ResourceManager::LoadTexture("resource/bullet.png", true, "bullet");

    // 2. Setup Renderer
    renderer = new SpriteRenderer(*ResourceManager::GetShader("sprite"));
    spaceshipRenderer = new ModelRenderer(*ResourceManager::GetShader("model"));

    // 3. Bật Alpha Blending để sửa lỗi hiển thị ảnh PNG
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 4. Khởi tạo thông số người chơi
    playerSize = glm::vec2(0.2f, 0.2f);
    
    // 5. Khởi tạo các ngôi sao
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 100; ++i) {
        Star s;
        s.Position.x = ((float)(rand() % 200) / 100.0f) - 1.0f; // -1.0 to 1.0
        s.Position.y = ((float)(rand() % 200) / 100.0f) - 1.0f;
        s.Size = ((float)(rand() % 30) / 10.0f) + 1.0f;        // 1.0 to 4.0
        s.Speed = ((float)(rand() % 50) / 100.0f) + 0.1f;      // 0.1 to 0.6
        s.Brightness = ((float)(rand() % 50) / 100.0f) + 0.5f; // 0.5 to 1.0
        stars.push_back(s);
    }

    // 6. Gọi Reset để khởi tạo danh sách kẻ địch ban đầu
    Reset();
}

void Game::Reset() {
    // Xóa sạch dữ liệu cũ để tránh lỗi Restart
    this->enemies.clear();
    this->bullets.clear();
    this->state = PLAYING;
    this->playerPosition = glm::vec2(0.0f, -0.8f);

    // Khởi tạo lại kẻ địch
    glm::vec2 enemySize(0.2f, 0.2f);
    glm::vec2 enemyVelocity(0.0f, -0.3f);
    enemies.push_back(Enemy(glm::vec2(-0.6f, 0.8f), enemySize, ResourceManager::GetTexture("enemy"), enemyVelocity));
    enemies.push_back(Enemy(glm::vec2( 0.0f, 0.8f), enemySize, ResourceManager::GetTexture("enemy"), enemyVelocity));
    enemies.push_back(Enemy(glm::vec2( 0.6f, 0.8f), enemySize, ResourceManager::GetTexture("enemy"), enemyVelocity));
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

    if (state == MENU) {
        glUseProgram(0); // Switch to fixed pipeline for text rendering
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.2f, 0.0f);
        const char* t = "PRESS SPACE TO START";
        while (*t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *t++);
    } 
    else if (state == PLAYING) {
        for (auto &b : bullets) spaceshipRenderer->DrawModel(ModelType::BULLET, b.Position, glm::vec2(0.05f, 0.1f), 0.0f, glm::vec3(1.0f, 1.0f, 0.0f)); // Đạn màu vàng
        for (auto &e : enemies) if (!e.Destroyed) spaceshipRenderer->DrawModel(ModelType::ENEMY, e.Position, e.Size, 0.0f, glm::vec3(1.0f, 0.2f, 0.2f)); // Quái vật màu đỏ
        // renderer->DrawSprite(ResourceManager::GetTexture("ship"), playerPosition, playerSize);
        spaceshipRenderer->DrawModel(ModelType::SPACESHIP, playerPosition, playerSize, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Máy bay màu xanh lá
    } 
    else if (state == GAME_OVER) {
        glUseProgram(0); // Switch to fixed pipeline for text rendering
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.2f, 0.0f);
        const char* t = "GAME OVER! PRESS R TO RESTART";
        while (*t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *t++);
    }
}

void Game::HandleKeyPress(unsigned char key) {
    if (key < 256) keys[key] = true;
    if (key == ' ' && state == MENU) state = PLAYING;
    else if (key == ' ' && state == PLAYING) {
        bullets.emplace_back(playerPosition + glm::vec2(0.0f, 0.1f), glm::vec2(0.0f, 1.5f), ResourceManager::GetTexture("bullet"));
    }
    if ((key == 'r' || key == 'R') && state == GAME_OVER) {
        Reset();
    }
    if (key == 27) exit(0);
}

void Game::HandleKeyRelease(unsigned char key) {
    if (key < 256) keys[key] = false;
}