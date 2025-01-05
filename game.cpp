#include "SDL2/SDL.h"
#include <iostream>

using namespace std;

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        success = false;
    }

    return success;
}

int main(int argc, char *args[])
{
    if (!init())
    {
        cout << "Failed to initialize. " << endl;
    }
    else
    {
        cout << "Initialized. " << endl;
    }
    return 0;
}