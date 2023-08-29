#include <iostream>
#include <chrono>
#include <thread>

#include <glm.hpp>
#include <SDL.h>

#include <EngineCore.h>

#undef main
using std::cout;
using std::endl;


int main(int argc, char** argv) {

    EngineCore core;
    core.createWindow(640, 480);
    core.gameLoop();


    return 0;
}
