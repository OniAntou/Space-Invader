#ifndef GAME_H
#define GAME_H
#include <vector>
#include <glm/glm.hpp>
#include "resourceManager.h"
#include "model_renderer.h"

struct Enemy {
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec2 Velocity;
    bool Destroyed;

    Enemy(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity) 
        : Position(pos), Size(size), Velocity(velocity), Destroyed(false) {}
};

struct Bullet {
    glm::vec2 Position;
    glm::vec2 Velocity;
    bool Active;

    Bullet(glm::vec2 pos, glm::vec2 vel)
        : Position(pos), Velocity(vel), Active(true) {}
};

struct Star {
    glm::vec2 Position;
    float Size;
    float Speed;
    float Brightness;
};

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    GAME_WON
};

class Game {
public:
    Game();
    ~Game();

    void Init();
    void InitLevel(); // Thêm hàm khởi tạo theo màn chơi
    void Reset();
    void HandleKeyPress(unsigned char key);
    void HandleKeyRelease(unsigned char key);
    void Update(float deltaTime);
    void Render();

    GameState state;

private:
    ModelRenderer* spaceshipRenderer;
    glm::vec2 playerPosition;
    glm::vec2 playerSize;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Star> stars;
    bool keys[256]{};
    
    int currentLevel;
    const int MAX_LEVEL = 3;
};

#endif