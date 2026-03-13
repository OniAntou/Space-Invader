#ifndef GAME_H
#define GAME_H

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();

    void Update(float deltaTime);
    void Render();

    GameState GetState() const { return state; }
    void SetState(GameState newState) { state = newState; }

    // Handle input
    void HandleKeyPress(unsigned char key);
    void HandleKeyRelease(unsigned char key);

private:
    GameState state;

    // Game data
    // Add player, enemies, etc. later
};

#endif // GAME_H