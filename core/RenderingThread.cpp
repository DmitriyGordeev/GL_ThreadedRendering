#include "RenderingThread.h"
#include "Logger.h"


RenderingThread::RenderingThread(SDL_Window *window) {
    if (!window) {
        Logger::error("Couldn't create window");
        throw std::runtime_error("Couldn't create window");
    }

    m_Thread = std::thread([this, window] {

        // 1. Create GL context ----------------
        m_Window = window;
        m_GlContext = SDL_GL_CreateContext(window);
        try {
            if (m_GlContext == nullptr) {
                Logger::error("SDL_GL_CreateContext return nullptr");
                throw std::runtime_error("SDL_GL_CreateContext returned nullptr");
            }
        }
        catch (const std::exception &e) {
            m_ExceptionPtr = std::current_exception();
        }

        // 2. Init glew --------------------------
        GLenum error = glewInit();
        try {
            if (error != GLEW_OK) {
                Logger::error("error != GLEW_OK");
                throw std::runtime_error("error() != GLEW_OK");
            }
        }
        catch (const std::exception &e) {
            m_ExceptionPtr = std::current_exception();
        }

        Logger::info("Create shader");
        createShader();

        prepareCanvas();

        while(!m_Running) {}

        // Rendering loop
        while (!m_ShouldStopRender) {

            // if rendering thread is too fast in comparison to the game thread,
            // we should wait until the next game frame is ready to render
            if (auto gt = m_EngineCoreWeakRef.lock()) {
                if (m_FrameRendered >= gt->getCurrentGameThreadFrame())
                    continue;
            }

            Logger::info("Render loop");
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Process queue of added objects
            if (!m_ObjectsQueue.empty()) {
                Logger::info("Queue size before processing = " + std::to_string(
                        m_ObjectsScene.size()
                ));

                // todo: Нужен loop по всем необработанным объектам в очереди
                Object* obj = m_ObjectsQueue.front();
                obj->buildBuffers();
                obj->applyShader(m_Shader);
                obj->render();

                m_ObjectsScene.push_back(obj);  // add object to the scene's array
                m_ObjectsQueue.pop_front();     // remove object from queue after processing it

                Logger::info("Queue size after processing = " + std::to_string(
                        m_ObjectsScene.size()
                ));
            }

            for(auto& obj : m_ObjectsScene) {
                obj->updateBuffers();
                obj->render();
            }

            // swap buffers and draw everything on the screen
            SDL_GL_SwapWindow(window);

            std::this_thread::sleep_for(std::chrono::milliseconds(FPS_MS));
            m_FrameRendered++;
        }

        Logger::info("Rendering thread: QUIT");
    });

    Logger::info("After thread created");
}

RenderingThread::~RenderingThread() {
    if (m_Thread.joinable())
        m_Thread.join();

    try {
        if (m_ExceptionPtr)
            std::rethrow_exception(m_ExceptionPtr);
    }
    catch (const std::exception &e) {
        Logger::error(e.what());
    }
}

void RenderingThread::prepareCanvas() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void RenderingThread::startRenderingLoop() {
    m_Running = true;
}

void RenderingThread::stopRenderingLoop() { m_ShouldStopRender = true; }

// todo: несколько шейдеров
void RenderingThread::createShader() {
    if (m_Shader)
        return;

    m_Shader = std::make_shared<Shaders>();
    try {
        m_Shader->compile("../../../shaders/test_vert.vs", "../../../shaders/test_frag.fs");
        m_Shader->link();
        m_Shader->loadTexture("../../../textures/box.png");
    }
    catch (const std::exception &e) {
        Logger::error(e.what());
        m_ShouldStopRender = true;
    }
}

void RenderingThread::addObject(Object* object) {
    m_Mutex.lock();
    m_ObjectsQueue.push_back(object);
    m_Mutex.unlock();
}