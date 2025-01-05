#include "SDL2/SDL.h"
#include <iostream>

using namespace std;

SDL_Window *window = NULL;
SDL_Surface *windowSurface = NULL;
SDL_Surface *background = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

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

bool loadMedia()
{
    background = SDL_LoadBMP("background.bmp");
    if (background == NULL)
    {
        return false;
    }
    return true;
}

void close()
{
    SDL_FreeSurface(background);
    background = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
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

        if (!loadMedia())
        {
            cout << "Failed to load media. " << endl;
        }
        else
        {
            cout << "Media loaded. " << endl;
            SDL_BlitSurface(background, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(window);
            SDL_Event e;
            bool quit = false;
            while (quit == false)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
            }
        }
    }

    close();

    return 0;
}