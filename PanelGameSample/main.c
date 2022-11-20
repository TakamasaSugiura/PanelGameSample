#include "all.h"
#include "setting.h"
#include "game.h"

void InitAll()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void QuitAll()
{
    IMG_Quit();
    SDL_Quit();
}

int SDL_main(int argc, char* argv[])
{
    InitAll();
    SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    game_main(window);
    SDL_DestroyWindow(window);
    QuitAll();
    return 0;
}