#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = SCREEN_WIDTH;

const int numOfSquares = 8;
const int spacing = SCREEN_WIDTH / numOfSquares;

const int numOfMines = 10;

vector<pair<int, int>> mines;

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

    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
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

void drawGrid()
{
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 255);

    for (int x = 0; x < SCREEN_WIDTH / spacing; x++)
    {
        SDL_RenderDrawLine(renderer, x * spacing, 0, x * spacing, SCREEN_HEIGHT);
    }

    for (int y = 0; y < SCREEN_HEIGHT / spacing; y++)
    {
        SDL_RenderDrawLine(renderer, 0, y * spacing, SCREEN_WIDTH, y * spacing);
    }

    SDL_RenderPresent(renderer);
}

void generateMinePositions()
{
    int placedMines = 0;

    while (placedMines <= numOfMines)
    {
        int x = rand() % SCREEN_WIDTH / spacing;
        int y = rand() % SCREEN_HEIGHT / spacing;
        pair<int, int> coord = make_pair(x, y);

        int cnt = count(mines.begin(), mines.end(), coord);

        if (cnt == 0)
        {
            placedMines++;
            mines.push_back(coord);
        }
    }

    for (auto i : mines)
        cout << i.first << " " << i.second
             << endl;
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

        srand(time(0));

        SDL_Event e;
        bool quit = false;

        generateMinePositions();

        while (!quit)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    quit = true;
            }

            drawGrid();
        }
    }

    close();

    return 0;
}