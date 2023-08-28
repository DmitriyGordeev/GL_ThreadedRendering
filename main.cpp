#include <iostream>
//#include <glm.hpp>
#include <SDL.h>
#undef main
using std::cout;
using std::endl;


int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    int width = 640;
    int height = 480;

    SDL_Window* window = SDL_CreateWindow(
            "MyPhysics",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL);

    if (window == nullptr)
        cout << "SDL window could not be created!" << endl;

    return 0;
}
