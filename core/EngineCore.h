#ifndef COLLIDERGAME_ENGINECORE_H
#define COLLIDERGAME_ENGINECORE_H

#include <SDL.h>
#include <memory>

class EngineCore {
public:
    EngineCore();
    virtual ~EngineCore();

    void createWindow(int width, int height);

protected:
    SDL_Window* m_Window;

};


#endif //COLLIDERGAME_ENGINECORE_H
