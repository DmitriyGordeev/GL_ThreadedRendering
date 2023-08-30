
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
        GLContext::prepareCanvas();
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

//    //  Enable Alpha Blend
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //  Anti-Aliasing:
    glEnable(GL_MULTISAMPLE);




    // TODO: prepare scene()
    m_Camera->init(width, height);
    m_Object = std::make_shared<Object>();
    m_Object->applyShader(m_Shaders->getShaderProgramId());
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


    // Update MVP matrix value of the shader's uniform
    GLint pLocation = m_Shaders->getUniformLocation("P");
    glm::mat4 cameraMatrix = m_Camera->getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));


    m_Object->render(m_Shaders->getShaderProgramId());



//    m_Shaders->use();   // todo: сделать как в webgl ?
////    glActiveTexture(GL_TEXTURE0);
//
////    // Get texture variable from shaders
////    GLint textureLocation = m_Shaders->getUniformLocation("textureSampler");
////    glUniform1i(textureLocation, 0);
//
//    // Update MVP matrix value of the shader's uniform
//    GLint pLocation = m_Shaders->getUniformLocation("P");
//    glm::mat4 cameraMatrix = m_Camera->getCameraMatrix();
//    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
//
////    // Draws all figures
////    TODO: Actor class, у которого есть свои VBO, VAO - взять из webgl
////    _objectsGroup.cleanUp();
////    _scene->draw();
////    _objectsGroup.post();
////    _objectsGroup.renderGroup();
//
//
//    m_Object->applyShader(m_Shaders->getShaderProgramId());
//    m_Object->render(m_Shaders->getShaderProgramId());
//
//
////    glBindTexture(GL_TEXTURE_2D, 0);
////    m_Shaders->disable();   // todo: ?

    // swap buffers and draw everything on the screen
    SDL_GL_SwapWindow(m_Window);

}


