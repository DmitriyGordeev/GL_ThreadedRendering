#ifndef COLLIDERGAME_ENGINECORE_H
#define COLLIDERGAME_ENGINECORE_H
#include <memory>

#include <SDL.h>
#include <GLContext.h>

class EngineCore {
public:
    EngineCore();
    virtual ~EngineCore();

    void createWindow(int width, int height);

protected:
    SDL_Window* m_Window;


};


#endif //COLLIDERGAME_ENGINECORE_H
