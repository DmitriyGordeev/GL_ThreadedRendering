#ifndef COLLIDERGAME_ENGINECORE_H
#define COLLIDERGAME_ENGINECORE_H
#include <memory>
#include <ctime>
#include <vector>
#include <chrono>

#include <SDL.h>
#include <GLContext.h>
#include "InputSystem.h"
#include "Shaders.h"
#include "Camera.h"
#include "Object.h"
#include "Scene.h"
#include "RenderingThread.h"

enum class GameState {RUNNING, EXIT};

class EngineCore {
public:
    EngineCore();
    virtual ~EngineCore();

    void createWindow(int width, int height);

    /** updates InputSystem's state with processed SDL events */
    void updateInputSystem();

    /* utilizes InputSystem's state to take
     * action depending on currently pressed keys */
    void handleKeyBindings();

    /** Stores execution of the main game loop */
    void gameLoop();

    void renderFrame();

    [[nodiscard]] long getCurrentGameThreadFrame() const { m_GameThreadFrame; }

    [[nodiscard]] GameState getCurrentGameState() const { return m_GameState; }

protected:
    SDL_Window* m_Window;
    std::shared_ptr<RenderingThread> m_RenderThread;

    std::shared_ptr<InputSystem> m_InputSystem;
    long long m_LastFrameTimeMillis {0};
    GameState m_GameState {GameState::RUNNING};

    std::shared_ptr<Camera> m_Camera;
    std::vector<std::shared_ptr<Shaders>> m_Shaders;
    std::shared_ptr<Scene> m_Scene;

    long m_GameThreadFrame {0};
};


#endif //COLLIDERGAME_ENGINECORE_H
