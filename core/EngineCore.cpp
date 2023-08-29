#include "EngineCore.h"
#include <GL/glew.h>
#include <chrono>
#include <sstream>
#include <thread>


EngineCore::EngineCore() {
    SDL_Init(SDL_INIT_EVERYTHING);
    m_InputSystem = std::make_shared<InputSystem>();
    m_Shaders = std::make_shared<Shaders>();
}

EngineCore::~EngineCore() {
    // TODO:
}


void EngineCore::createWindow(int width, int height) {
    m_Window = SDL_CreateWindow(
        "MyPhysics",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL);

    if (!m_Window)
        Logger::info("Couldn't create window, SDL_CreateWindow() returned nullptr");

    // try to create and prepare context
    try {
        GLContext::createContext(m_Window);
        GLContext::prepareCanvas();
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        return; // todo: выдать result code (int) или throw ?
    }

    // Create shaders
    try {
        // todo: ini файл со всеми путями? - передать в качестве аргумента?
        m_Shaders->compile("../shaders/shader.vs", "../shaders/shader.ps");
        m_Shaders->addAttribute("vertexPosition");
        m_Shaders->addAttribute("vertexColor");
        m_Shaders->addAttribute("vertexUV");
        m_Shaders->link();
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        m_GameStatus = GameState::EXIT;
    }
}


void EngineCore::updateInputSystem() {

    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                m_GameStatus = GameState::EXIT;
                break;

            case SDL_MOUSEMOTION:
                m_InputSystem->setMouseCoords((float)event.motion.x, (float)event.motion.y);
                break;

            case SDL_KEYDOWN:
                m_InputSystem->pressKey(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                m_InputSystem->releaseKey(event.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                m_InputSystem->pressKey(event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                m_InputSystem->releaseKey(event.button.button);
                break;

            // window close on 'x'
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        Logger::info("Quit pressed");
                        m_GameStatus = GameState::EXIT;
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    handleKeyBindings();
}


void EngineCore::handleKeyBindings() {
    if (m_InputSystem->isKeyPressed(SDLK_w)) {
        Logger::info("W pressed");
    }
}


void EngineCore::gameLoop() {
    m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    while(m_GameStatus != GameState::EXIT) {

        // Calculate time since the previous frame
        long long dt = (std::chrono::system_clock::now().time_since_epoch().count()
                - m_LastFrameTimeMillis) / 10000;


        updateInputSystem();

        // todo: если на сцене происходит много всего, то этот блок не имеет смысла
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

        m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    }
}


