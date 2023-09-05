#include <iostream>
#include <thread>
#include <gl/glew.h>
#include <SDL.h>
#undef main
#include "Logger.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"
#include "TextureLoader.h"

using std::cout;
using std::endl;


enum class GameState {
    RUNNING,
    EXIT
};
GameState gameState = GameState::RUNNING;


class RenderingThread {
public:
    RenderingThread(SDL_Window* window) {
        if (!window) {
//            Logger::error("[GLContext] Couldn't create window");
//            return;
            throw std::runtime_error("Couldn't create window");
        }

        m_Thread = std::thread([this, &window]{

            // 1. Create GL context ----------------
            m_Window = window;
            m_GlContext = SDL_GL_CreateContext(window);
            try {
                if (m_GlContext == nullptr)
                    throw std::runtime_error("SDL_GL_CreateContext returned nullptr");
            }
            catch(const std::exception& e) {
                m_ExceptionPtr = std::current_exception();
            }

            // 2. Init glew --------------------------
            GLenum error = glewInit();
            try {
                if (error != GLEW_OK)
                    throw std::runtime_error("glewInit() != GLEW_OK");
            }
            catch(const std::exception& e) {
                m_ExceptionPtr = std::current_exception();
            }

            prepareCanvas();

            // TODO: SDL poll event
            // TODO: Geometry - VBO, VAO ? - один раз вначале

            while(gameState != GameState::EXIT) {
                Logger::info("rendering loop");
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);




                // swap buffers and draw everything on the screen
                SDL_GL_SwapWindow(window);
            }

            Logger::info("Rendering thread: QUIT");
        });
    }

    ~RenderingThread() {
        if (m_Thread.joinable())
            m_Thread.join();

        try {
            if (m_ExceptionPtr)
                std::rethrow_exception(m_ExceptionPtr);
        }
        catch(const std::exception& e) {
            Logger::error(e.what());
        }
    }

    RenderingThread(const RenderingThread&) = delete;

    void prepareCanvas() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
    }


protected:
    SDL_Window* m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;
};

std::shared_ptr<class Logger> Logger::m_Instance = nullptr;


//class Object {
//public:
//    Object(int a) {
//        m_thread = std::thread([this, &a]{
//            try {
//                if (a == 0)
//                    throw std::runtime_error("a == 0");
//                std::this_thread::sleep_for(std::chrono::seconds(2));
//            }
//            catch(std::exception e) {
//                m_ep = std::current_exception();
//            }
//        });
//    }
//
//    ~Object() {
//        m_thread.join();
//        cout << "Joining thread" << endl;
//        try
//        {
//            rethrow_exception(m_ep);
//        }
//        catch(const std::exception& e)
//        {
//            cout << e.what() << endl;
//        }
//    }
//
//protected:
//    std::thread m_thread;
//    std::exception_ptr m_ep;
//};


void handleInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
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
}


int main() {

    SDL_Init(SDL_INIT_EVERYTHING);
    Camera camera;

    SDL_Window* window = SDL_CreateWindow(
            "Test RenderingThread",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL);

    camera.init(640, 480);
    if (!window)
        Logger::info("Couldn't create window, SDL_CreateWindow() returned nullptr");

    RenderingThread renderingThread(window);



    while(gameState != GameState::EXIT) {
        handleInput();
    }




    return 0;
}
