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
//    SDL_Init(SDL_INIT_EVERYTHING);
//
//    int width = 640;
//    int height = 480;
//
//    SDL_Window* window = SDL_CreateWindow(
//            "MyPhysics",
//            SDL_WINDOWPOS_CENTERED,
//            SDL_WINDOWPOS_CENTERED,
//            width,
//            height,
//            SDL_WINDOW_OPENGL);
//
//    if (window == nullptr)
//        cout << "SDL window could not be created!" << endl;


//    EngineCore engine;
//    engine.createWindow(640, 480);

    auto t_start = std::chrono::system_clock::now().time_since_epoch().count();

    int a = 0;
    for (int i = 0; i < 10000; i++) {
        for(int j = 0; j < 10000; j++)
            a = i - j;
    }

    auto t_end =  std::chrono::system_clock::now().time_since_epoch().count();
    auto dt = (t_end - t_start) / 10000;


    return 0;
}
