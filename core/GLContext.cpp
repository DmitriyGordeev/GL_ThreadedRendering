#include "GLContext.h"
#include <GL/glew.h>
#include <stdexcept>


void GLContext::createContext(SDL_Window* window) {
    if (!window) {
        throw std::runtime_error("[GLContext] Couldn't create window");
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        throw std::runtime_error("[GLContext] SDL: GL context could not be created!");
    }

    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw std::runtime_error("[GLContext] Couldn't initialize glew");
}

void GLContext::prepareCanvas() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable Alpha Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Anti-Aliasing:
    glEnable(GL_MULTISAMPLE);
}
