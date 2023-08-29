#ifndef COLLIDERGAME_ENGINECORE_H
#define COLLIDERGAME_ENGINECORE_H
#include <memory>
#include <ctime>
#include <chrono>

#include <SDL.h>
#include <GLContext.h>
#include "InputSystem.h"

enum class GameState {RUNNING, EXIT};

class EngineCore {
public:
    EngineCore();
    virtual ~EngineCore();

    void createWindow(int width, int height);

    /* updates InputSystem's state with processed SDL events */
    void updateInputSystem();

    /* utilizes InputSystem's state to take
     * action depending on currently pressed keys */
    void handleKeyBindings();

    /* Stores execution of the main game loop */
    void gameLoop();

protected:
    SDL_Window* m_Window;
    std::shared_ptr<InputSystem> m_InputSystem;
    long long m_LastFrameTimeMillis {0};
    GameState m_GameStatus {GameState::RUNNING};
};


#endif //COLLIDERGAME_ENGINECORE_H
