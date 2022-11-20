#include "all.h"

static char* _image_file_easy = "easy.png";
static char* _image_file_normal = "normal.png";
static char* _image_file_hard = "hard.png";

int init_start_screen(SDL_Window* window) 
{
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);
    SDL_FillAll(window_surface, SDL_MapRGB(window_surface->format, 0x00, 0x00, 0x00));

    SDL_Surface* img_easy = IMG_Load(_image_file_easy);
    SDL_Surface* img_normal = IMG_Load(_image_file_normal);
    SDL_Surface* img_hard = IMG_Load(_image_file_hard);

    SDL_Point point = { .x = 0, .y = 0 };

    SDL_PutSurface(img_easy, &point, window_surface);
    SDL_PutSurface(img_normal, &point, window_surface);
    SDL_PutSurface(img_normal, &point, window_surface);

    SDL_FreeSurface(img_easy);
    SDL_FreeSurface(img_normal);
    SDL_FreeSurface(img_hard);

    return 0;
}

int run_start_screen(SDL_Window* window)
{
    return 0;
}