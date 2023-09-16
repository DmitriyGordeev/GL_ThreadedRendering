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

        prepareCanvas();

        // warmup waiting cycle
        while(!m_Running) {
            processShaderQueue();

            // if objects have been added, but game thread hasn't ordered to start the game,
            // we can process them before rendering loop even starts
            processObjectQueue();

            Logger::info("warmup cycle");
        }

        // Rendering loop
        while (!m_ShouldStopRender) {

            // if rendering thread is too fast in comparison to the game thread,
            // we should wait until the next game frame is ready to render
            if (auto gt = m_EngineCoreWeakRef.lock()) {
                if (m_FrameRendered >= gt->getCurrentGameThreadFrame())
                    continue;
            }

            // Logger::info("Render loop");
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processShaderQueue();
            processObjectQueue();

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
    Logger::info("[RenderingThread] m_Running = true\n");
}

void RenderingThread::stopRenderingLoop() { m_ShouldStopRender = true; }

std::shared_ptr<Shaders> RenderingThread::addShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath,
        const std::string& texturePath) {

    std::shared_ptr<Shaders> shader(new Shaders(vertexShaderPath,
                                                fragmentShaderPath,
                                                texturePath));

    std::scoped_lock<std::mutex> scopedLock(m_Mutex);
    m_ShadersQueue.push_back(std::move(shader));
    return m_ShadersQueue.back();

//    std::shared_ptr<Shaders> shader(new Shaders());
//    shader->compile(vertexShaderPath, fragmentShaderPath);
//    shader->link();
//    m_Shaders.push_back(std::move(shader));
//    return m_Shaders.back();
}

void RenderingThread::processShaderQueue() {
    std::scoped_lock<std::mutex> scopedLock(m_Mutex);

    if (m_ShadersQueue.empty())
        return;

    Logger::info("unprocessed shaders queue size = " + std::to_string(m_ShadersQueue.size()));

    for(auto itr = m_ShadersQueue.begin(); itr != m_ShadersQueue.end(); ++itr) {
        (*itr)->compile();
        (*itr)->link();
        (*itr)->loadTexture();
        (*itr)->m_ShaderLoadedPromise.set_value(true);
        Logger::info("[processShaderQueue()] Shader main part compiled and linked");
        m_Shaders.push_back(*itr);
    }

    m_ShadersQueue.clear();
}

void RenderingThread::addObject(const std::shared_ptr<Object>& object) {
    m_Mutex.lock();
    m_ObjectsQueue.push_back(object);
    Logger::info("[RenderingThread::addObject()] object added to queue of RT, queue size = " + std::to_string(m_ObjectsQueue.size()));
    m_Mutex.unlock();
}

void RenderingThread::processObjectQueue() {
    std::scoped_lock<std::mutex> scopedLock(m_Mutex);

    if (m_ObjectsQueue.empty())
        return;

    Logger::info("unprocessed object queue size = " + std::to_string(m_ObjectsQueue.size()));

    for(auto itr = m_ObjectsQueue.begin(); itr != m_ObjectsQueue.end(); ++itr) {
        auto obj = m_ObjectsQueue.front();
        if (!obj)
            continue;
        obj->buildBuffers();
        obj->confirmShader();
        m_ObjectsScene.push_back(obj);
    }
    m_ObjectsQueue.clear();
}

void RenderingThread::addObjectsFromScene(const std::shared_ptr<Scene>& scene) {
    if (!scene) {
        Logger::error("[RenderingThread::addObjectsFromScene] Scene is null");
        return;
    }

    auto sceneObjects = scene->getObjectsMap();
    for(auto itr = sceneObjects.begin(); itr != sceneObjects.end(); ++itr)
        addObject(itr->second);
}