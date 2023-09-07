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

            while(!m_Running) {Logger::info("Blocked");}

            // Rendering loop
            while (gameState != GameState::EXIT) {
                if (m_FrameRendered >= GameThreadFrameNumber)
                    continue;

                Logger::info("Render loop");
                glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Process added objects
                if (!m_ObjectsQueue.empty()) {
                    Logger::info("Queue size before processing = " + std::to_string(
                            m_ObjectsScene.size()
                    ));

                    RObject* obj = m_ObjectsQueue.front();
                    obj->buildBuffers();
                    obj->applyShader(m_Shader);
                    obj->render();
                    m_ObjectsScene.push_back(obj);
                    m_ObjectsQueue.pop_front();

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
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
    }

    void createShader() {
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
            gameState = GameState::EXIT;
        }
    }

    void addObject(RObject* object) {
        m_Mutex.lock();
        m_ObjectsQueue.push_back(object);
        m_Mutex.unlock();
    }

    [[nodiscard]] long getRenderedFrame() const { return m_FrameRendered; }

protected:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;
    std::shared_ptr<Shaders> m_Shader {nullptr};

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

    RObject object(glm::vec2(0, 0), glm::vec2(0.05f, 0.05f));

    RenderingThread renderingThread(window);
    renderingThread.startRenderingLoop();
    renderingThread.addObject(&object);

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
