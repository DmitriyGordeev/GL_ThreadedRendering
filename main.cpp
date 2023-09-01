#include <iostream>
#include <chrono>
#include <thread>

#include <glm.hpp>
#include <SDL.h>

#include <EngineCore.h>
#include "TextureLoader.h"

#undef main
using std::cout;
using std::endl;

std::shared_ptr<Logger> Logger::m_Instance = nullptr;

int main(int argc, char** argv) {

    EngineCore core;
    core.createWindow(640, 480);
    core.gameLoop();

//    GLuint id = TextureLoader::loadTexture("../textures/box.png");

    return 0;
}
