#ifndef COLLIDERGAME_RENDERINGTHREAD_H
#define COLLIDERGAME_RENDERINGTHREAD_H

#include <iostream>
#include <thread>
#include "GL/glew.h"
#include "SDL.h"
#include <mutex>
#include <deque>
#include <unordered_map>
#undef main

#include "Object.h"
#include "EngineCore.h"

constexpr int FPS_MS = 1000 / 60;

class RenderingThread {
public:
    explicit RenderingThread(SDL_Window *window);
    RenderingThread(const RenderingThread &) = delete;
    virtual ~RenderingThread();

    void prepareCanvas();
    void startRenderingLoop();
    void stopRenderingLoop();
    void createShader();

    void addObject(Object* object);

protected:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;
    std::shared_ptr<Shaders> m_Shader {nullptr};

    bool m_Running {false};
    std::mutex m_Mutex;

    std::vector<Object*> m_ObjectsScene;
    std::deque<Object*> m_ObjectsQueue;

    long m_FrameRendered {0};
    bool m_ShouldStopRender {false};
    std::weak_ptr<EngineCore> m_EngineCoreWeakRef;
};


#endif //COLLIDERGAME_RENDERINGTHREAD_H
