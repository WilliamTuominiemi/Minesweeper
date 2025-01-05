#include "SDL2/SDL.h"
#include <iostream>

using namespace std;

SDL_Window *window = NULL;
SDL_Surface *windowSurface = NULL;

int SCREEN_WIDTH = 400;
int SCREEN_HEIGHT = 400;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            return false;
        }
        else
        {
            windowSurface = SDL_GetWindowSurface(window);
        }
    }

    return true;
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