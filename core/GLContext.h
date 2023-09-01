#ifndef COLLIDERGAME_GLCONTEXT_H
#define COLLIDERGAME_GLCONTEXT_H

#include <memory>
#include <SDL.h>
#include "Logger.h"

class GLContext {
public:
    static void createContext(SDL_Window* window);
    static void prepareCanvas();
};


#endif //COLLIDERGAME_GLCONTEXT_H
