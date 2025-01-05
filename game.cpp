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
vector<pair<int, int>> explored;

void clearBackground()
{
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
    SDL_RenderClear(renderer);
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        return false;
    }

    clearBackground();

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
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

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

    while (placedMines < numOfMines)
    {
        int x = rand() % numOfSquares;
        int y = rand() % numOfSquares;
        pair<int, int> coord = make_pair(x, y);

        int cnt = count(mines.begin(), mines.end(), coord);

        if (cnt == 0)
        {
            placedMines++;
            mines.push_back(coord);
        }
    }
}

void drawMines()
{
    for (auto &mine : mines)
    {
        SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
        SDL_Rect rect = {
            mine.first * spacing + spacing / 4,
            mine.second * spacing + spacing / 4,
            spacing / 2,
            spacing / 2};
        SDL_RenderFillRect(renderer, &rect);
    }
}

vector<pair<int, int>> getNeighbours(pair<int, int> coord)
{
    vector<pair<int, int>> neighbours;
    vector<pair<int, int>> offsets = {
        {-1, -1},
        {0, -1},
        {1, -1},
        {-1, 0},
        {1, 0},
        {-1, 1},
        {0, 1},
        {1, 1}};

    for (auto &offset : offsets)
    {
        int nX = coord.first + offset.first;
        int nY = coord.second + offset.second;

        if (nX >= 0 && nX < numOfSquares && nY >= 0 && nY < numOfSquares)
        {
            neighbours.push_back(make_pair(nX, nY));
        }
    }

    return neighbours;
}

int neighbouringMines(pair<int, int> coord)
{
    vector<pair<int, int>> neighbours = getNeighbours(coord);

    int mineCount = 0;

    for (auto &neighbour : neighbours)
    {
        for (auto &mine : mines)
        {
            if (neighbour == mine)
            {
                mineCount++;
            }
        }
    }

    return mineCount;
}

void drawExlored()
{
    for (auto &square : explored)
    {
        int nearbyMines = neighbouringMines(square);

        switch (nearbyMines)
        {
        case 0:
            SDL_SetRenderDrawColor(renderer, 137, 148, 153, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
            break;
        case 3:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case 4:
            SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
            break;
        case 5:
            SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
            break;
        case 6:
            SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
            break;
        case 7:
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            break;
        case 8:
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 120, 6, 6, 255);
            break;
        }

        SDL_Rect rect = {
            square.first * spacing,
            square.second * spacing,
            spacing,
            spacing};
        SDL_RenderFillRect(renderer, &rect);
    }
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

        clearBackground();

        while (!quit)
        {
            while (SDL_PollEvent(&e))
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    quit = true;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_MouseButtonEvent mouseEvent = e.button;

                    if (mouseEvent.button == SDL_BUTTON_LEFT)
                    {
                        explored.push_back(make_pair(mouseEvent.x / spacing, mouseEvent.y / spacing));
                    }
                }
            }

            drawGrid();
            drawMines();
            drawExlored();
        }
    }

    close();

    return 0;
}