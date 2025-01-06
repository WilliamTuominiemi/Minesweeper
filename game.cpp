#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>
#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;
TTF_Font *font = NULL;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = SCREEN_WIDTH;

int numOfSquares = 8;
int spacing = SCREEN_WIDTH / numOfSquares;

int numOfMines = 10;

vector<pair<int, int>> mines;
vector<pair<int, int>> explored;
vector<pair<int, int>> flags;

bool gameOver = false;
bool gameWon = false;

pair<int, int> mineHit;

void clearBackground()
{
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
    SDL_RenderClear(renderer);
    SDL_RenderSetViewport(renderer, NULL);
    SDL_SetRenderTarget(renderer, NULL);
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "Failed to initialize SDL: %s\n"
             << SDL_GetError() << endl;
        return false;
    }

    if (TTF_Init() == -1)
    {
        cerr << "Failed to initialize SDL_ttf: %s\n"
             << TTF_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        cerr << "Failed to create window: %s\n"
             << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        cerr << "Failed to create renderer: %s\n"
             << SDL_GetError() << endl;
        return false;
    }

    clearBackground();

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
}

void generateMinePositions(pair<int, int> firstSquare)
{
    int placedMines = 0;

    while (placedMines < numOfMines)
    {
        int x = rand() % numOfSquares;
        int y = rand() % numOfSquares;
        pair<int, int> coord = make_pair(x, y);

        int cnt = count(mines.begin(), mines.end(), coord);

        if (cnt == 0 && coord != firstSquare)
        {
            placedMines++;
            mines.push_back(coord);
        }
    }
}

void reset()
{
    mines.clear();
    explored.clear();
    flags.clear();
    gameOver = false;
    gameWon = false;

    spacing = SCREEN_WIDTH / numOfSquares;

    clearBackground();
    drawGrid();
}

void drawMines()
{
    for (auto &mine : mines)
    {
        if (mineHit == mine)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect rect1 = {
                mine.first * spacing,
                mine.second * spacing,
                spacing,
                spacing};
            SDL_RenderFillRect(renderer, &rect1);
        }
        else if (gameWon)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect1 = {
                mine.first * spacing,
                mine.second * spacing,
                spacing,
                spacing};
            SDL_RenderFillRect(renderer, &rect1);
        }

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

void drawExplored()
{
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", spacing);
    if (font == NULL)
    {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
    }

    for (auto &square : explored)
    {
        int nearbyMines = neighbouringMines(square);
        SDL_Color textColor;

        SDL_SetRenderDrawColor(renderer, 137, 148, 153, 255);
        SDL_Rect rect = {
            square.first * spacing,
            square.second * spacing,
            spacing,
            spacing};
        SDL_RenderFillRect(renderer, &rect);

        if (nearbyMines == 0)
            continue;

        switch (nearbyMines)
        {
        case 1:
            textColor = {0, 0, 255, 255};
            break;
        case 2:
            textColor = {0, 128, 0, 255};
            break;
        case 3:
            textColor = {255, 0, 0, 255};
            break;
        case 4:
            textColor = {0, 0, 128, 255};
            break;
        case 5:
            textColor = {128, 0, 0, 255};
            break;
        case 6:
            textColor = {0, 128, 128, 255};
            break;
        case 7:
            textColor = {0, 0, 0, 255};
            break;
        case 8:
            textColor = {128, 128, 128, 255};
            break;
        default:
            textColor = {120, 6, 6, 255};
            break;
        }

        string text = to_string(nearbyMines);

        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (surface == NULL)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
            continue;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int textW = surface->w;
        int textH = surface->h;
        SDL_Rect textRect = {
            square.first * spacing + (spacing - textW) / 2,
            square.second * spacing + (spacing - textH) / 2,
            textW,
            textH};

        SDL_RenderCopy(renderer, texture, NULL, &textRect);

        SDL_DestroyTexture(texture);
    }
}

