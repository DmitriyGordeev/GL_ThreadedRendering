#include <iostream>
#include <thread>
#include "GL/glew.h"
#include "SDL.h"
#include <mutex>
#include <deque>
#include <unordered_map>
#undef main

#include "Logger.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"
#include "RObject.h"
#include "InputSystem.h"

using std::cout;
using std::endl;

constexpr int FPS_MS = 1000 / 60;

enum class GameState {
    RUNNING,
    EXIT
};
GameState gameState = GameState::RUNNING;


int GameThreadFrameNumber = 0;


class RenderingThread {
public:
    explicit RenderingThread(SDL_Window *window) {
        if (!window) {
            Logger::error("Couldn't create window");
            throw std::runtime_error("Couldn't create window");
        }

        // &window - неправильно, потому что после выхода из конструктора
        // указатель window будет убран из стека, а тред будет на нее ссылаться
        // нужно делать капчур по значению (копировать)
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

            while(!m_Running) {
                Logger::info("Warmup Queue size = " + std::to_string(m_ObjectsQueue.size()));
                processObjectQueue();
            }

            // Rendering loop
            while (gameState != GameState::EXIT) {
                if (m_FrameRendered >= GameThreadFrameNumber)
                    continue;

                Logger::info("Render loop");
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Process added objects
                if (!m_ObjectsQueue.empty()) {
                    Logger::info("Queue size before processing = " + std::to_string(
                            m_ObjectsQueue.size()
                    ));

                    processObjectQueue();

                    Logger::info("Queue size after processing = " + std::to_string(
                            m_ObjectsQueue.size()
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

    ~RenderingThread() {
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

    RenderingThread(const RenderingThread &) = delete;

    void startRenderingLoop() {
        m_Running = true;
    }

    void prepareCanvas() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
    }

    void createShader() {
        if (m_Shaders)
            return;

        m_Shaders = std::make_shared<Shaders>("../../../shaders/test_vert.vs", "../../../shaders/test_frag.fs");
        try {
            m_Shaders->compile();
            m_Shaders->link();
            m_Shaders->loadTexture("../../../textures/box.png");
        }
        catch (const std::exception &e) {
            Logger::error(e.what());
            gameState = GameState::EXIT;
        }
    }

    void addObject(RObject* object) {
        m_Mutex.lock();
        m_ObjectsQueue.push_back(object);
        m_Mutex.unlock();
    }

    void processObjectQueue() {
        std::scoped_lock<std::mutex> scopedLock(m_Mutex);

        if (m_ObjectsQueue.empty())
            return;

        for(auto itr = m_ObjectsQueue.begin(); itr != m_ObjectsQueue.end(); ++itr) {
            RObject* obj = m_ObjectsQueue.front();
            if (!obj)
                continue;
            obj->buildBuffers();
            obj->applyShader(m_Shaders);
            m_ObjectsScene.push_back(obj);
        }
        m_ObjectsQueue.clear();
    }

    [[nodiscard]] long getRenderedFrame() const { return m_FrameRendered; }

protected:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;
    std::shared_ptr<Shaders> m_Shaders {nullptr};

    bool m_Running {false};
    std::mutex m_Mutex;

    std::vector<RObject*> m_ObjectsScene;
    std::deque<RObject*> m_ObjectsQueue;

    long m_FrameRendered {0};
};

std::shared_ptr<class Logger> Logger::m_Instance = nullptr;


void keyBinding(InputSystem& inputSystem, RObject& object) {
    if (inputSystem.isKeyPressed(SDLK_w)) {
        Logger::info("key W");
        object.move(glm::vec2(0.0f, 0.0001f * FPS_MS));
    }
}

void handleInput(InputSystem& inputSystem, RObject& object) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                inputSystem.pressKey(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                inputSystem.releaseKey(event.key.keysym.sym);
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        Logger::info("Quit pressed");
                        gameState = GameState::EXIT;
                        break;
                    default:
                        break;
                }
                break;

        }
    }

    keyBinding(inputSystem, object);
}


int main() {

    SDL_Init(SDL_INIT_EVERYTHING);
    Camera camera;

    SDL_Window *window = SDL_CreateWindow(
            "Test RenderingThread",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL);

    camera.init(640, 480);
    if (!window)
        Logger::info("Couldn't create window, SDL_CreateWindow() returned nullptr");

    InputSystem inputSystem;

    RObject object(glm::vec2(0, 0), glm::vec2(0.2f, 0.2f));

    RenderingThread renderingThread(window);
    renderingThread.addObject(&object);
    renderingThread.startRenderingLoop();

    while (gameState != GameState::EXIT) {
        Logger::info("Rendered frame = "
                    + std::to_string(renderingThread.getRenderedFrame()) +
                    " | Game thread frame = "
                    + std::to_string(GameThreadFrameNumber));

        handleInput(inputSystem, object);

        std::this_thread::sleep_for(std::chrono::milliseconds(FPS_MS));
        GameThreadFrameNumber++;
    }

    return 0;
}
