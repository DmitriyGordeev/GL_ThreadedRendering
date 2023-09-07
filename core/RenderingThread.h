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

#include "EngineCore.h"
#include "Object.h"
#include "Scene.h"


class EngineCore;

constexpr int FPS_MS = 1000 / 60;

class RenderingThread {
public:
    explicit RenderingThread(SDL_Window *window);
    RenderingThread(const RenderingThread &) = delete;
    virtual ~RenderingThread();

    void prepareCanvas();
    void startRenderingLoop();
    void stopRenderingLoop();

    /** Schedules shaders to be loaded and setup
     * inside rendering thread where GL context is living */
    std::shared_ptr<Shaders> addShader(
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath);

    void processShaderQueue();

    void addObject(const std::shared_ptr<Object>& object);
    void processQueue();
    void addObjectsFromScene(const std::shared_ptr<Scene>& scene);

protected:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;

    std::deque<std::shared_ptr<Shaders>> m_ShadersQueue;
    std::vector<std::shared_ptr<Shaders>> m_Shaders;

    bool m_Running {false};
    std::mutex m_Mutex;

    std::vector<std::shared_ptr<Object>> m_ObjectsScene;
    std::deque<std::shared_ptr<Object>> m_ObjectsQueue;

    long m_FrameRendered {0};
    bool m_ShouldStopRender {false};
    std::weak_ptr<EngineCore> m_EngineCoreWeakRef;
};


#endif //COLLIDERGAME_RENDERINGTHREAD_H
