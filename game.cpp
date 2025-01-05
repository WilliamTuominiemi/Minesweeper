#include "SDL2/SDL.h"
#include <iostream>

using namespace std;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return true;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

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

        SDL_Event e;
        bool quit = false;

        while (!quit)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    quit = true;
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
            SDL_Rect rect = {50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100};
            SDL_RenderFillRect(renderer, &rect);

            SDL_RenderPresent(renderer);
        }
    }

    close();

    return 0;
}