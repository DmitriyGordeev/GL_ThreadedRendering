#include "EngineCore.h"
#include <GL/glew.h>


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
        return;
    }

}


