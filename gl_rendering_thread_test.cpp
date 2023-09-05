#include <iostream>
#include <thread>
#include <gl/glew.h>
#include <SDL.h>
#include <mutex>

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

            while(!m_Running || m_ShouldAcceptGeometry) {
                Logger::info("Blocked");
                if (m_ShouldAcceptGeometry) {
                    m_Mutex.lock();

                    createGeometry();

                    m_ShouldAcceptGeometry = false;
                    m_Mutex.unlock();
                }
            }

            while (gameState != GameState::EXIT) {
                Logger::info("Render loop");
                glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);


                if (m_ShouldAcceptGeometry) {
                    m_Mutex.lock();
                    createGeometry();
                    m_ShouldAcceptGeometry = false;
                    m_Mutex.unlock();
                }

                Logger::info("m_VAO = " + std::to_string(m_VAO));
                Logger::info("ShaderID = " + std::to_string(m_Shader->getShaderProgramId()));
                if (m_VAO) {
                    glUseProgram(m_Shader->getShaderProgramId());
                    glBindVertexArray(m_VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }

                // swap buffers and draw everything on the screen
                SDL_GL_SwapWindow(window);
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

        m_Shader = new Shaders();
        try {
            m_Shader->compile("../shaders/test_vert.vs", "../shaders/test_frag.fs");
            m_Shader->link();
            m_Shader->loadTexture("../textures/box.png");
        }
        catch (const std::exception &e) {
            Logger::error(e.what());
            gameState = GameState::EXIT;
        }
    }

    void createGeometry() {
        if (!m_Geometry || !m_Indices) {
            Logger::error("RenderThread.createGeometry() : Geometry or Indices are null");
            return;
        }

        Logger::info("Creating buffers");

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

//        GLuint vbo;
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), m_Geometry, GL_STATIC_DRAW);

//        GLuint ibo;
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), m_Indices, GL_STATIC_DRAW);

        // =====================================================================================
        // Attributes
        GLuint posAttr = glGetAttribLocation(m_Shader->getShaderProgramId(), "vertexPosition");
        glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(posAttr);

        GLuint colAttr = glGetAttribLocation(m_Shader->getShaderProgramId(), "vertexColor");
        glVertexAttribPointer(colAttr, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)8);
        glEnableVertexAttribArray(colAttr);

        GLuint uvAttr = glGetAttribLocation(m_Shader->getShaderProgramId(), "vertexUV");
        glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
        glEnableVertexAttribArray(uvAttr);

        Logger::info("Buffers created");

        // --------------------------------------------------------------------------------
        // Send shader's texture to uniform
        glUseProgram(m_Shader->getShaderProgramId());
        glActiveTexture(GL_TEXTURE0);
        Logger::info("TextureID = " + std::to_string(m_Shader->getTextureID()));
        glBindTexture(GL_TEXTURE_2D, m_Shader->getTextureID());
        GLuint textureLocation = m_Shader->getUniformLocation("textureSampler");
        if (textureLocation == GL_INVALID_INDEX) {
            throw std::runtime_error("textureLocation uniform INV INDEX");
        }
        glUniform1i(textureLocation, 0);

        Logger::info("Shader uniform sent");
    }

    void addObject(Vertex* geometry, int* indices) {
        m_Mutex.lock();
        if (!geometry || !indices)
            return;

        m_ShouldAcceptGeometry = true;
        m_Geometry = geometry;
        m_Indices = indices;
        m_Mutex.unlock();
    }



protected:
    SDL_Window *m_Window;
    SDL_GLContext m_GlContext;
    std::thread m_Thread;
    std::exception_ptr m_ExceptionPtr;
    Shaders *m_Shader{nullptr};

    bool m_Running {false};
    bool m_ShouldAcceptGeometry {false};
    Vertex* m_Geometry {nullptr};
    int* m_Indices {nullptr};
    std::mutex m_Mutex;

    GLuint m_VAO {0};
    GLuint m_VBO {0};
    GLuint m_IBO {0};
};

std::shared_ptr<class Logger> Logger::m_Instance = nullptr;

void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
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


void createGeometry(Vertex* geometry, int* indices) {
    glm::vec2 m_Position = {0.0f, 0.0f};
    glm::vec2 m_WorldSize = {1.0f, 1.0f};

    // top left
    geometry[0].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    geometry[0].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    geometry[0].color.r = 0;
    geometry[0].color.g = 20;
    geometry[0].color.b = 0;
    geometry[0].uv.u = 0.0f;
    geometry[0].uv.v = 1.0f;

    // bottom left
    geometry[1].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    geometry[1].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    geometry[1].color.r = 255;
    geometry[1].color.g = 255;
    geometry[1].color.b = 255;
    geometry[1].uv.u = 0.0f;
    geometry[1].uv.v = 0.0f;

    // bottom right
    geometry[2].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    geometry[2].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    geometry[2].color.r = 255;
    geometry[2].color.g = 255;
    geometry[2].color.b = 255;
    geometry[2].uv.u = 1.0f;
    geometry[2].uv.v = 0.0f;

    // top right
    geometry[3].pos.x = m_Position.x + m_WorldSize.x / 2.0f + 0.1f;
    geometry[3].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    geometry[3].color.r = 255;
    geometry[3].color.g = 255;
    geometry[3].color.b = 255;
    geometry[3].uv.u = 1.0f;
    geometry[3].uv.v = 1.0f;

//    indices = new int[6];
    indices[0] = 3;
    indices[1] = 0;
    indices[2] = 1;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;
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


    RenderingThread renderingThread(window);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    renderingThread.startRenderingLoop();

    Vertex* geometry = new Vertex[4];
    int* indices = new int[6];
    createGeometry(geometry, indices);
    renderingThread.addObject(geometry, indices);

    while (gameState != GameState::EXIT) {
        handleInput();
    }

    return 0;
}
