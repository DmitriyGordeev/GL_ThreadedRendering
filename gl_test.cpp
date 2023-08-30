#include <iostream>
#include <gl/glew.h>
#include <SDL.h>
#undef main
#include "Logger.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"

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
    Shaders shaders;
    try {
        shaders.compile("../shaders/shader.vs", "../shaders/shader.fs");
        shaders.link();
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        gameState = GameState::EXIT;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_MULTISAMPLE);


    // =============================================================================================
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint posAttr = glGetAttribLocation(shaders.getShaderProgramId(), "vertexPosition");
    glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(posAttr);


//    glBindVertexArray(VAO);
//    GLuint shaderID = shaders.getShaderProgramId();
//    GLuint posAttr = glGetAttribLocation(shaderID, "vertexPosition");
//    glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
//    glEnableVertexAttribArray(posAttr);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(gameState != GameState::EXIT) {
        inputSystem();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaders.getShaderProgramId());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

//         glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

//        // enable shader attributes
//        glDisableVertexAttribArray(posAttr);
//        glDisableVertexAttribArray(colAttr);
//        glDisableVertexAttribArray(uvAttr);

        // swap buffers and draw everything on the screen
        SDL_GL_SwapWindow(window);
    }


    return 0;
}
