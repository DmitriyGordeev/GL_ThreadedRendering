#include <iostream>
#include <gl/glew.h>
#include <SDL.h>
#undef main
#include "Logger.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"
#include "TextureLoader.h"

enum class GameState {
    RUNNING,
    EXIT
};


GameState gameState = GameState::RUNNING;


void inputSystem() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                gameState = GameState::EXIT;
                break;

                // window close on 'x'
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:   // exit game
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

std::shared_ptr<class Logger> Logger::m_Instance = nullptr;

int main() {

    SDL_Init(SDL_INIT_EVERYTHING);
    Camera camera;

    SDL_Window* window = SDL_CreateWindow(
            "TestGL",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL);

    camera.init(640, 480);

    if (!window)
        Logger::info("Couldn't create window, SDL_CreateWindow() returned nullptr");


    if (!window) {
        throw std::runtime_error("[GLContext] Couldn't create window");
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        throw std::runtime_error("[GLContext] SDL: GL context could not be created!");
    }

    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw std::runtime_error("[GLContext] Couldn't initialize glew");

    // Create shaders
    Shaders shader;
    GLuint textureID;
    try {
        shader.compile("../shaders/test_vert.vs", "../shaders/test_frag.fs");
        shader.link();
        textureID = shader.loadTexture("../textures/box.png");
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        gameState = GameState::EXIT;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);



    // Geometry =====================================================
    Vertex* m_Geometry = new Vertex[4];
    glm::vec2 m_Position = {0.0f, 0.0f};
    glm::vec2 m_WorldSize = {1.0f, 1.0f};

    // top left
    m_Geometry[0].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    m_Geometry[0].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    m_Geometry[0].color.r = 0;
    m_Geometry[0].color.g = 20;
    m_Geometry[0].color.b = 0;
    m_Geometry[0].uv.u = 0.0f;
    m_Geometry[0].uv.v = 1.0f;

    // bottom left
    m_Geometry[1].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
    m_Geometry[1].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[1].color.r = 255;
    m_Geometry[1].color.g = 255;
    m_Geometry[1].color.b = 255;
    m_Geometry[1].uv.u = 0.0f;
    m_Geometry[1].uv.v = 0.0f;

    // bottom right
    m_Geometry[2].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
    m_Geometry[2].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
    m_Geometry[2].color.r = 255;
    m_Geometry[2].color.g = 255;
    m_Geometry[2].color.b = 255;
    m_Geometry[2].uv.u = 1.0f;
    m_Geometry[2].uv.v = 0.0f;

    // top right
    m_Geometry[3].pos.x = m_Position.x + m_WorldSize.x / 2.0f + 0.1f;
    m_Geometry[3].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
    m_Geometry[3].color.r = 255;
    m_Geometry[3].color.g = 255;
    m_Geometry[3].color.b = 255;
    m_Geometry[3].uv.u = 1.0f;
    m_Geometry[3].uv.v = 1.0f;

    int* indices = new int[6];
    indices[0] = 3;
    indices[1] = 0;
    indices[2] = 1;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), m_Geometry, GL_STATIC_DRAW);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);

    // =====================================================================================
    // Attributes
    GLuint posAttr = glGetAttribLocation(shader.getShaderProgramId(), "vertexPosition");
    glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(posAttr);

    GLuint colAttr = glGetAttribLocation(shader.getShaderProgramId(), "vertexColor");
    glVertexAttribPointer(colAttr, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)8);
    glEnableVertexAttribArray(colAttr);

    GLuint uvAttr = glGetAttribLocation(shader.getShaderProgramId(), "vertexUV");
    glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
    glEnableVertexAttribArray(uvAttr);

    // =====================================================================================
    // Send texture to Uniform
    glUseProgram(shader.getShaderProgramId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLuint textureLocation = shader.getUniformLocation("textureSampler");
    if (textureLocation == GL_INVALID_INDEX) {
        throw std::runtime_error("textureLocation uniform INV INDEX");
    }
    glUniform1i(textureLocation, 0);


    // =====================================================================================
    // Render loop
    while(gameState != GameState::EXIT) {
        inputSystem();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.getShaderProgramId());

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // swap buffers and draw everything on the screen
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
