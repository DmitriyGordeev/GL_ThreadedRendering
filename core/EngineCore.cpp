#include "EngineCore.h"
#include <iostream>
#include <GL/glew.h>
using std::cout;            // todo: logger -> file logger
using std::endl;

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
        cout << "Couldn't create window, SDL_CreateWindow() returned nullptr" << endl;


    // Create OpenGL context and verify we can use OpenGL
    SDL_GLContext glContext = SDL_GL_CreateContext(m_Window);
    if (glContext == nullptr)
        cout << "SDL GL context could not be created!" << endl;


    // Init Glew
    GLenum error = glewInit();
    if (error != GLEW_OK)
        cout << "Couldn't initialize glew" << endl;
}


