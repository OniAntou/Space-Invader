#ifndef GAME_H
#define GAME_H
#include <vector>
#include <glm/glm.hpp>
#include "resourceManager.h"
#include "sprite_renderer.h"

struct Enemy {
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec2 Velocity;
    Texture Sprite;
    bool Destroyed;

    Enemy(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec2 velocity) 
        : Position(pos), Size(size), Sprite(sprite), Velocity(velocity), Destroyed(false) {}
};

struct Bullet {
    glm::vec2 Position;
    glm::vec2 Velocity;
    Texture Sprite;
    bool Active;

    Bullet(glm::vec2 pos, glm::vec2 vel, Texture sprite)
        : Position(pos), Velocity(vel), Sprite(sprite), Active(true) {}
};

enum GameState { MENU, PLAYING, GAME_OVER };

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Update(float deltaTime);
    void Render();
    void Reset(); // Hàm quan trọng để sửa lỗi Restart

    void HandleKeyPress(unsigned char key);
    void HandleKeyRelease(unsigned char key);

    GameState state;

private:
    SpriteRenderer* renderer;
    glm::vec2 playerPosition;
    glm::vec2 playerSize;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    bool keys[256]{};
};

#endif