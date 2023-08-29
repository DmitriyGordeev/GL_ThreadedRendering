#include "EngineCore.h"
#include <GL/glew.h>
#include <chrono>


EngineCore::EngineCore() {
    SDL_Init(SDL_INIT_EVERYTHING);
}

EngineCore::~EngineCore() {
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
}


void EngineCore::mainLoop() {
    m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    while(true) {
        long long dt = std::chrono::system_clock::now().time_since_epoch().count()
                - m_LastFrameTimeMillis;

        // all work here


        m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    }
}


