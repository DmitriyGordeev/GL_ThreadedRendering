
#include <chrono>
#include <sstream>
#include <thread>

#include "EngineCore.h"
#include "Object.h"

EngineCore::EngineCore() {
    SDL_Init(SDL_INIT_EVERYTHING);
    m_InputSystem = std::make_shared<InputSystem>();
    m_Shaders = std::make_shared<Shaders>();
    m_Camera = std::make_shared<Camera>();
}

EngineCore::~EngineCore() {
    // TODO:
}


void EngineCore::createWindow(int width, int height) {
    m_Window = SDL_CreateWindow(
        "Collider",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL);

    if (!m_Window)
        Logger::info("Couldn't create window, SDL_CreateWindow() returned nullptr");

    // try to create and prepare context
    try {
        GLContext::createContext(m_Window);
//        GLContext::prepareCanvas();
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        return; // todo: выдать result code (int) или throw ?
    }


    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw std::runtime_error("could not initialize glew");


    // Create shaders
    try {
        // todo: ini файл со всеми путями? - передать в качестве аргумента?
        m_Shaders->compile("../shaders/shader.vs", "../shaders/shader.fs");
        m_Shaders->link();
    }
    catch(const std::exception& e) {
        Logger::error(e.what());
        m_GameStatus = GameState::EXIT;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //  Enable Alpha Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //  Anti-Aliasing:
    glEnable(GL_MULTISAMPLE);


    // TODO: prepare scene()

    m_Camera->init(width, height);
    m_Object = std::make_shared<Object>();
    m_Object->applyShader(m_Shaders->getShaderProgramId());


//    // Manual check -----------------------------------------------------
//    Vertex* m_Geometry = new Vertex[4];
//    glm::vec2 m_Position = {0.0f, 0.0f};
//    glm::vec2 m_WorldSize = {1.0f, 1.0f};
//
//    // top left
//    m_Geometry[0].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
//    m_Geometry[0].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
//    m_Geometry[0].color.r = 0;
//    m_Geometry[0].color.g = 20;
//    m_Geometry[0].color.b = 0;
//    m_Geometry[0].uv.u = 0.0f;
//    m_Geometry[0].uv.v = 1.0f;
//
//    // bottom left
//    m_Geometry[1].pos.x = m_Position.x - m_WorldSize.x / 2.0f;
//    m_Geometry[1].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
//    m_Geometry[1].color.r = 255;
//    m_Geometry[1].color.g = 255;
//    m_Geometry[1].color.b = 255;
//    m_Geometry[1].uv.u = 1.0f;
//    m_Geometry[1].uv.v = 1.0f;
//
//    // bottom right
//    m_Geometry[2].pos.x = m_Position.x + m_WorldSize.x / 2.0f;
//    m_Geometry[2].pos.y = m_Position.y - m_WorldSize.y / 2.0f;
//    m_Geometry[2].color.r = 255;
//    m_Geometry[2].color.g = 255;
//    m_Geometry[2].color.b = 255;
//    m_Geometry[2].uv.u = 1.0f;
//    m_Geometry[2].uv.v = 0.0f;
//
//    // top right
//    m_Geometry[3].pos.x = m_Position.x + m_WorldSize.x / 2.0f + 0.1f;
//    m_Geometry[3].pos.y = m_Position.y + m_WorldSize.y / 2.0f;
//    m_Geometry[3].color.r = 255;
//    m_Geometry[3].color.g = 255;
//    m_Geometry[3].color.b = 255;
//    m_Geometry[3].uv.u = 0.0f;
//    m_Geometry[3].uv.v = 0.0f;
//
//    int* indices = new int[6];
//    indices[0] = 3;
//    indices[1] = 0;
//    indices[2] = 1;
//    indices[3] = 1;
//    indices[4] = 2;
//    indices[5] = 3;
//
//    if (vao == 0) {
//        glGenVertexArrays(1, &vao);
//        glBindVertexArray(vao);
//    }
//    if(vbo == 0) {
//        glGenBuffers(1, &vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), m_Geometry, GL_STATIC_DRAW);
//    }
//    if(ibo == 0) {
//        glGenBuffers(1, &ibo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);
//    }
//
//
////    glBindVertexArray(0);
////    glBindBuffer(GL_ARRAY_BUFFER, 0);
////    glBindBuffer(GL_INDEX_ARRAY, 0);
//
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    GLuint shaderID = m_Shaders->getShaderProgramId();
//    glUseProgram(shaderID);
//
//    posAttr = glGetAttribLocation(shaderID, "vertexPosition");
//    glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//    glEnableVertexAttribArray(posAttr);
//
//    colAttr = glGetAttribLocation(shaderID, "vertexColor");
//    glVertexAttribPointer(colAttr, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)8);
//    glEnableVertexAttribArray(colAttr);
//
//    uvAttr = glGetAttribLocation(shaderID, "vertexUV");
//    glVertexAttribPointer(uvAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);
//    glEnableVertexAttribArray(uvAttr);

}


void EngineCore::updateInputSystem() {

    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                m_GameStatus = GameState::EXIT;
                break;

            case SDL_MOUSEMOTION:
                m_InputSystem->setMouseCoords((float)event.motion.x, (float)event.motion.y);
                break;

            case SDL_KEYDOWN:
                m_InputSystem->pressKey(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                m_InputSystem->releaseKey(event.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                m_InputSystem->pressKey(event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                m_InputSystem->releaseKey(event.button.button);
                break;

            // window close on 'x'
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        Logger::info("Quit pressed");
                        m_GameStatus = GameState::EXIT;
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    handleKeyBindings();
}


void EngineCore::handleKeyBindings() {
    if (m_InputSystem->isKeyPressed(SDLK_w)) {
        Logger::info("W pressed");
    }
}


void EngineCore::gameLoop() {
    m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    while(m_GameStatus != GameState::EXIT) {

        // Calculate time since the previous frame
        long long dt = (std::chrono::system_clock::now().time_since_epoch().count()
                - m_LastFrameTimeMillis) / 10000;

        updateInputSystem();
        m_Camera->update();
        renderFrame();

        // todo: если на сцене происходит много всего, то этот блок не имеет смысла
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

        // should be executed at the end of the loop
        m_LastFrameTimeMillis = std::chrono::system_clock::now().time_since_epoch().count();
    }
}


void EngineCore::renderFrame() {

    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Object->render(m_Shaders->getShaderProgramId());


//    glUseProgram(m_Shaders->getShaderProgramId());
//    glBindVertexArray(vao);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//
////    // enable shader attributes
////    glDisableVertexAttribArray(posAttr);
////    glDisableVertexAttribArray(colAttr);
////    glDisableVertexAttribArray(uvAttr);
//
//
////    // Update MVP matrix value of the shader's uniform
////    GLint pLocation = m_Shaders->getUniformLocation("P");
////    glm::mat4 cameraMatrix = m_Camera->getCameraMatrix();
////    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
//
////    m_Shaders->use();   // todo: сделать как в webgl ?
//////    glActiveTexture(GL_TEXTURE0);
////
//////    // Get texture variable from shaders
//////    GLint textureLocation = m_Shaders->getUniformLocation("textureSampler");
//////    glUniform1i(textureLocation, 0);
////
////    // Update MVP matrix value of the shader's uniform
////    GLint pLocation = m_Shaders->getUniformLocation("P");
////    glm::mat4 cameraMatrix = m_Camera->getCameraMatrix();
////    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
////
//////    // Draws all figures
//////    TODO: Actor class, у которого есть свои VBO, VAO - взять из webgl
//////    _objectsGroup.cleanUp();
//////    _scene->draw();
//////    _objectsGroup.post();
//////    _objectsGroup.renderGroup();
////
////
////    m_Object->applyShader(m_Shaders->getShaderProgramId());
////    m_Object->render(m_Shaders->getShaderProgramId());
////
////
//////    glBindTexture(GL_TEXTURE_2D, 0);
//////    m_Shaders->disable();   // todo: ?







    // swap buffers and draw everything on the screen
    SDL_GL_SwapWindow(m_Window);

}


