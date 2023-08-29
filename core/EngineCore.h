#ifndef COLLIDERGAME_ENGINECORE_H
#define COLLIDERGAME_ENGINECORE_H
#include <memory>
#include <ctime>
#include <chrono>

#include <SDL.h>
#include <GLContext.h>

class EngineCore {
public:
    EngineCore();
    virtual ~EngineCore();

    void createWindow(int width, int height);

    void mainLoop();

protected:
    SDL_Window* m_Window;
    long long m_LastFrameTimeMillis {0};
};


#endif //COLLIDERGAME_ENGINECORE_H
