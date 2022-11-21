#include "all.h"
#include "game_objects.h"

static char* _image_file_easy = "easy.png";
static char* _image_file_normal = "normal.png";
static char* _image_file_hard = "hard.png";

static int _levelIndex = 0;

static void EasyButtonFunction(SDL_MouseButtonEvent* event, void* parameter)
{
    _levelIndex = 1;
}

static void NormalButtonFunction(SDL_MouseButtonEvent* event, void* parameter)
{
    _levelIndex = 2;
}

static void HardButtonFunction(SDL_MouseButtonEvent* event, void* parameter)
{
    _levelIndex = 3;
}

int InitStartScreen(SDL_Window* window)
{
    _levelIndex = 0;
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);
    SDL_FillAll(window_surface, SDL_MapRGB(window_surface->format, 0x00, 0x00, 0x00));

    SDL_Surface* easy_image = IMG_Load(_image_file_easy);
    SDL_Surface* normal_image = IMG_Load(_image_file_normal);
    SDL_Surface* hard_image = IMG_Load(_image_file_hard);

    SDL_Point point = { .x = 0, .y = 0 };

    SDL_PutSurface(easy_image, &point, window_surface);
    SDL_Rect easy_rect = { .x = point.x, .y = point.y, .w = easy_image->clip_rect.w, .h = easy_image->clip_rect.h };
    point.y += 120;
    SDL_PutSurface(normal_image, &point, window_surface);
    SDL_Rect normal_rect = { .x = point.x, .y = point.y, .w = normal_image->clip_rect.w, .h = normal_image->clip_rect.h };
    point.y += 120;
    SDL_PutSurface(hard_image, &point, window_surface);
    SDL_Rect hard_rect = { .x = point.x, .y = point.y, .w = hard_image->clip_rect.w, .h = hard_image->clip_rect.h };

    RegisterScreenButton(&easy_rect, EasyButtonFunction, NULL);
    RegisterScreenButton(&normal_rect, NormalButtonFunction, NULL);
    RegisterScreenButton(&hard_rect, HardButtonFunction, NULL);

    SDL_FreeSurface(easy_image);
    SDL_FreeSurface(normal_image);
    SDL_FreeSurface(hard_image);
    SDL_UpdateWindowSurface(window);

    return 0;
}

static void EventLoop(SDL_Window* window)
{
    SDL_bool endFlag = SDL_FALSE;

    while (!endFlag)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                endFlag = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                DoButtonAction(&event.button);
                if (_levelIndex > 0)
                {
                    endFlag = SDL_TRUE;
                }
                break;
            default:
                break;
            }
        }
        SDL_Delay(10);
    }
}

int RunStartScreen(SDL_Window* window)
{
    InitStartScreen(window);
    EventLoop(window);
    UnregisterAllScreenButtons();
    return _levelIndex;
}