bool checkIfMineInSquare(pair<int, int> coord)
{
    for (auto &mine : mines)
    {
        if (mine == coord)
        {
            return true;
        }
    }
    return false;
}

void explore(pair<int, int> coord)
{
    if (find(explored.begin(), explored.end(), coord) != explored.end())
    {
        return;
    }

    if (checkIfMineInSquare(coord))
    {
        mineHit = coord;
        gameOver = true;
        return;
    }

    explored.push_back(coord);

    if (neighbouringMines(coord) == 0)
    {
        vector<pair<int, int>> neighbours = getNeighbours(coord);

        for (auto &neighbour : neighbours)
        {
            explore(neighbour);
        }
    }
}

void drawFlags()
{
    for (auto &square : flags)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {
            square.first * spacing + spacing / 4,
            square.second * spacing + spacing / 4,
            spacing / 2,
            spacing / 2};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void flag(pair<int, int> coord)
{
    auto it = find(flags.begin(), flags.end(), coord);
    if (it != flags.end())
    {
        flags.erase(it);

        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
        SDL_Rect rect = {
            coord.first * spacing,
            coord.second * spacing,
            spacing,
            spacing};
        SDL_RenderFillRect(renderer, &rect);
    }
    else if (count(explored.begin(), explored.end(), coord) == 0)
    {
        flags.push_back(coord);
    }
}

bool checkIfWon()
{
    if ((numOfSquares * numOfSquares) - numOfMines == explored.size())
    {
        gameWon = true;
    }
    return false;
}

void openDifficultyMenu()
{
    const SDL_MessageBoxButtonData buttons[] = {
        {0, 0, "Easy"},
        {0, 1, "Medium"},
        {0, 2, "Hard"}};

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL,
        "Select Difficulty",
        "Choose your difficulty level:",
        SDL_arraysize(buttons),
        buttons,
        NULL};

    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
    {
        cout << "Error displaying message box: " << SDL_GetError() << std::endl;
        return;
    }

    switch (buttonid)
    {
    case 0:
        numOfSquares = 8;
        numOfMines = 10;
        break;
    case 1:
        numOfSquares = 16;
        numOfMines = 40;
        break;
    case 2:
        numOfSquares = 25;
        numOfMines = 60;
        break;
    default:
        cout << "No selection made." << endl;
    }

    reset();
}

void drawEndText()
{
    const char *header;
    const char *text;

    if (gameOver)
    {
        header = "Game over";
        text = "You hit a mine. Click 'Ok' to try again...";
    }
    else
    {
        header = "You won";
        text = "You found all the mines. Click 'Ok' to try again...";
    }

    clearBackground();
    drawExplored();
    drawFlags();
    drawMines();
    drawGrid();
    SDL_RenderPresent(renderer);

    if (SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_INFORMATION,
            header,
            text,
            window) < 0)
    {
        std::cout << "Failed to display message box! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        reset();
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

        clearBackground();

        while (!quit)
        {
            while (SDL_PollEvent(&e))
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_MouseButtonEvent mouseEvent = e.button;

                    if (mouseEvent.button == SDL_BUTTON_LEFT && !gameOver && !gameWon)
                    {
                        if (explored.empty())
                        {
                            generateMinePositions(make_pair(mouseEvent.x / spacing, mouseEvent.y / spacing));
                        }
                        explore(make_pair(mouseEvent.x / spacing, mouseEvent.y / spacing));
                        if (checkIfWon())
                        {
                            gameWon = true;
                        }
                    }

                    if (mouseEvent.button == SDL_BUTTON_RIGHT && !gameOver && !gameWon)
                    {
                        flag(make_pair(mouseEvent.x / spacing, mouseEvent.y / spacing));
                    }

                    break;
                }

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        openDifficultyMenu();
                    }
                    break;
                }
            }

            clearBackground();
            drawExplored();
            drawFlags();
            if (gameOver || gameWon)
            {
                drawMines();
                drawEndText();
            }
            drawGrid();
            SDL_RenderPresent(renderer);
        }
    }

    close();

    return 0;
}